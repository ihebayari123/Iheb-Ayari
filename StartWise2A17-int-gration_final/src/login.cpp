#include "login.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

Login::Login(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    connect(loginButton, &QPushButton::clicked, this, &Login::handleLogin);
}

void Login::handleLogin() {
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();

    if (validateCredentials(username, password)) {
        accept(); // Close the login dialog and return success
    } else {
        QMessageBox::warning(this, "Login Failed", "Invalid username or password.");
    }
}

bool Login::validateCredentials(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT * FROM employees WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);
    
    if (!query.exec()) {
        qDebug() << "Database query error:" << query.lastError().text();
        return false;
    }

    return query.next(); // Returns true if a record is found
}