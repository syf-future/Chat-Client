#include "chat.h"
#include "ui_chat.h"
#include "QDebug"
#include "QTextCursor"
Chat::Chat(QWidget *parent, QTcpSocket *clientSocket, UserInfo *myUserInfo, UserInfo *userInfo)
    : QWidget(parent)
    , ui(new Ui::Chat)
{
    ui->setupUi(this);
    this->myUserInfo = myUserInfo;
    this->userInfo = userInfo;
    this->clientSocket = clientSocket;

    ui->label->setText(this->userInfo->userName);
    ui->label->setStyleSheet("QLabel { font-size: 24px; background-color: lightblue;}"); // 设置字体大小为24像素
    ui->label->setAlignment(Qt::AlignCenter);       // 设置文字居中对齐
    //设置按钮样式
    ui->pushButton_2->setStyleSheet(
        "QPushButton {"
        "   background-color: rgb(100, 175, 255);"
        "   color: white;"
        "}");
    //聊天框设置只读
    ui->textEdit_2->setReadOnly(true);

    //设置logo图片
    QPixmap pixmap1;
    pixmap1.load(":/resource/chat.png");
    pixmap1 = pixmap1.scaled(ui->label_2->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    // 设置 QLabel 的 pixmap
    ui->label_2->setPixmap(pixmap1);

    //绑定发送按钮信号和槽
    connect(ui->pushButton_2,&QPushButton::clicked,this, &Chat::sendMsg);
    //取消按钮
    connect(ui->pushButton,&QPushButton::clicked,this, &Chat::closeChat);
}

Chat::~Chat()
{
    delete ui;
    delete this->chatMessage;
}

/**
 * 发送消息
 * @brief Chat::sendMsg
 */
void Chat::sendMsg()
{
    QString inputText = ui->textEdit->toPlainText();
    if(inputText != ""){
        qDebug()<< "发送消息: " << inputText;
        ui->textEdit_2->append(QString("<div style=\"margin-bottom:10px;font-size:14px;\"><font color='blue'>%1</font>：我</div>").arg(inputText));
        ui->textEdit_2->setAlignment(Qt::AlignRight);
        if(this->clientSocket && this->clientSocket->state() ==  QTcpSocket::ConnectedState){
            qDebug()<<"连接成功开始发信息";
            Message<QString> message;
            message.messageId = this->userInfo->requestNo;
            message.messageType = MessageType::CHAT;
            message.date = inputText;
            QByteArray msg = message.serializeMessage().toUtf8();
            qDebug() << "聊天信息:"<< msg;
            this->clientSocket->write(msg);
            this->clientSocket->flush();
            qDebug() << "聊天信息发送成功";
        }else{
            qDebug() << "未连接到服务器";
        }
    }
}

/**
 * 处理消息
 * @brief Chat::receiveMsg
 */
void Chat::receiveMsg()
{
    QString inputText = *this->chatMessage;
    if(inputText != ""){
        qDebug()<< "接收: " << inputText;
        ui->textEdit_2->append(QString("<div style=\"margin-bottom:10px;font-size:14px;\"><font color='green'>%2：</font> %1</div>").arg(inputText).arg(this->userInfo->userName));
        ui->textEdit_2->setAlignment(Qt::AlignLeft);
        delete this->chatMessage;
    }
}

void Chat::closeChat()
{
    this->close();
}
