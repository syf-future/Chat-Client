#ifndef WIDGET_H
#define WIDGET_H
#include "register.h"
#include "userface.h"
#include <QWidget>
#include "datebaseutils.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::Widget *ui;
    Register *registerUi;
    UserFace *userFace;
};
#endif // WIDGET_H
