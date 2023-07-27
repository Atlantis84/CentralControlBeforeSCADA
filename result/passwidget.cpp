#include "passwidget.h"
#include <QPainter>
#include "gdatafactory.h"
PassWidget::PassWidget(QWidget *parent) : QWidget(parent)
{
//    m_pTimeID = startTimer(1000);
}

static int testSign = 0;
void PassWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
//    if(testSign % 2 == 0)
//        painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));
//    else
        painter.fillRect(this->rect(),QBrush(QColor(255,255,255)));

    int interval;
    QRect rt;
    if(this->width()>this->height())
    {
        interval = (this->width()-this->height())/2;
        rt.setTopLeft(QPoint(interval+10,10));
        rt.setWidth(this->height()-20);
        rt.setHeight(this->height()-20);
    }
    else
    {
        interval = (this->height()-this->width()-20)/2;
        rt.setTopLeft(QPoint(10,interval));
        rt.setWidth(this->width()-20);
        rt.setHeight(this->width()-20);
    }

    QFont ft(u8"KaiTi",100,QFont::Bold,false);
    ft.setUnderline(true);
    painter.setFont(ft);
    if(testSign%2 == 0)
    {
        painter.setPen(QPen(QColor(0,0,255),8));
        painter.drawEllipse(rt);
        painter.setPen(QPen(QColor(0,0,255),50));
        painter.drawText(rt,Qt::AlignHCenter | Qt::AlignVCenter,u8"PASS");
    }
    else {
        painter.setPen(QPen(QColor(255,0,0),8));
        painter.drawEllipse(rt);
        painter.setPen(QPen(QColor(255,0,0),50));
        painter.drawText(rt,Qt::AlignHCenter | Qt::AlignVCenter,u8"NG");
    }
}

void PassWidget::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_pTimeID)
    {
        testSign++;
        this->repaint();
    }

}
