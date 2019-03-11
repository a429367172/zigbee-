#include "motor.h"

motor::motor(QObject *parent) : QObject(parent)
{
    return;
}
motor::motor(int node)
{
    isExist = true;
    NodeSeq = node;
    memset(LED,0,sizeof(int)*4);
    memset(Mtr,0,sizeof(int)*2);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(setNotExist()));
    timer->start(TIMER_SET);
}
motor::~motor()
{
    if(timer->isActive())
        timer->stop();
    delete timer;
}
void motor::Packet_getInfo(QByteArray packet)
{
    UART_Format_End1 UART_END1;//格式：长度为7位的包
    packet = QByteArray::fromHex(packet);
    memcpy(&UART_END1,packet,sizeof(UART_Format_End1));
    //int LED_1,LED_2,LED_3,LED_4,LG9110_MF,LG9110_MR;//LED灯和电机转动的状态
    LED[0] = (UART_END1.Data[0] & 0x80)?1:0;
    LED[1] = (UART_END1.Data[0] & 0x40)?1:0;
    LED[2] = (UART_END1.Data[0] & 0x20)?1:0;
    LED[3] = (UART_END1.Data[0] & 0x10)?1:0;
    Mtr[0] = (UART_END1.Data[0] & 0x08)?1:0;//是否正传
    Mtr[1] = (UART_END1.Data[0] & 0x04)?1:0;//是否反转
}
void motor::startTimer()
{
    isExist = true;
    timer->start(TIMER_SET);
}
bool motor::checkState()
{
    return isExist;
}
bool motor::checkNode(int node)
{
    return node == NodeSeq;
}
int *motor::getLED()
{
    return LED;
}
int *motor::getMotor()
{
    return Mtr;
}
void motor::setNotExist()
{
    isExist = false;
    if(timer->isActive())
        timer->stop();
}
/*******************/
int motor::getSeq()
{
    return NodeSeq;
}
