#include "form.h"
#include "ui_form.h"

form::form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::form)
{
    ui->setupUi(this);
    this->setWindowTitle("设置");
}

form::~form()
{
    delete ui;
}


void form::on_setButton_clicked()
{
    emit sendsignal(temper,hum);
    this->close();
}

void form::on_temperEdit_editingFinished()
{
    bool ok;
    temper = ui->temperEdit->text();
    if(temper.toLatin1().toInt(&ok) >41||temper.toLatin1().toInt(&ok) <34)//判断温度不再范围
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, tr("错误!"), MESSAGE1);
    }
}

void form::on_humEdit_editingFinished()
{
    bool ok;
    hum = ui->humEdit->text();
    if(hum.toLatin1().toInt(&ok) < 30||hum.toLatin1().toInt(&ok) > 70)//判断范围
    {
        QMessageBox::information(this,tr("错误!"),MESSAGE2);
    }
}
