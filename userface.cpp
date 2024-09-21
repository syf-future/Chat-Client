#include "userface.h"
#include "ui_userface.h"
#include "QVariant";
#define HOSTNAME "127.0.0.1"
#define PORT 8888

UserFace::UserFace(QWidget *parent, UserInfo *userInfo, QList<UserInfo> *userInfoList)
    : QWidget(parent)
    , ui(new Ui::UserFace)
{
    ui->setupUi(this);
    this->userInfo = userInfo;
    this->userInfoList = userInfoList;
    //启动客户端
    startClient();
}


UserFace::~UserFace()
{
    delete ui;
    delete this->userInfo;
    delete this->userInfoList;
}


/**
 * 启动客户端并于服务端建立连接
 * @brief UserFace::startClient
 */
void UserFace::startClient()
{
    qDebug()<<"开始启动客户端";
    this->clientSocket = new QTcpSocket();
    // 设置服务器地址和端口
    this->clientSocket->connectToHost(HOSTNAME, PORT);
    //连接成功信号
    connect(clientSocket, &QTcpSocket::connected, this, &UserFace::onConnected);
    //接收数据信号
    connect(clientSocket, &QTcpSocket::readyRead, this, &UserFace::receiveMsg);
    // 处理连接错误信号
    connect(clientSocket, &QTcpSocket::errorOccurred, this, &UserFace::handleConnectionError);
    //初始化用户界面
    this->initUser();
}

void UserFace::initUser()
{
    QList<QPair<QString, QString>> friends;
    qDebug()<<"共有好友："<<this->userInfoList->size();
    for (int var = 0; var < this->userInfoList->size(); ++var) {
        friends.append(QPair<QString, QString>(userInfoList->at(var).userName, ":/resource/head.jpg"));
    }
    // 设置图标大小
    ui->listWidget->setIconSize(QSize(60, 60));
    qDebug()<<"1";
    for (const auto &friendInfo : friends) {
        // 创建QListWidgetItem并设置图标
        QListWidgetItem *item = new QListWidgetItem(QIcon(friendInfo.second), friendInfo.first);
        item->setSizeHint(QSize(303, 70)); // 设置项的大小提示
        // 添加项到列表
        ui->listWidget->addItem(item);
    }
    // 设置QListWidget样式表以调整文本大小
    ui->listWidget->setStyleSheet("QListWidget { font-size: 16pt; }");
    // 设置不显示滚动条（如果需要）
    ui->listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //设置logo图片
    ui->label->setText(this->userInfo->userName);
    ui->label->setStyleSheet("QLabel { font-size: 24px; background-color: lightblue;}"); // 设置字体大小为24像素
    ui->label->setAlignment(Qt::AlignCenter);       // 设置文字居中对齐

    //设置logo图片
    QPixmap pixmap12;
    pixmap12.load(":/resource/chat2.png");
    pixmap12 = pixmap12.scaled(ui->label_2->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    // 设置 QLabel 的 pixmap
    ui->label_2->setPixmap(pixmap12);

    //双击好友开始聊天
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &UserFace::onItemDoubleClicked);
}

/**
 * 处理连接成功事件
 * @brief UserFace::onConnected
 */
void UserFace::onConnected()
{
    qDebug()<<"连接服务端成功,开始发送注册消息";
    if(this->clientSocket->state() ==  QTcpSocket::ConnectedState){
        Message<QString> message;
        message.messageId = this->userInfo->requestNo;
        message.messageType = Chat::MessageType::REGISTER;
        message.date = "hello";
        this->clientSocket->write(message.serializeMessage().toUtf8());
        this->clientSocket->flush();
        qDebug() << "注册信息发送成功";
    }else{
        qDebug() << "未连接到服务器";
    }
}

/**
 * 双击好友开始聊天
 * @brief UserFace::onItemDoubleClicked
 * @param item
 */
void UserFace::onItemDoubleClicked(QListWidgetItem *item)
{
    // 获取双击的项的名字
    QString name = item->text();
    UserInfo *userInfo1 = new UserInfo();
    for (int var = 0; var < this->userInfoList->size(); ++var) {
        if(this->userInfoList->at(var).userName == name){
            *userInfo1 = this->userInfoList->at(var);
        }
    }
    this->chart = new Chat(nullptr, this->clientSocket, this->userInfo, userInfo1);
    // 打印名字
    qDebug() << "双击的项的名字：" << name;
    this->chart->show();
}

/**
 * 接收消息
 * @brief UserFace::receiveMsg
 */
void UserFace::receiveMsg()
{
    QByteArray data = this->clientSocket->readAll();
    qDebug() << "接收到服务端消息:" << data;

    Message<QString> message;
    message.deserializeMessage(data.data());
    switch (message.messageType) {
    case Chat::MessageType::ADD:
        qDebug()<<"开始处理加好友事件";
        break;
    case Chat::MessageType::CHAT:
        qDebug()<<"开始处理聊天事件  聊天消息："<<message.date;
        if(this->chart != nullptr){
            qDebug()<< "聊天框已开启" ;
            QString *chatMessage = new QString( message.date);
            this->chart->chatMessage = chatMessage;
            //触发chat的消息处理
            this->chart->receiveMsg();
        }else{
            qDebug()<< "聊天框未开启" ;
        }
        break;
    default:
        qDebug()<<"错误的消息类型";
        break;
    }
}


/**
 * 处理socket异常
 * @brief UserFace::handleConnectionError
 * @param error
 */
void UserFace::handleConnectionError(QAbstractSocket::SocketError error)
{
    switch (error) {
    case QAbstractSocket::RemoteHostClosedError:
        qDebug() << "远程主机关闭了连接";
        break;
    case QAbstractSocket::HostNotFoundError:
        qDebug() << "找不到主机：" << clientSocket->errorString();
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug() << "连接被拒绝：" << clientSocket->errorString();
        break;
    case QAbstractSocket::SocketAccessError:
        qDebug() << "套接字访问错误：" << clientSocket->errorString();
        break;
    case QAbstractSocket::SocketResourceError:
        qDebug() << "套接字资源错误：" << clientSocket->errorString();
        break;
    case QAbstractSocket::SocketTimeoutError:
        qDebug() << "套接字超时：" << clientSocket->errorString();
        break;
    case QAbstractSocket::DatagramTooLargeError:
        qDebug() << "数据报太大：" << clientSocket->errorString();
        break;
    case QAbstractSocket::NetworkError:
        qDebug() << "网络错误：" << clientSocket->errorString();
        break;
    case QAbstractSocket::AddressInUseError:
        qDebug() << "地址已被使用：" << clientSocket->errorString();
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        qDebug() << "地址不可用：" << clientSocket->errorString();
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        qDebug() << "不支持的套接字操作：" << clientSocket->errorString();
        break;
    case QAbstractSocket::UnfinishedSocketOperationError:
        qDebug() << "未完成的套接字操作：" << clientSocket->errorString();
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        qDebug() << "代理认证所需：" << clientSocket->errorString();
        break;
    case QAbstractSocket::SslHandshakeFailedError:
        qDebug() << "SSL握手失败：" << clientSocket->errorString();
        break;
    case QAbstractSocket::UnknownSocketError:
    default:
        qDebug() << "未知错误：" << clientSocket->errorString();
        break;
    }
}
