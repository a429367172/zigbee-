#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QQueue>
#include <QDateTime>
#include <QTimer>
#include <QSlider>
//cck
#include <QVector>
#include <tempandhum.h>
#include <motor.h>
#include <form.h>
/***/
#include <QTableWidget>
#include "qwt_plot_zoomer.h"
#include "qwt_plot_curve.h"
#include "qwt_plot.h"
#include "qwt_legend.h"
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_grid.h>
#include <qwt_scale_draw.h>

namespace Ui {
class MainWindow;
class QDebug;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void ReadData();

    void on_OpenSerialButton_clicked();

    void on_searchButton_clicked();
    void processBufQue();
    int  CheckSum(QByteArray packg,int len);
    void sendPwm(int);
    void on_pushButton_clicked();

    void on_motorButton_clicked();

    void on_pushButton_2_clicked();

    /*cck*/
    void reshow(QString,QString);
    void on_setButton_clicked();
   // void timeUpdate();
    void updateNum();
    void update_DataofNode();
    void debug();
    void on_pushButton_7_clicked();

private:
    QVector<double> x_light;
    QVector<double> y_light;
    QVector<double> x_Humidity;
    QVector<double> y_Humidity;
    QVector<double> x_Temperature;
    QVector<double> y_Temperature;
    Ui::MainWindow *ui;
    QString number;
    QSerialPort *serial;
    QByteArray bufQue;
    QTimer processBufQueTimer;

    /*cck*/
    form *f1;
    QVector<tempandhum*> temp_nodes;//温湿度节点
    QVector<tempandhum*>::iterator iter;
    QVector<motor*> motor_nodes;//电机节点
    QVector<motor*>::iterator iter2;
    QString temper_set,hum_set;
    QDateTime startTime;
    /*cck*/
    QwtPlotCurve *curve ;
    QwtPlotCurve *curve1 ;
    QwtPlotCurve *curve2 ;

    QSlider *Slider;
    void sendMessage(int temper,int hum );
    /******************************************************************************/

    QTimer updateTimer;
    //QString demoName;
    void Packet_checkExit(QByteArray packet,int NodeSeq);
    void Packet_showInfo(QByteArray packet,int type);
    void Process_Packet(QByteArray packet);
    /***/

    void setup_Table();
    void setupRealtimelight(QwtPlot *qwtplot);
    void setupRealtimeHumidity(QwtPlot *qwtplot);
    void setupRealtimeTemperature(QwtPlot *qwtplot);
    void updatedataSlot_light(int t);
    void updatedataSlot_Humidity(int t );
    void updatedataSlot_Temperature(int t );

    /***/
    bool ok;
};

#endif // MAINWINDOW_H
