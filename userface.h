#ifndef USERFACE_H
#define USERFACE_H
#include "QListWidgetItem"
#include <QWidget>
#include "chat.h"
#include "QTcpServer"
#include "QTcpSocket"
#include "datebaseutils.h"

namespace Ui {
class UserFace;
}

class UserFace : public QWidget
{
    Q_OBJECT

public:
    explicit UserFace(QWidget *parent = nullptr, UserInfo *userInfo =nullptr, QList<UserInfo> *userInfoList = nullptr);
    UserFace(UserInfo *userInfo, QList<UserInfo> *userInfoList);
    ~UserFace();

    UserInfo *userInfo;              //用户消息
    QList<UserInfo> *userInfoList;   //好友信息

private:
    Ui::UserFace *ui;

    Chat *chart;
    QTcpSocket *clientSocket;

    void startClient();         //启动客户端
    void initUser();            //初始化用户界面

public slots:
    void onConnected(); //处理连接成功
    void onItemDoubleClicked(QListWidgetItem *item);    //双击好友开始聊天
    void receiveMsg();  //接收消息
    void handleConnectionError(QAbstractSocket::SocketError error); //处理socket异常场景
};

#endif // USERFACE_H
