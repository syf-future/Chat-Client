#ifndef REGISTER_H
#define REGISTER_H
#include "datebaseutils.h"
#include <QWidget>

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private:
    Ui::Register *ui;
    void showErrorInfo(QString errorInfo);

signals:
    void back(); // 发出 back 信号

private slots:
    void registerUser();
};

#endif // REGISTER_H
