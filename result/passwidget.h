#ifndef PASSWIDGET_H
#define PASSWIDGET_H

#include <QWidget>

class PassWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PassWidget(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    int m_pTimeID;
signals:

public slots:
};

#endif // PASSWIDGET_H
