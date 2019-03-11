#include "tempandhum.h"



tempandhum::tempandhum(QObject *parent) : QObject(parent)
{
    return;
}
tempandhum::tempandhum(int node)
{
    isExist = true;//收到包则创建，说明开始存在
    NodeSeq = node;
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(setNotExist()));
    timer->start(TIMER_SET);
    qDebug()<<"node:"<<NodeSeq<<"is being created!"<<endl;
}
tempandhum::~tempandhum()
{
    if(timer->isActive())
        timer->stop();
    delete timer;
}
void tempandhum::Packet_getInfo(QByteArray packet)
{
    bool ok;
    int GH,GL;//光强高低8位
    temp = packet.mid(10,2).toInt(&ok,16)*256 +  packet.mid(12,2).toInt(&ok,16);

    hum = packet.mid(14,2).toInt(&ok,16)*256 +  packet.mid(16,2).toInt(&ok,16);
    GH = packet.mid(18,2).toInt(&ok,16);
    GL = packet.mid(20,2).toInt(&ok,16);
    light = (GH*256+GL)*3012.9/(32768*4);

}
void tempandhum::setNotExist()
{
    isExist = false;
    if(timer->isActive())//停止定时器，等待重启
        timer->stop();
    qDebug()<<"node:"<<NodeSeq<<"is not exist!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
}
void tempandhum::startTimer()
{
    timer->start(TIMER_SET);
    isExist = true;

    qDebug()<<"node:"<<NodeSeq<<"is exist!"<<endl;
}
bool tempandhum::checkNode(int node)
{
    return node == NodeSeq;
}
bool tempandhum::checkState()
{
    return isExist;
}
int tempandhum::getTemp()
{
    return temp;
}
int tempandhum::getHum()
{
    return hum;
}
int tempandhum::getLight()
{
    return light;
}
/***/
int tempandhum::getSeq()
{
    return NodeSeq;
}
/***/
