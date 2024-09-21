#ifndef CHAT_H
#define CHAT_H
#include "message.h"
#include <QWidget>
#include "QTcpSocket"
#include "datebaseutils.h"
namespace Ui {
class Chat;
}

class Chat : public QWidget
{
    Q_OBJECT

public:
    explicit Chat(QWidget *parent = nullptr, QTcpSocket *clientSocket = nullptr,UserInfo *myUserInfo = nullptr, UserInfo *userInfo = nullptr);
    ~Chat();

    enum MessageType{
        REGISTER = 0,       //注册
        ADD = 1,            //添加
        CHAT = 2            //聊天
    };
    UserInfo *myUserInfo;
    UserInfo *userInfo;
    QString *chatMessage;

private:
    Ui::Chat *ui;

    QTcpSocket *clientSocket;

public slots:
    void sendMsg();     //发送信息
    void receiveMsg();  //处理消息
    void closeChat();   //关闭聊天
};

#endif // CHAT_H
