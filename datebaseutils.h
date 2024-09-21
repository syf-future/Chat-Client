#ifndef DATEBASEUTILS_H
#define DATEBASEUTILS_H
#include "QSqlDatabase"
#include "QSqlError"
#include "QDebug"
#include "QMap"
#include "QSqlQuery"
#include "QList"
#include "QSqlRecord"

struct UserInfo {
public:
    QString requestNo;
    QString userName;
    QString account;
    QString password;

    static QList<UserInfo> convertToList(const QList<QMap<QString, QVariant>>& sourceList)
    {
        QList<UserInfo> resultList;
        for (const auto& row : sourceList) {
            UserInfo userInfo;
            userInfo.requestNo = row.value("REQUEST_NO").toString();
            userInfo.userName = row.value("USER_NAME").toString();
            userInfo.account = row.value("ACCOUNT").toString();
            userInfo.password = row.value("PASSWORD").toString();
            resultList.append(userInfo);
        }
        return resultList;
    }
};

class DateBaseUtils
{
public:
    DateBaseUtils();

    static DateBaseUtils* build(QString libraryName, QString tableName);

    QList<QMap<QString, QVariant>> selectByRecord();
    QList<QMap<QString, QVariant>> selectByRecord(QMap<QString, QString> map);
    QList<QMap<QString, QVariant>> selectByRecord(const QString sql);

    bool insert(QMap<QString, QString> map);

    void close();

private:
    QSqlDatabase *db;
    QString *tableName;
};
#endif // DATEBASEUTILS_H
