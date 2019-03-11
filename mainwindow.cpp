#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "public.h"
#include <QDateTime>
#include <QTimer>
#include <QVector>
#include <QDebug>








/**********************分割线********************************************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    processBufQueTimer.start(500);
    updateTimer.start(500);
    /*cck*/
    f1 = new form;
    /*cck*/

    connect(&updateTimer,SIGNAL(timeout()),this,SLOT(processBufQue()));
    connect(&processBufQueTimer,SIGNAL(timeout()),this,SLOT(debug()));
   // connect(Slider, SIGNAL(valueChanged(int)), this, SLOT(sendPwm(int)));

    /*cck*/
    connect(f1,SIGNAL(sendsignal(QString,QString)),this,SLOT(reshow(QString,QString)));

    setupRealtimelight(ui->qwtPlot);
    setupRealtimeTemperature(ui->qwtPlot_2);
    setupRealtimeHumidity(ui->qwtPlot_3);
    //qDebug("666");
    //setup_Table();
    //qDebug("666");
    /************************************************************************/
    connect(&updateTimer,SIGNAL(timeout()),this,SLOT(updateNum()));
    connect(&updateTimer,SIGNAL(timeout()),this,SLOT(update_DataofNode()));
    //connect (定义一个时钟，和updaNnum（）连在一起)  更新第一页平均数据
    //connect (定义一个时钟，和update_DataofNode()连在一起)  更新各节点数据
    /**************************************************************************/
    /*cck*/
}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_OpenSerialButton_clicked()
{
    if(ui->OpenSerialButton->text() == tr("打开串口"))
    {
        //设置串口名
        serial = new QSerialPort;
        serial->setPortName(ui->portBox->currentText());
        //打开串口
        serial->open(QIODevice::ReadWrite);
        //设置波特率
        serial->setBaudRate(115200);//设置波特率
        //设置数据位数
        serial->setDataBits(QSerialPort::Data8);//设置数据位8
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);//停止位设置为1
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);//设置为无流控制
        //关闭设置菜单使能
        ui->portBox->setEnabled(false);
        ui->OpenSerialButton->setText(tr("关闭串口"));
        //连接信号槽
        QObject::connect(serial,&QSerialPort::readyRead,this,&MainWindow::ReadData);

    }
    else
    {
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();
        //恢复设置使能
        ui->portBox->setEnabled(true);
        ui->OpenSerialButton->setText(tr("打开串口"));

    }
    //QObject::connect(serial,&QSerialPort::readyRead,this,&MainWindow::ReadData);
}
void MainWindow::on_searchButton_clicked()
{
    foreach (const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            if(ui->portBox->findText(serial.portName())==-1)
            {
                ui->portBox->addItem(serial.portName());
            }
            serial.close();
        }
    }
}
void MainWindow::Packet_showInfo(QByteArray packet,int NodeSeq)
{

    //int len = packet.mid(2,2).toInt(&ok,16)*2;
    int type = packet.mid(6,2).toInt(&ok,16);
    switch (type) {
    case 2://温湿度传感器发来的信息

        //QVector<tempandhum*>::iterator iter;
        for (iter=temp_nodes.begin();iter!=temp_nodes.end();iter++)
        {
            if((*iter)->checkNode(NodeSeq))//找到该节点
            {
                (*iter)->Packet_getInfo(packet);//首先处理包，获取温湿度信息

                //显示编号为NodeSeq的温度  编号：NodeSeq 温度： (*iter)->getTemp()
                //qDebug()<<(*iter)->getSeq()<<"temp:" << (*iter)->getTemp()<<endl;
                //显示编号为NodeSeq的湿度   (*iter)->getHum()
                //qDebug()<<"hum:" << (*iter)->getHum()<<endl;
                //显示编号为NodeSeq的光照强度 (*iter)->getLight()
                //qDebug()<<"lightx:" << (*iter)->getLight()<<endl;
                break;
            }
        }
        break;
    case 6://电机模块
        //QVector<motor*>::iterator iter2;
        for (iter2=motor_nodes.begin();iter2!=motor_nodes.end();iter2++)
        {
            if((*iter2)->checkNode(NodeSeq))//找到该节点
            {
                (*iter2)->Packet_getInfo(packet);//首先处理包，获取温湿度信息
                int *LED;
                int *Mtr;
                LED = (*iter2)->getLED();
                Mtr = (*iter2)->getMotor();
                //显示 编号： NodeSeq 4个灯的状态： LED[0]  LED[1]  LED[2]  LED[3]
                qDebug()<<"node: "<<NodeSeq<<"led1: "<<LED[0]<<"led2: "<<LED[1]<<"led3: "<<LED[2]<<"led4: "<<LED[3];
                qDebug()<<"node: "<<NodeSeq<<"MF: "<<Mtr[0]<<"MG: "<<Mtr[1]<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
                //显示 编号： NodeSeq  电机是否正转:   Mtr[0]  电机是否反转：  Mtr[1]
                break;
            }
        }
        break;
    default:
        break;
    }
}
void MainWindow::Packet_checkExit(QByteArray packet,int NodeSeq)
{

    int type = packet.mid(6,2).toInt(&ok,16);
    int flag;
    switch (type) {
    case 2://温湿度传感器发来的信息
        //QVector<tempandhum*>::iterator iter;
        flag = 0;
        for (iter=temp_nodes.begin();iter!=temp_nodes.end();iter++)
        {
            if((*iter)->checkNode(NodeSeq))//找到该节点
            {
                flag = 1;
                (*iter)->startTimer();//重新计时
                break;
            }
        }
        if(!flag)//没有该节点，新建
        {
           tempandhum  *temp = new tempandhum(NodeSeq);
           temp_nodes.append(temp);
           flag=0;
           /*cck add*/
           QString s = "节点"+ QString::number(NodeSeq);
           ui->comboBox->addItem(s);
           //
        }
        break;
    case 6://电机模块
        //QVector<motor*>::iterator iter2;
        flag = 0;
        for (iter2=motor_nodes.begin();iter2!=motor_nodes.end();iter2++)
        {
            if((*iter2)->checkNode(NodeSeq))//找到该节点
            {
                flag = 1;
                (*iter2)->startTimer();//重新计时
                break;
            }
        }
        if(!flag)//没有该节点，新建
        {
           motor *mtr = new motor (NodeSeq);
           motor_nodes.append(mtr);
           flag=0;
        }
        break;
    default:
        break;
    }
}
void MainWindow::Process_Packet(QByteArray packet)
{
    if(packet.mid(8,2) == "AA")//心跳消息
        Packet_checkExit(packet,packet.mid(4,2).toInt(&ok,16));
    else if(packet.mid(8,2) == "DD"||packet.mid(8,2) == "01")//返回消息
        Packet_showInfo(packet,packet.mid(4,2).toInt(&ok,16));
}

