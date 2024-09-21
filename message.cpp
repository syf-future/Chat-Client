#include "message.h"


template<typename T>
QString Message<T>::serializeMessage()
{
    QJsonObject obj;
    // 直接插入messageId和messageType
    obj.insert("messageId", this->messageId);
    obj.insert("messageType", this->messageType);
    // 使用QVariant来处理data字段
    QVariant var = QVariant::fromValue(this->date);
    QJsonValue dataValue = var.toJsonValue();

    obj.insert("data", dataValue);

    QJsonDocument doc(obj);
    return QString(doc.toJson(QJsonDocument::Compact));
}

template<typename T>
bool Message<T>::deserializeMessage(const QString &jsonString)
{
    QJsonDocument doc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (!doc.isObject()) {
        qDebug()<<"无效的json数据: " << jsonString;
        return false;
    }

    QJsonObject obj = doc.object();

    // 提取messageId
    if (obj.contains("messageId") && obj["messageId"].isString()) {
        messageId = obj["messageId"].toString();
    } else {
        return false;
    }

    // 提取messageType
    if (obj.contains("messageType") && obj["messageType"].isDouble()) {
        messageType = obj["messageType"].toInt();
    } else {
        return false;
    }

    // 提取data字段，这里假设T类型可以被QVariant转换
    if (obj.contains("data")) {
        QVariant var = obj["data"].toVariant();
        if (var.canConvert<T>()) {
            date = var.value<T>();
        } else {
            return false;
        }
    } else {
        return false;
    }

    return true;
}

//显式实例化
template QString Message<QString>::serializeMessage();
template bool Message<QString>::deserializeMessage(const QString &jsonString);

