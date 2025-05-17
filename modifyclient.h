#ifndef MODIFYCLIENT_H
#define MODIFYCLIENT_H
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class modifyclient : public QDialog
{
    Q_OBJECT
public:

    explicit modifyclient(QWidget *parent = nullptr);
    void setData(const QString &nom, const QString &prenom, const QString &adresse, const QString &email, int tel);
    QString getNom() const;
    QString getPrenom() const;
    int getTel() const;
    QString getAdresse() const;
    QString getEmail() const;

private:
    QLineEdit *lineEditNom;
    QLineEdit *lineEditPrenom;
    QLineEdit *lineEditEmail;
    QLineEdit *lineEditAdresse;
    QLineEdit *lineEditTel;
    QPushButton *btnConfirmer;
    QPushButton *btnDecliner;

};

#endif // MODIFYCLIENT_H
