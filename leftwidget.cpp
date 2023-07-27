#include "leftwidget.h"
#include <QPainter>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QList>
#include <QDebug>
#include "gdatafactory.h"
#include <QStyleFactory>
LeftWidget::LeftWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();
    QVBoxLayout* vBottom = new QVBoxLayout();

    QTreeWidget *treeWidget = new QTreeWidget();
    connect(treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(slot_getPath(QTreeWidgetItem*,int)));
    treeWidget->setColumnCount(1);
    treeWidget->setFont(QFont(u8"KaiTi",13,QFont::Normal,false));
//    treeWidget->setRootIsDecorated(false);
    treeWidget->setStyleSheet("QTreeWidget{color:rgb(255,255,255);}QTreeView::item{ padding:5px; } ");
    QList<QTreeWidgetItem*> items;
    for (int i=0;i<3;i++)
    {
        if(i==0)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem((QTreeWidget*)0,QStringList(QString(u8"换产管理")));
            item->setIcon(0,QIcon(":/icon/root2.png"));
            items.append(item);

            QTreeWidgetItem* childitem = new QTreeWidgetItem(items.at(i),QStringList(QString(u8"一键换产(TCON)")));
            childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
            items.at(i)->addChild(childitem);

            QTreeWidgetItem* childitem1 = new QTreeWidgetItem(items.at(i),QStringList(QString(u8"一键换产(TBOX)")));
            childitem1->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
            items.at(i)->addChild(childitem1);
        }
        else if(i == 1)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem((QTreeWidget*)0,QStringList(QString(u8"单工站管理")));
            item->setIcon(0,QIcon(":/icon/root4.png"));
            items.append(item);

            for(int j=0;j<4;j++)
            {
                if(j==0)
                {
                    QTreeWidgetItem* childitem = new QTreeWidgetItem(items.at(i),QStringList(QString(u8"灯条擦拭")));
                    childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
                    items.at(i)->addChild(childitem);
                }
                else if(j==1){
                    QTreeWidgetItem* childitem = new QTreeWidgetItem(items.at(i),QStringList(QString(u8"分板上下料")));
                    childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
                    items.at(i)->addChild(childitem);
                }
//                else if(j == 2){
//                    QTreeWidgetItem* childitem = new QTreeWidgetItem(items.at(i),QStringList(QString(u8"TBOX")));
//                    childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
//                    items.at(i)->addChild(childitem);
//                }
                else if(j == 2){
                    QTreeWidgetItem* childitem = new QTreeWidgetItem(items.at(i),QStringList(QString(u8"LTE")));
                    childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
                    items.at(i)->addChild(childitem);
                }
                else if(j == 3){
                    QTreeWidgetItem* childitem = new QTreeWidgetItem(items.at(i),QStringList(QString(u8"物联自动包装")));
                    childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
                    items.at(i)->addChild(childitem);
                }
            }
        }
        else
        {
            QTreeWidgetItem* item = new QTreeWidgetItem((QTreeWidget*)0,QStringList(QString(u8"其他操作")));
            item->setIcon(0,QIcon(":/icon/root3.png"));
            items.append(item);
            for (int j=0;j<5;j++)
            {
                if(j == 0)
                {
                    QTreeWidgetItem* childitem = new QTreeWidgetItem(items.at(i),QStringList(QString(u8"实时日志")));
                    childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
                    items.at(i)->addChild(childitem);
                }
                else if(j == 1)
                {
                    QTreeWidgetItem* childitem = new QTreeWidgetItem(items.at(i),QStringList(QString(u8"数据管理")));
                    childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
                    items.at(i)->addChild(childitem);
                }
                else if (j == 2){
                    QTreeWidgetItem* childitem = new QTreeWidgetItem(items.at(i),QStringList(QString(u8"站点监视")));
                    childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
                    items.at(i)->addChild(childitem);
                }
                else if (j == 3){
                    QTreeWidgetItem* childitem = new QTreeWidgetItem(items.at(i),QStringList(QString(u8"用户管理")));
                    childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
                    items.at(i)->addChild(childitem);
                }
                else {
                    QTreeWidgetItem* childitem = new QTreeWidgetItem(items.at(i),QStringList(QString(u8"操作%1").arg(j)));
                    childitem->setFont(0,QFont(u8"KaiTi",12,QFont::Normal,false));
                    items.at(i)->addChild(childitem);
                }
            }
        }
    }
    treeWidget->insertTopLevelItems(0,items);
    treeWidget->setHeaderHidden(true);
    treeWidget->setStyle(QStyleFactory::create("windows"));

    //    treeWidget->setItemsExpandable(false);
    treeWidget->expandAll();

    vBottom->addWidget(treeWidget);
    vAll->addSpacing(70);
    vAll->addLayout(vBottom);
    vAll->setMargin(0);
    this->setLayout(vAll);
}

void LeftWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(22,194,194)));
    int headHeight = this->height()/13;
    QRect rtHead(0,0,this->width(),headHeight);
    painter.fillRect(rtHead,QBrush(QColor(0,166,170)));
    QRect rtHeadLeft(10,15,this->width()/2-20,headHeight-30);
    QRect rtHeadRight(this->width()/2,0,this->width()/2,headHeight);
    painter.fillRect(rtHeadLeft,QBrush(QColor(255,255,255,255)));

    painter.setFont(QFont(u8"KaiTi",20,QFont::Normal,false));

    painter.save();
    painter.setPen(QColor(27,162,160));
//    painter.drawText(rtHeadLeft,Qt::AlignHCenter | Qt::AlignVCenter,u8"Hisense");
    painter.drawPixmap(rtHeadLeft,QPixmap(":/icon/2.png"));
    painter.restore();

    painter.setPen(Qt::white);
    painter.drawText(rtHeadRight,Qt::AlignHCenter | Qt::AlignVCenter,u8"中控系统");

}

void LeftWidget::slot_getPath(QTreeWidgetItem *wItem, int count)
{
    Q_UNUSED(count);
    QStringList filePath;
    QTreeWidgetItem *itemFile = wItem;
    while (itemFile != nullptr)
    {
        filePath<<itemFile->text(0);
        itemFile = itemFile->parent();
    }

    QString strPath;
    for (int i=filePath.size()-1;i>=0;i--)
    {
        strPath +=filePath.at(i);
        if(i!=0)
            strPath+="/";
    }
//    qDebug()<<strPath;
//    QByteArray tmpArray;
//    tmpArray.append(0x61);
//    qDebug()<<tmpArray;
    if(strPath.contains(u8"实时日志"))
        emit signal_create_tab_item(3,u8"实时日志");
//        if(GDataFactory::get_factory()->get_current_usr_name() != "fbj")
//            GDataFactory::get_logs_widget()->show();

//    if(strPath.contains(u8"数据管理"))
//        GDataFactory::get_history_display_wgt()->show();
//    if(strPath.contains(u8"站点监视"))
//        GDataFactory::get_station_monitor_wgt()->show();
    if(strPath.contains(u8"操作3"))
    {
        if((GDataFactory::get_factory()->get_current_usr_name() == "tcon")||
                (GDataFactory::get_factory()->get_current_usr_name() == "admin"))
            GDataFactory::get_factory()->truncate_change_info_table_pure();
    }
    if(strPath.contains(u8"灯条擦拭"))
    {
        if((GDataFactory::get_factory()->get_current_usr_name() == "myl")||
                (GDataFactory::get_factory()->get_current_usr_name() == "admin"))
            emit signal_create_tab_item(1,u8"灯条擦拭");
//            GDataFactory::get_factory()->get_light_bar_wgt()->show();
    }
    if(strPath.contains(u8"用户管理"))
    {
        if(GDataFactory::get_factory()->get_current_usr_name() == "admin")
            GDataFactory::get_user_tab_wgt()->show();
    }

    if(strPath.contains(u8"分板上下料"))
    {
        if((GDataFactory::get_factory()->get_current_usr_name() == "fbj")||
                (GDataFactory::get_factory()->get_current_usr_name() == "admin"))
            emit signal_create_tab_item(2,u8"分板上下料");
    }

    if(strPath.contains(u8"一键换产(TCON)"))
        if((GDataFactory::get_factory()->get_current_usr_name() == "tcon")||
                (GDataFactory::get_factory()->get_current_usr_name() == "admin"))
            emit signal_create_tab_item(0,u8"一键换产");

    if(strPath.contains(u8"一键换产(TBOX)"))
        if((GDataFactory::get_factory()->get_current_usr_name() == "tbox")||
                (GDataFactory::get_factory()->get_current_usr_name() == "admin"))
            emit signal_create_tab_item(4,u8"TBOX");

    emit signal_send_path(strPath);
}
