#include <QApplication>
#include <QMessageBox>
#include "mainwindow.h"
#include "login.h"
#include "connection.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Connection c;
    if (!c.createconnect()) {
        QMessageBox::critical(nullptr, QObject::tr("Database Connection"),
                              QObject::tr("Connection failed.\nClick OK to exit."),
                              QMessageBox::Ok);
        return -1;
    }

    Login loginDialog;
    if (loginDialog.exec() == QDialog::Accepted) {
        QString userRole = loginDialog.getUserRole(); // Récupère le rôle de l'utilisateur
        QString userName = loginDialog.getUserName();
        QString userEmail = loginDialog.getUserEmail();
       QPixmap userPhoto = loginDialog.getUserPhoto();
         MainWindow mainWindow(userRole, userName, userEmail, userPhoto);
        mainWindow.show();
        return app.exec();
    }

    return 0;
}
