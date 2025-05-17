#ifndef DATABASE_CONNECTION_H
#define DATABASE_CONNECTION_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QDebug>

class DatabaseConnection {
public:
    DatabaseConnection();
    ~DatabaseConnection();

    bool connectToDatabase(const QString &dbName);
    bool validateUser(const QString &username, const QString &password);

private:
    QSqlDatabase db;
};

#endif // DATABASE_CONNECTION_H