void MainWindow::ReadData()
{
    QByteArray buf;
    buf = serial->readAll();

    if(!buf.isEmpty())
    {
        bufQue.append(buf.toHex().toUpper());
        QString str = ui->textEdit->toPlainText();
        str+=tr(buf.toHex())+'\n';
        ui->textEdit->clear();
        ui->textEdit->append(str);
    }
    buf.clear();
}
void MainWindow:: processBufQue()
{
    QByteArray packg;
    while (!bufQue.isEmpty()) {
        while (bufQue.mid(0,2)!="40"&&!bufQue.isEmpty()) {
            bufQue = bufQue.right(bufQue.size()-1);
        }
        int len = bufQue.mid(2,2).toInt(&ok,16)*2;
        packg = bufQue.mid(0,len).data();
        if (CheckSum(packg,len)==packg.mid(len-2,2).toInt(&ok,16))
        {
            bufQue = bufQue.right(bufQue.size()-len);
        }
        else {
            bufQue = bufQue.right(bufQue.size()-1);
        }
        Process_Packet(packg);
    }
}
int MainWindow:: CheckSum(QByteArray packg,int len)
{
  int i,sum=0;
  bool ok;
  for(i=0;i< len-3;i+=2)
  {
    sum+=packg.mid(i,2).toInt(&ok,16);
  }
  return sum%256;
}

