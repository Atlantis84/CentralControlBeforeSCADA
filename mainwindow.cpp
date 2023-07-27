#include "mainwindow.h"
#include <qdesktopwidget.h>
#include <QApplication>
#include "TitleBar.h"
#include <QPainter>
#include "gdatafactory.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    this->setStyleSheet("QMainWindow{background-color:#000000}");
    resize(1000,800);
    QDesktopWidget* desktop = QApplication::desktop();
    int w,h;
    w = desktop->screenGeometry().width();
    h = desktop->screenGeometry().height();
    this->setGeometry(w/2-700,h/2-400,1400,800);

    GDataFactory::get_central_wgt()->setGeometry(0,40,this->width(),this->height()-40);
    GDataFactory::get_central_wgt()->setParent(this);
    GDataFactory::get_central_wgt()->show();

    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"数据采集与监视控制系统");
    pTitleBar->setFixedWidth(1400);
    installEventFilter(pTitleBar);
}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(240,242,245)));
    return;
}
