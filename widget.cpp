#include "widget.h"
#include "ui_widget.h"
#include "QDebug"
#include "QMessageBox"
#include "QTimer"
#include "QPixmap"
#include "QMovie"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    // 设置窗口为无边框
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setFixedSize(428,328);

    this->registerUi = new Register();      //实例化注册页面
    //接收注册页面发来的back信号 显示主页面
    connect(this->registerUi, &Register::back, this, [=](){
        this->registerUi->close();
        this->show();
    } );


    //设置默认头像
    const QString sheetStyle = "min-width: 55px; min-height: 55px;max-width:55px; max-height: 55px;border-radius: 27px;  border:1px solid white;background:white";
    ui->label_3->setStyleSheet(sheetStyle);
    // 设置图片
    QPixmap pixmap;
    pixmap.load(":/resource/head.jpg");
    // 将图片缩放到 40x40 像素
    QSize size(40, 40);
    pixmap = pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // 设置 QLabel 的 pixmap
    ui->label_3->setPixmap(pixmap);
    ui->label_3->setAlignment(Qt::AlignCenter);     //居中显示

    //显示背景动图
    QMovie *movie = new QMovie(":/resource/background.gif");
    ui->label_5->setMovie(movie);
    ui->label_5->lower();
    movie->start();

    //设置logo图片
    QPixmap pixmap1;
    pixmap1.load(":/resource/logo.png");
    pixmap1 = pixmap1.scaled(ui->label_4->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // 设置 QLabel 的 pixmap
    ui->label_4->setPixmap(pixmap1);
}

Widget::~Widget()
{
    delete ui;
    delete registerUi;
}

void showInfoMessage(const QString& text);

/**登录按钮
 * @brief Widget::on_pushButton_clicked
 */
void Widget::on_pushButton_clicked()
{
    qDebug()<<"登录";
    QLineEdit *accountText = ui->lineEdit1;
    QLineEdit *passwordText = ui->lineEdit2;

    QString account = accountText->text();
    QString password = passwordText->text();

    if(account.isEmpty()){
        qDebug()<<"请输入账号后再登录";
        showInfoMessage("请输入账号后再登录");
        return;
    }
    if(password.isEmpty()){
        qDebug()<<"请输入密码后再登录";
        showInfoMessage("请输入密码后再登录");
        return;
    }
    QList<UserInfo> *userInfoList = new QList<UserInfo>(UserInfo::convertToList(DateBaseUtils::build("chat","user_info")->selectByRecord()));
    if(userInfoList->size()==0){
        showInfoMessage("当前无用户，请注册");
        return;
    }else{
        UserInfo *userInfo = new UserInfo();
        for (int var = 0; var < userInfoList->size(); ++var) {
            if (userInfoList->at(var).account == account && userInfoList->at(var).password == password) {
                *userInfo = userInfoList->at(var);
                userInfoList->remove(var);
            }
        }
        if(!userInfo->account.isEmpty()){
            this->userFace = new UserFace(nullptr,userInfo, userInfoList);
            this->hide();   //隐藏当前界面
            this->userFace->show();
        }else{
            showInfoMessage("用户名或密码错误");
        }
    }
}

/**
 * 点击注册按钮
 * @brief Widget::on_pushButton_2_clicked
 */
void Widget::on_pushButton_2_clicked()
{
    //绑定注册按钮   切换到注册页面
    this->hide();
    this->registerUi->show();

}


/**
 * 信息提示栏
 * @brief showInfoMessage
 * @param text
 */
void showInfoMessage(const QString& text)
{
    // 创建一个无边框和标题栏的对话框
    QDialog *dialog = new QDialog;
    dialog->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    // dialog->setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明

    // 创建一个标签来显示提示信息
    QLabel* label = new QLabel(text, &*dialog);
    label->setAlignment(Qt::AlignCenter); // 文本居中显示
    label->setStyleSheet("QLabel { font-size: 14px; color: black; }"); // 自定义样式

    // 设置对话框的大小
    dialog->resize(label->sizeHint());

    // 启动定时器，在2秒后自动关闭对话框
    QTimer::singleShot(1000, &*dialog,[dialog](){
        dialog->close();
        delete dialog;
    });

    // 显示对话框（非模态）
    dialog->show();
}

/**
 * 关闭主界面
 * @brief Widget::on_pushButton_4_clicked
 */
void Widget::on_pushButton_4_clicked()
{
    this->close();
}

