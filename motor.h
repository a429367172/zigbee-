#ifndef MOTOR_H
#define MOTOR_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include "public.h"
#define TIMER_SET 7000
class motor : public QObject
{
    Q_OBJECT
public:
    explicit motor(QObject *parent = nullptr);
private:
    bool isExist;
    int NodeSeq;
    int LED[4];//led状态
    int Mtr[2];//MF,和MR状态
    QTimer *timer;
public:
    motor(int node);
    ~motor();
    void Packet_getInfo(QByteArray packet);//从packet中获取电机信息
    void startTimer();//重启定时器
    bool checkState();//返回是否掉电的状态isExit
    bool checkNode(int node);//检查编号为NodeSeq的节点是否是当前对象节点
    int *getLED();
    int *getMotor();
    int getSeq();

public slots:
    void setNotExist();//设置状态isExist
};

#endif // MOTOR_H
