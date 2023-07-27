#include "udpservice.h"
#include "gdatafactory.h"
#include <QNetworkDatagram>
#include "QsLog.h"
#include <QEventLoop>
#include <QTimer>
#include "gdatafactory.h"
#include "QsLog.h"
#include <QThread>
UdpService* UdpService::_instance = nullptr;
UdpService::UdpService()
{
    m_pUdpSocket = new QUdpSocket(this);
//    m_pUdpSocket->bind(QHostAddress(GDataFactory::get_instance()->get_config_para("PLC_IP")),GDataFactory::get_instance()->get_config_para("PLC_PORT").toInt());
    connect(m_pUdpSocket,SIGNAL(readyRead()),this,SLOT(slot_receive_message()));
    connect(this,SIGNAL(signal_proc_plc_msg(const QByteArray)),this,SLOT(slot_proc_plc_msg(const QByteArray)));
    connect(this,SIGNAL(signal_proc_real_data(QByteArray)),this,SLOT(slot_proc_real_data(QByteArray)));
    qRegisterMetaType<QByteArray>("QByteArray");
}

UdpService::~UdpService()
{
}

void UdpService::release_res()
{
    disconnect(m_pUdpSocket,SIGNAL(readyRead()),this,SLOT(slot_receive_message()));
    disconnect(this,SIGNAL(signal_proc_plc_msg(const QByteArray)),this,SLOT(slot_proc_plc_msg(const QByteArray)));
    if(m_pUdpSocket != nullptr)
        delete m_pUdpSocket;
    m_pUdpSocket = nullptr;
}

void UdpService::send_message_to_plc(const uchar function_code, const short data_address,const short data_length/*length is word length*/,const QByteArray data_array)
{
    m_mutex.lock();
    QHostAddress hAddr(GDataFactory::get_factory()->get_config_para("PLC_IP"));
    int hPort = GDataFactory::get_factory()->get_config_para("PLC_PORT").toInt();
    QByteArray btArray = make_plc_message(function_code,data_address,data_length,data_array);
    QLOG_INFO()<<"Send Msg To PLC:"<<GDataFactory::get_factory()->bytes_to_str(btArray);
    m_pUdpSocket->writeDatagram(btArray,btArray.length(),hAddr,static_cast<quint16>(hPort));
    m_pUdpSocket->flush();
    m_mutex.unlock();
}

QByteArray UdpService::make_plc_message(const uchar function_code, const short data_address,const short data_length/*length is word length*/,const QByteArray data_array)
{
    QByteArray m_data;
    m_data.append(static_cast<char>(0x80));  //ÆðÊ¼Âë
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x02));
    m_data.append(static_cast<char>(0x00));  //PLC IP Address
    m_data.append(static_cast<char>(GDataFactory::get_factory()->get_last_ip(1)));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x00));  //self IP Address
    m_data.append(static_cast<char>(GDataFactory::get_factory()->get_last_ip(0)));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0xFF));  //SID
    m_data.append(static_cast<char>(0x01));  //FINS

    switch (function_code)
    {
    case 1://read plc
        m_data.append(static_cast<char>(0x01));
        break;
    case 2://write plc
        m_data.append(static_cast<char>(0x02));
        break;
    }

    m_data.append(static_cast<char>(0x82));
    m_data.append(static_cast<char>(0xFF&(data_address>>8)));
    m_data.append(static_cast<char>(0xFF&data_address));
    m_data.append(static_cast<char>(0x00));
    int length = data_length/2;//plc recognize by word
    m_data.append(static_cast<char>(0xFF&(length>>8)));
    m_data.append(static_cast<char>(0xFF&length));

    if(function_code == 2)
        m_data.append(data_array);

    return m_data;
}

void UdpService::slot_clear_read_sign()
{
    QByteArray clearData;
    clearData.append(static_cast<char>(0x00));
    clearData.append(static_cast<char>(0x00));
//    send_message_to_plc(WRITE_PLC,ADDRESS_D10010,0x02,clearData);
}

