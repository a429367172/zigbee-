#ifndef TEMPANDHUM_H
#define TEMPANDHUM_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#define TIMER_SET 7000
class tempandhum : public QObject
{
    Q_OBJECT
public:
    explicit tempandhum(QObject *parent = nullptr);
private:
    bool isExist;
    int NodeSeq;
    int temp;
    int hum;
    int light;
    QTimer *timer;
public:
    tempandhum(int node);
    //explicit tempandhum(const tempandhum&);
    ~tempandhum();
    void Packet_getInfo(QByteArray packet);//从packet中获取温湿度信息,并给temp,hum等赋值
    void startTimer();//重启定时器
    bool checkState();//返回是否掉电的状态isExit
    bool checkNode(int NodeSeq);//检查编号为NodeSeq的节点是否是当前对象节点
    int getTemp();//返回temp
    int getHum();
    int getLight();
    /****/
    int getSeq();
    /****/
public slots:
    void setNotExist();//设置状态isExist
};

#endif // TEMPANDHUM_H