/*判断类型ID，显示传感器信息*/


















void MainWindow::on_pushButton_clicked()
{
    serial->write(ui->textEdit_2->toPlainText().toLatin1());
}


void MainWindow::sendPwm(int)
{

//    QString st = QString::number( ui->horizontalSlider->value(),10);
//    qDebug()<<st;
//    ui->label_2->setText(st);
}


void MainWindow::on_motorButton_clicked()
{
    if(ui->motorButton->text() == tr("开始通风"))
    {
        UART_Format_Control UART_Control;
        UART_Control.Header= 0x40;
        UART_Control.Len=0x06;
        UART_Control.NodeSeq = 0x01;
        UART_Control.NodeID = Motor;
        UART_Control.Command = 0x0a;
        UART_Control.Verify = 0x57;
        QByteArray st;
        st.resize(sizeof(UART_Control));
        memcpy(st.data(),&UART_Control,sizeof(UART_Format_Control));

        serial->write(st);//发送到串
        ui->motorButton->setText(tr("正在通风"));
     }
    else
        {
        UART_Format_Control UART_Control;
        UART_Control.Header= 0x40;
        UART_Control.Len=0x06;
        UART_Control.NodeSeq = 0x01;
        UART_Control.NodeID = Motor;
        UART_Control.Command = 0x0c;
        UART_Control.Verify = 0x59;
        QByteArray st;
        st.resize(sizeof(UART_Control));
        memcpy(st.data(),&UART_Control,sizeof(UART_Format_Control));
        serial->write(st);//发送到串
        ui->motorButton->setText(tr("开始通风"));
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    UART_Format_Control UART_Control;
    UART_Control.Header= 0x40;
    UART_Control.Len=0x06;
    UART_Control.NodeSeq = 0x01;
    UART_Control.NodeID = Motor;
    UART_Control.Command = 0x05;
    UART_Control.Verify = 0x52;
QByteArray st1;
    st1.resize(sizeof(UART_Control));
    memcpy(st1.data(),&UART_Control,sizeof(UART_Format_Control));
    serial->write(st1);//发送到串
}

/*cck*/
void MainWindow::on_setButton_clicked()
{
    f1->show();
    this->hide();
}
void MainWindow::reshow(QString temp, QString hum)
{
    this->show();
    temper_set = temp;
    hum_set = hum;
    qDebug("temp: "+temp.toLatin1()+"hum: "+hum.toLatin1());
}







void MainWindow::debug()
{
    for (iter=temp_nodes.begin();iter!=temp_nodes.end();iter++)
    {
        int p=(*iter)->getSeq();
        QString str= QString::number(p, 10);
        qDebug()<<"debug  "<<str;
        qDebug()<<"debug  "<<(*iter)->getSeq()<<"temp:" << (*iter)->getTemp()<<endl;
    }
}








/****/
void MainWindow::update_DataofNode()
{
    /***温度***/
    for(int i = ui->tableWidget->rowCount();i >0;i--)
       {
          ui->tableWidget->removeRow(i-1);
       }
    for(int i = ui->tableWidget_3->rowCount();i >0;i--)
       {
          ui->tableWidget_3->removeRow(i-1);
       }
    for(int i = ui->tableWidget_2->rowCount();i > 0;i--)
       {
          ui->tableWidget_2->removeRow(i-1);
       }
    setup_Table();
    for (iter=temp_nodes.begin();iter!=temp_nodes.end();iter++)
    {

        int row_count = ui->tableWidget->rowCount(); //获取表单行数
        ui->tableWidget->insertRow(row_count);//插入新行
        int p=(*iter)->getSeq();
        QString str= QString::number(p, 10);
        qDebug()<<str;
        qDebug()<<(*iter)->getSeq()<<"temp:" << (*iter)->getTemp()<<endl;
        ui->tableWidget->setItem(row_count,0,new QTableWidgetItem(str));

        if((*iter)->checkState())
        {
            QTableWidgetItem *item = new QTableWidgetItem("yes");
            item->setBackgroundColor(QColor(0,0,255));//设为绿色
            ui->tableWidget->setItem(row_count,1,item);
        }
        else
        {
            QTableWidgetItem *item = new QTableWidgetItem("no");
            item->setBackgroundColor(QColor(255,0,0));//红色
            ui->tableWidget->setItem(row_count,1,item);
        }
        int t=(*iter)->getTemp();
        int h=(*iter)->getHum();
        int l=(*iter)->getLight();
        QString tt= QString::number(t, 10);
        QString hh= QString::number(h, 10);
        QString ll= QString::number(l, 10);
        ui->tableWidget->setItem(row_count,2,new QTableWidgetItem(tt));
        ui->tableWidget->setItem(row_count,3,new QTableWidgetItem(hh));
        ui->tableWidget->setItem(row_count,4,new QTableWidgetItem(ll));
        //首先处理包，获取温湿度信息
//        qDebug()<<"temp:" << (*iter)->getTemp()<<endl;
//        qDebug()<<"hum:" << (*iter)->getHum()<<endl;
//        qDebug()<<"lightx:" << (*iter)->getLight()<<endl;
    }

    for (iter2=motor_nodes.begin();iter2!=motor_nodes.end();iter2++)
    {
        int row_count = ui->tableWidget_2->rowCount(); //获取表单行数
        ui->tableWidget_2->insertRow(row_count);//插入新行
        int p=(*iter2)->getSeq();
        QString str= QString::number(p, 10);
        ui->tableWidget_2->setItem(row_count,0,new QTableWidgetItem(str));
        if((*iter2)->checkState())
        {
            QTableWidgetItem *item = new QTableWidgetItem("yes");
            item->setBackgroundColor(QColor(0,255,255));//设为绿色
            ui->tableWidget_2->setItem(row_count,1,item);
        }
        else
        {
            QTableWidgetItem *item = new QTableWidgetItem("no");
            item->setBackgroundColor(QColor(255,0,0));//红色
            ui->tableWidget_2->setItem(row_count,1,item);
        }
        int *Motor = (*iter2)->getMotor();
        if(Motor[0]==1)
        {
            QTableWidgetItem *item = new QTableWidgetItem("yes");
            ui->tableWidget_2->setItem(row_count,2,item);
        }
        else
        {
            QTableWidgetItem *item = new QTableWidgetItem("no");
            ui->tableWidget_2->setItem(row_count,2,item);
        }
    }



    for (iter2=motor_nodes.begin();iter2!=motor_nodes.end();iter2++)
    {
        int row_count = ui->tableWidget_3->rowCount(); //获取表单行数
        ui->tableWidget_3->insertRow(row_count);//插入新行
        int p=(*iter2)->getSeq();
        QString str= QString::number(p, 10);
        //qDebug()<<str;
        ui->tableWidget_3->setItem(row_count,0,new QTableWidgetItem(str));
        if((*iter2)->checkState())
        {
            QTableWidgetItem *item = new QTableWidgetItem("yes");
            item->setBackgroundColor(QColor(0,0,255));//设为绿色
            ui->tableWidget_3->setItem(row_count,1,item);
        }
        else
        {
            QTableWidgetItem *item = new QTableWidgetItem("no");
            item->setBackgroundColor(QColor(255,0,0));//红色
            ui->tableWidget_3->setItem(row_count,1,item);
        }
        int *Motor = (*iter2)->getMotor();
        if(Motor[0]==1)
        {
            QTableWidgetItem *item = new QTableWidgetItem("yes");
            ui->tableWidget_3->setItem(row_count,2,item);
        }
        else
        {
            QTableWidgetItem *item = new QTableWidgetItem("no");
            ui->tableWidget_3->setItem(row_count,2,item);
        }
    }

}

void MainWindow::setup_Table()
{
    ui->tableWidget->setColumnCount(5);
    QStringList header;
    header<<tr("节点编号")<<tr("是否在线")<<tr("温度")<<tr("湿度")<<tr("光照");
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();

    ui->tableWidget_2->setColumnCount(3);
    QStringList header1;
    header1<<tr("节点编号")<<tr("是否在线")<<tr("是否通风");
    ui->tableWidget_2->setHorizontalHeaderLabels(header1);
    ui->tableWidget_2->resizeRowsToContents();
    ui->tableWidget_2->resizeColumnsToContents();

    ui->tableWidget_3->setColumnCount(3);
    QStringList header2;
    header2<<tr("节点编号")<<tr("是否在线")<<tr("是否翻蛋");
    ui->tableWidget_3->setHorizontalHeaderLabels(header2);
    ui->tableWidget_3->resizeRowsToContents();
    ui->tableWidget_3->resizeColumnsToContents();


}






void MainWindow::setupRealtimelight(QwtPlot *qwtplot)
{
    //初始化xdata,x对应长度为5的坐标，y初始全为0
    for(int i=1;i<5001;i++)
    {
        x_light.append(double(i)/1000-5);
        y_light.append(0);
    }

    //light_Name = "实时变化曲线";
   // qwtplot->setTitle(light_Name);
    qwtplot->setCanvasBackground(Qt::white);//背景
    qwtplot->insertLegend(new QwtLegend(),QwtPlot::RightLegend);//标签

    curve = new QwtPlotCurve();
    //curve->setTitle("光照强度变化曲线");//曲线名字
    curve->setPen( Qt::yellow, 1 );//曲线的颜色 宽度;

    QTime curtime;
    curtime=curtime.currentTime();
    qwtplot->setAxisTitle(QwtPlot::xBottom, " time/s");
    qwtplot->setAxisTitle(QwtPlot::yLeft,"Light/lx");
    qwtplot->setAxisScale(QwtPlot::yLeft,0,300,10);
    qwtplot->setAxisScale(QwtPlot::xBottom,-5,0,1);



    QwtPlotZoomer *zoomer = new QwtPlotZoomer( qwtplot->canvas() );
    zoomer->setRubberBandPen( QColor( Qt::blue ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton );
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier( qwtplot->canvas() );                 //默认的滑轮及右键缩放功能  图形的整体缩放

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableX( true );//设置网格线
    grid->enableY( true );
    grid->setMajorPen( Qt::black, 0, Qt::DotLine );
    grid->attach(qwtplot);

//    connect(&updateTimer,SIGNAL(timeout()),this,SLOT(updatedataSlot_light()));
//    updateTimer.start(0);


}

//用于更新y_light,实际情况就是read数据
void MainWindow::updatedataSlot_light(int t){
    static QTime dataTime(QTime::currentTime());
    long int eltime = dataTime.elapsed();
    static int lastpointtime = 0;

    int size = (eltime - lastpointtime);


    if(size>0){//有数据传入
        y_light.erase(y_light.begin(),y_light.begin()+size);//擦除多余的数据
        for(int i=1;i<size+1;++i){
            y_light.append(t);
//            qDebug() << temp_humi_light->getLight();
        }
        lastpointtime = eltime;
    }

    curve->setSamples(x_light,y_light);
    curve->attach(ui->qwtPlot);
    ui->qwtPlot->replot();

    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    double fpstime = (double)eltime/1000.0-lastFpsKey;
    if ( fpstime> 2) // average fps over 2 seconds
    {
        ui->statusBar->showMessage(
                    QString("%1 FPS")
                    .arg(frameCount/fpstime, 0, 'f', 0)
                    , 0);
        lastFpsKey = (double)eltime/1000.0;
        frameCount = 0;
    }
}
void MainWindow::setupRealtimeHumidity(QwtPlot *qwtplot)
{
    //初始化_Humidity,x对应长度为5的坐标，y初始全为0
    for(int i=1;i<5001;i++)
    {
        x_Humidity.append(double(i)/1000-5);
        y_Humidity.append(0);
    }

   // Humidity_Name = "实时变化曲线";
   // qwtplot->setTitle(Humidity_Name);
    qwtplot->setCanvasBackground(Qt::white);//背景
    qwtplot->insertLegend(new QwtLegend(),QwtPlot::RightLegend);//标签

    curve1 = new QwtPlotCurve();
    //curve1->setTitle("湿度变化曲线");//曲线名字
    curve1->setPen( Qt::red, 1 );//曲线的颜色 宽度;

    QTime curtime;
    curtime=curtime.currentTime();
    qwtplot->setAxisTitle(QwtPlot::xBottom, " time/s");
    qwtplot->setAxisTitle(QwtPlot::yLeft,"Humidity/%");
    qwtplot->setAxisScale(QwtPlot::yLeft,0,100,5);
    qwtplot->setAxisScale(QwtPlot::xBottom,-5,0,1);



    QwtPlotZoomer *zoomer = new QwtPlotZoomer( qwtplot->canvas() );
    zoomer->setRubberBandPen( QColor( Qt::blue ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton );
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier( qwtplot->canvas() );                 //默认的滑轮及右键缩放功能  图形的整体缩放

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableX( true );//设置网格线
    grid->enableY( true );
    grid->setMajorPen( Qt::black, 0, Qt::DotLine );
    grid->attach(qwtplot);

//    connect(&updateTimer,SIGNAL(timeout()),this,SLOT(updatedataSlot()));
//    updateTimer.start(0);

}

//用于更新y_Humidity,实际情况就是read数据
void MainWindow::updatedataSlot_Humidity(int t)
{
    static QTime dataTime(QTime::currentTime());
    long int eltime = dataTime.elapsed();
    static int lastpointtime = 0;

    int size = (eltime - lastpointtime);


    if(size>0){//有数据传入
        y_Humidity.erase(y_Humidity.begin(),y_Humidity.begin()+size);//擦除多余的数据
        for(int i=1;i<size+1;++i){
            y_Humidity.append(t);
//            qDebug() << temp_humi_light->getHumidity();
        }
        lastpointtime = eltime;
    }

    curve1->setSamples(x_Humidity,y_Humidity);
    curve1->attach(ui->qwtPlot_2);
    ui->qwtPlot_2->replot();

}

void MainWindow::setupRealtimeTemperature(QwtPlot *qwtplot)
{
    //初始化x_Temperature,x对应长度为5的坐标，y初始全为0
    for(int i=1;i<5001;i++)
    {
        x_Temperature.append(double(i)/1000-5);
        y_Temperature.append(0);
    }

    //Temperature_Name = "实时变化曲线";
    //qwtplot->setTitle(Temperature_Name);
    qwtplot->setCanvasBackground(Qt::white);//背景
    qwtplot->insertLegend(new QwtLegend(),QwtPlot::RightLegend);//标签

    curve2 = new QwtPlotCurve();
    //curve2->setTitle("温度变化曲线");//曲线名字
    curve2->setPen( Qt::blue, 1 );//曲线的颜色 宽度;

    QTime curtime;
    curtime=curtime.currentTime();
    qwtplot->setAxisTitle(QwtPlot::xBottom, " time/s");
    qwtplot->setAxisTitle(QwtPlot::yLeft,"Temperature");
    qwtplot->setAxisScale(QwtPlot::yLeft,-10,50,2);
    qwtplot->setAxisScale(QwtPlot::xBottom,-5,0,1);



    QwtPlotZoomer *zoomer = new QwtPlotZoomer( qwtplot->canvas() );
    zoomer->setRubberBandPen( QColor( Qt::blue ) );
    zoomer->setTrackerPen( QColor( Qt::black ) );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2,Qt::RightButton, Qt::ControlModifier );
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3,Qt::RightButton );
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier( qwtplot->canvas() );                 //默认的滑轮及右键缩放功能  图形的整体缩放

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableX( true );//设置网格线
    grid->enableY( true );
    grid->setMajorPen( Qt::black, 0, Qt::DotLine );
    grid->attach(qwtplot);

//    connect(&updateTimer,SIGNAL(timeout()),this,SLOT(updatedataSlot()));
//    updateTimer.start(0);

}

//用于更新y_Temperature,实际情况就是read数据
void MainWindow::updatedataSlot_Temperature(int t){
    static QTime dataTime(QTime::currentTime());
    long int eltime = dataTime.elapsed();
    static int lastpointtime = 0;

    int size = (eltime - lastpointtime);

    if(size>0){//有数据传入
        y_Temperature.erase(y_Temperature.begin(),y_Temperature.begin()+size);//擦除多余的数据
        for(int i=1;i<size+1;++i){
            y_Temperature.append(t);
//            qDebug() << temp_humi_light->getTemperature();
        }
        lastpointtime = eltime;
    }

    curve2->setSamples(x_Temperature,y_Temperature);
    curve2->attach(ui->qwtPlot_3);
    ui->qwtPlot_3->replot();

}


void MainWindow::updateNum()//更新显示信息
{
    //qDebug()<<"yes/update";
    int aver_temper = 0,aver_hum = 0,aver_light = 0;
    int n = 0;
    for(iter = temp_nodes.begin();iter != temp_nodes.end();iter++)
    {
        aver_temper += (*iter)->getTemp();
        aver_hum += (*iter)->getHum();
        aver_light += (*iter)->getLight();
        n++;
    }
    if (n!=0)
    {
        aver_temper = aver_temper / n;
        aver_hum = aver_hum / n;
        aver_light = aver_light / n;
        ui->lcd_temper->display(aver_temper);//lcd模块显示温度
        ui->lcd_hum->display(aver_hum);
        ui->lcd_light->display(aver_light);
        updatedataSlot_Humidity(aver_temper);//更新湿度模块的图像
        updatedataSlot_light(aver_light);//更新光照模块的图像
        updatedataSlot_Temperature(aver_hum);//更新温度模块的图像

    }
}



/****/

void MainWindow::on_pushButton_7_clicked()
{
    if(ui->pushButton_7->text() == tr("控制翻蛋"))
    {
        UART_Format_Control UART_Control;
        UART_Control.Header= 0x40;
        UART_Control.Len=0x06;
        UART_Control.NodeSeq = 0x02;//翻蛋电机固定拷贝02
        UART_Control.NodeID = Motor;
        UART_Control.Command = 0x0a;
        UART_Control.Verify = 0x58;
        QByteArray st;
        st.resize(sizeof(UART_Control));
        memcpy(st.data(),&UART_Control,sizeof(UART_Format_Control));

        serial->write(st);//发送到串
        ui->pushButton_7->setText(tr("正在翻蛋"));
     }
    else
        {
        UART_Format_Control UART_Control;
        UART_Control.Header= 0x40;
        UART_Control.Len=0x06;
        UART_Control.NodeSeq = 0x02;
        UART_Control.NodeID = Motor;
        UART_Control.Command = 0x0c;
        UART_Control.Verify = 0x5a;
        QByteArray st;
        st.resize(sizeof(UART_Control));
        memcpy(st.data(),&UART_Control,sizeof(UART_Format_Control));
        serial->write(st);//发送到串

        ui->pushButton_7->setText(tr("控制翻蛋"));
    }
}