static int testCount = 0;
void UdpService::slot_proc_real_data(QByteArray data)
{
    if(data.length() == 4)//light bar info
    {
        short a,b;
        a = static_cast<short>(data[1]);
        b = static_cast<short>(data[3]);
        if((a == 1) && (b != 0))
        {
            QByteArray clearData;
            clearData.append(static_cast<char>(0x00));
            clearData.append(static_cast<char>(0x00));
            clearData.append(static_cast<char>(0x00));
            clearData.append(static_cast<char>(0x00));
            send_message_to_plc(WRITE_PLC,ADDRESS_D10040,0x04,clearData);
            //finish washing
            emit signal_finish_washing(static_cast<int>(data[3]+data[2]));
        }
        else
            ;
    }
    else if(data.length() == 2)//fbj sign
    {
        if(data[0] || data[1])
        {
            QByteArray clearData;
            clearData.append(static_cast<char>(0x00));
            clearData.append(static_cast<char>(0x00));
            send_message_to_plc(WRITE_PLC,ADDRESS_D10010,0x02,clearData);
            QEventLoop eventloop;
            QTimer::singleShot(500,&eventloop,&QEventLoop::quit);
            send_message_to_plc(READ_PLC,ADDRESS_D10000,0x14,nullptr);
        }
    }
    else if(data.length() == 20)//fbj SN
    {
        testCount++;
        QLOG_WARN()<<"the current SN is:"<<data;
        QByteArray qByteArray;
        qByteArray.append(data);
        qByteArray.append(static_cast<char>(0x00));
        qByteArray.append(static_cast<char>(0x01));
//        if(testCount%2 == 0)
//            qByteArray.append(static_cast<char>(0x02));
//        else
//            qByteArray.append(static_cast<char>(0x01));

        send_message_to_plc(WRITE_PLC,ADDRESS_D10020,static_cast<short>(qByteArray.length()),qByteArray);
    }
    else
    {
        ;
    }
}

void UdpService::slot_receive_message()
{
    m_mutex.lock();
    if(m_pUdpSocket == nullptr)
        return;
    QByteArray datagram;
    datagram.resize(static_cast<int>(m_pUdpSocket->pendingDatagramSize()));
    while(m_pUdpSocket->hasPendingDatagrams())
    {
        m_pUdpSocket->readDatagram(datagram.data(), datagram.size());
    }
    m_mutex.unlock();
    emit signal_proc_plc_msg(datagram);
}

void UdpService::slot_proc_plc_msg(const QByteArray data_array)
{
    if(data_array.length() > 10)//check the header length
    {
        if((data_array[9] & 0xFF) == 0xFF)
        {
            QLOG_INFO()<<"REV PLC MSG:"<<GDataFactory::get_factory()->bytes_to_str(data_array);
            if((data_array[10] == 0x01) && (data_array[11] == 0x01))
            {
                if((data_array[12] == 0x00)&&(data_array[13] == 0x00))
                {
                    QByteArray tmpArray;
                    for(int i=14;i<data_array.length();i++)
                        tmpArray.append(data_array[i]);
                    emit signal_proc_real_data(tmpArray);
                }
                else
                {
                    QLOG_WARN()<<"PLC read back message is ABNORMAL!";
                }
            }
            else if((data_array[10] == 0x01) && (data_array[11] == 0x02))
            {
                if((data_array[12] == 0x00)&&(data_array[13] == 0x00))
                {
                    ;
                }
                else
                {
                    QLOG_WARN()<<"PLC write back message is ABNORMAL!";
                }
            }
        }
    }
    else
    {
        QLOG_WARN()<<"PLC message header lenght is WRONG!";
    }
}

void UdpService::slot_query_PLC_sign()
{
//    send_message_to_plc(READ_PLC,ADDRESS_D10010,0x02,nullptr);
}

void UdpService::slot_send_Mes_info_to_PLC(const QString SN, const short OK_NG, const short standard_num, const short current_num)
{
    QByteArray data;
    data.append(SN.toUtf8());
    for(int i = data.length();i<20;i++)
        data.append(static_cast<char>(0x00));
    data.append(static_cast<char>(0xFF&(OK_NG>>8)));
    data.append(static_cast<char>(0xFF&OK_NG));

    data.append(static_cast<char>(0xFF&(standard_num>>8)));
    data.append(static_cast<char>(0xFF&standard_num));

    data.append(static_cast<char>(0xFF&(current_num>>8)));
    data.append(static_cast<char>(0xFF&current_num));

//    send_message_to_plc(WRITE_PLC,ADDRESS_D10020,data.length(),data);
}
