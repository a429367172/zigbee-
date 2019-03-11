#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QMessageBox>
namespace Ui {
class form;
}
#define MESSAGE1 "请在三十四到四十一摄氏度范围内输入"
#define MESSAGE2 "请在三十到七十相对湿度范围内输入"
class form : public QWidget
{
    Q_OBJECT

public:
    explicit form(QWidget *parent = 0);
    ~form();
private:
    Ui::form *ui;
    QString temper;
    QString hum;
signals:
    void sendsignal(QString,QString);
private slots:
    void on_setButton_clicked();
    void on_temperEdit_editingFinished();
    void on_humEdit_editingFinished();
};

#endif // FORM_H
