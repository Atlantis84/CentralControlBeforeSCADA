#include "tcpclient.h"
#include <QDebug>
#include "QsLog.h"
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
#include <QTimerEvent>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include "gdatafactory.h"
#include <QTextCodec>
#include <QThread>
#include <QCoreApplication>
TcpClient::TcpClient()
{
    m_pTcpSocketClient = nullptr;
    IdentityID = -1;
}

bool TcpClient::connect_to_tcp_server(QString sIp, int sPort)
{
    if(m_pTcpSocketClient == nullptr)
        m_pTcpSocketClient = new QTcpSocket(this);
    if(m_pTcpSocketClient->state() != QAbstractSocket::UnconnectedState)
    {
        QLOG_ERROR()<<"the connect state is not UnconnectedState, cannot connect to server!";
        return false;
    }

    m_pTcpSocketClient->connectToHost(sIp,sPort);
    if(m_pTcpSocketClient->waitForConnected(1000))
    {
        connect(m_pTcpSocketClient,&QTcpSocket::readyRead,this,&TcpClient::slot_rev_from_tcp_server);
        QLOG_WARN()<<u8"connect to tcp server SUCCESS!";
        return true;
    }
    else {
        QLOG_ERROR()<<u8"connect to tcp server FAILED!";
        return false;
    }
}

void TcpClient::disconnect_from_server()
{
    if(m_pTcpSocketClient != nullptr)
        m_pTcpSocketClient->disconnectFromHost();
    else
        return;
    QThread::msleep(300);
    QLOG_WARN()<<m_pTcpSocketClient->state();
    if(m_pTcpSocketClient->state() == QAbstractSocket::UnconnectedState)
    {
        disconnect(m_pTcpSocketClient,&QTcpSocket::readyRead,this,&TcpClient::slot_rev_from_tcp_server);
        QLOG_INFO()<<"the client disconnect from server SUCCESS!";
    }
    else
        QLOG_ERROR()<<"the client disconnect from server FAILED!";
}

void TcpClient::send_to_tcp_server(QByteArray data)
{
//    QJsonObject  root;
//    root.insert("ProductSn",QString(data));

//    QJsonDocument doc_data(root);
//    QByteArray request_data = doc_data.toJson(QJsonDocument::Compact);
    QLOG_WARN()<<u8"the sending JSON info is:"<<QString(data);

    m_pTcpSocketClient->write(data,data.length());
    m_pTcpSocketClient->flush();
}

void TcpClient::slot_rev_from_tcp_server()
{
    QJsonParseError parseJsonErr;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(m_pTcpSocketClient->readAll(),&parseJsonErr);
    QLOG_WARN()<<u8"测试软件转产结果为:"<<jsonDocument.toJson(QJsonDocument::Compact);
    if(!(parseJsonErr.error == QJsonParseError::NoError))
        QLOG_WARN()<<"parse json failed!";
    else
    {
        QJsonObject jsonObject = jsonDocument.object();
        if(this->IdentityID == 1)
        {
            if(jsonObject.value("requestType").toString() == "TBOX_TEST")
            {
                if(jsonObject.value("code").toString() == "OK")
                {
                    QLOG_INFO()<<u8"机台1换产成功!";
                    emit signal_rev_result(1);
                }
                else
                    QLOG_INFO()<<u8"机台1换产失败!";
            }
        }

        if(this->IdentityID == 2)
        {
            if(jsonObject.value("requestType").toString() == "TBOX_TEST")
            {
                if(jsonObject.value("code").toString() == "OK")
                {
                    QLOG_INFO()<<u8"机台2换产成功!";
                    emit signal_rev_result(2);
                }
                else
                    QLOG_INFO()<<u8"机台2换产失败!";
            }
        }

        if(this->IdentityID == 3)
        {
            if(jsonObject.value("requestType").toString() == "TBOX_TEST")
            {
                if(jsonObject.value("code").toString() == "OK")
                {
                    QLOG_INFO()<<u8"机台3换产成功!";
                    emit signal_rev_result(3);
                }
                else
                    QLOG_INFO()<<u8"机台3换产失败!";
            }
        }

        if(this->IdentityID == 4)
        {
            if(jsonObject.value("requestType").toString() == "TBOX_TEST")
            {
                if(jsonObject.value("code").toString() == "OK")
                {
                    QLOG_INFO()<<u8"机台4换产成功!";
                    emit signal_rev_result(4);
                }
                else
                    QLOG_INFO()<<u8"机台4换产失败!";
            }
        }
    }
}
