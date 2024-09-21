#include "register.h"
#include "ui_register.h"
#include "QMovie"
#include "QDebug"
#include "QTimer"
void showInfoMessage(const QString& text);
Register::Register(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Register)
{
    ui->setupUi(this);
    this->setWindowTitle("QQ注册");
    //点击返回发送back信号
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Register::back);

    QPixmap pixmap1;
    pixmap1.load(":/resource/register.jpg");
    pixmap1 = pixmap1.scaled(ui->label_3->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // 设置 QLabel 的 pixmap
    ui->label_3->lower();
    ui->label_3->setPixmap(pixmap1);
    ui->label_4->setStyleSheet("QLabel { color: red; }");
    //绑定注册按钮的信号与槽
    connect(ui->pushButton, &QPushButton::clicked, this, &Register::registerUser);
}

Register::~Register()
{
    delete ui;
}

void Register::showErrorInfo(QString errorInfo)
{
    ui->label_4->setText(errorInfo);
    QTimer::singleShot(2000, &*ui->label_4,[=](){
        ui->label_4->clear();
    });
}

void Register::registerUser()
{
    QString userName = ui->lineEdit_8->text();
    QString account = ui->lineEdit_7->text();
    QString password = ui->lineEdit_6->text();
    if(userName == ""){
        this->showErrorInfo("请输入用户名");
        return;
    }
    if(account == ""){
        this->showErrorInfo("请输入账号");
        return;
    }
    if(password == ""){
        this->showErrorInfo("请输入密码");
        return;
    }
    if(ui->radioButton->isChecked()){
        QMap<QString, QString> map;
        map.insert("USER_NAME",userName);
        map.insert("ACCOUNT", account);
        QString sql = "SELECT * FROM user_info WHERE account='%1' OR userName='%2'";
        QList<UserInfo> userInfoList = QList<UserInfo>(UserInfo::convertToList(DateBaseUtils::build("chat","user_info")->selectByRecord(sql.arg(account).arg(userName))));
        if(userInfoList.size()==0){
            qDebug()<<"开始注册用户";
            map.insert("REQUEST_NO", account.append(userName));
            bool b = DateBaseUtils::build("chat","user_info")->insert(map);
            if(b){
                qDebug()<<"注册成功";
                QMetaObject::invokeMethod(ui->pushButton_2, "click");
            }
        }else{
            UserInfo userInfo = userInfoList[0];
            if(userInfo.userName == userName){
                this->showErrorInfo("用户名已存在");
            }else{
                this->showErrorInfo("账号已存在");
            }
        }
    }else{
        this->showErrorInfo("请阅读并勾选协议");
        return;
    }
}
