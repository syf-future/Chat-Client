#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

template<typename T>
struct Message
{
    QString messageId;
    int messageType;
    T date;


    /**
     * 序列化
     * @brief serializeMessage
     * @param msg
     * @return
     */
    QString serializeMessage();


    /**
     * 反序列化
     * @brief deserializeMessage
     * @param jsonString
     * @return
     */
    bool deserializeMessage(const QString& jsonString);
};
#endif // MAINWINDOW_H
