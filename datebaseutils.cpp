#include "datebaseutils.h"
DateBaseUtils::DateBaseUtils() {}

DateBaseUtils* DateBaseUtils::build(QString libraryName, QString tableName)
{
    QSqlDatabase *db = new QSqlDatabase();
    *db = QSqlDatabase::addDatabase("QMYSQL"); //添加Mysql数据库

    db->setHostName("127.0.0.1"); // 你的MySQL服务器地址
    db->setPort(3306); // 端口号
    db->setDatabaseName(libraryName); // 你的数据库名
    db->setUserName("root"); // 你的数据库用户名
    db->setPassword("1234"); // 你的数据库密码
    bool ok = db->open();
    if(!ok){
        QString err = db->lastError().text();
        qDebug()<<"连接数据库失败: "<<err;
        throw std::runtime_error("连接数据库失败: " + err.toStdString());
    }
    else {
        qDebug()<<"连接数据库成功";
        DateBaseUtils *dateBaseUtils = new DateBaseUtils;
        dateBaseUtils->db = db;
        dateBaseUtils->tableName = &tableName;
        return dateBaseUtils;
    }
}

QList<QMap<QString, QVariant>> DateBaseUtils::selectByRecord(){
    QMap<QString, QString> map;
    return this->selectByRecord(map);
}

QList<QMap<QString, QVariant>> DateBaseUtils::selectByRecord(QMap<QString, QString> map)
{
    QList<QMap<QString, QVariant>> queryResults;
    try {
        QString querySql = "SELECT * FROM " + *this->tableName;
        QString condition = "";
        QList<QString> keys = map.keys();
        if(keys.size()>0){
            condition = " WHERE ";
        }
        for (int var = 0; var < keys.size(); ++var) {
            QString key = keys.at(var);
            QString value = map.value(key);
            condition.append(key).append("='").append(value).append("'");
            if(var < keys.size()-1){
                condition.append(" AND ");
            }
        }
        querySql.append(condition);
        qDebug()<< "查询sql：" << querySql;
        QSqlQuery query(*this->db);
        query.exec(querySql);
        if(query.isActive()){
            while (query.next()) {
                QMap<QString, QVariant> row;
                // 获取列的数量
                int columnCount = query.record().count();
                for (int i = 0; i < columnCount; ++i) {
                    QString fieldName = query.record().fieldName(i); // 获取字段名称
                    QVariant fieldValue = query.value(i); // 获取字段的值
                    row.insert(fieldName, fieldValue);
                }
                queryResults.append(row);
            }
        }else{
            qDebug() << "查询异常: " << query.lastError().text();
        }
        return queryResults;
        close();
    } catch (...) {
        close();
    }
    return queryResults;

}

QList<QMap<QString, QVariant> > DateBaseUtils::selectByRecord(const QString sql)
{
    QList<QMap<QString, QVariant>> queryResults;
    try {
        qDebug()<< "查询sql：" << sql;
        QSqlQuery query(*this->db);
        query.exec(sql);
        if(query.isActive()){
            while (query.next()) {
                QMap<QString, QVariant> row;
                // 获取列的数量
                int columnCount = query.record().count();
                for (int i = 0; i < columnCount; ++i) {
                    QString fieldName = query.record().fieldName(i); // 获取字段名称
                    QVariant fieldValue = query.value(i); // 获取字段的值
                    row.insert(fieldName, fieldValue);
                }
                queryResults.append(row);
            }
        }else{
            qDebug() << "查询异常: " << query.lastError().text();
        }
        return queryResults;
        close();
    } catch (...) {
        close();
    }
    return queryResults;
}

bool DateBaseUtils::insert(QMap<QString, QString> map)
{
    try {
        QList<QString> keys = map.keys();
        QString keyList = "(";
        QString valueList = "(";
        for (int var = 0; var < keys.size(); ++var) {
            QString key = keys.at(var);
            QString value = map.value(key);
            if(var < keys.size()-1){
                keyList.append(key).append(",");
                valueList.append("'").append(value).append("'").append(",");
            }else{
                keyList.append(key).append(")");
                valueList.append("'").append(value).append("'").append(")");
            }
        }
        QString insterSql = "INSERT INTO " + *this->tableName + " " + keyList + " VALUES " + valueList;
        qDebug()<< "新增sql：" << insterSql;
        QSqlQuery query(*this->db);
        bool *b = new bool(query.exec(insterSql)) ;
        close();
        return b;
    } catch (...) {
        close();
    }
    return false;
}

void DateBaseUtils::close()
{
    if(this->db != NULL){
        delete this->db;
    }
    delete this->tableName;
    delete this;
}


