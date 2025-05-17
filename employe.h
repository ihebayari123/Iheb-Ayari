#ifndef EMPLOYE_H
#define EMPLOYE_H
#include <QString>
#include <QSqlQuery>

#include <QWidget>
class Employe
{
private:
    QString nom;
    QString prenom;
    QString email;
    QString motdepasse;
    QString role;
    QByteArray photoData; // Attribut pour stocker la photo (BLOB)
    QByteArray audioData; // Nouvel attribut pour stocker l'audio (BLOB)

public:
    Employe(QString nom, QString prenom, QString email, QString motdepasse, QString role);
    Employe();

    QString getNom() const;
    void setNom(const QString &value);

    QString getPrenom() const;
    void setPrenom(const QString &value);

    QString getEmail() const;
    void setEmail(const QString &value);

    QString getMotdepasse() const;
    void setMotdepasse(const QString &value);

    QString getRole() const;
    void setRole(const QString &value);

    QByteArray getPhotoData() const;
    void setPhotoData(const QByteArray &value);

    QByteArray getAudioData() const;    // Getter pour l'audio
    void setAudioData(const QByteArray &value);   // Setter pour l'audio

    bool createEmploye();
    QSqlQuery afficher();
    bool updateEmploye(int id);
    bool deleteEmploye(int id);

    // Méthode pour assigner la photo capturée
    void setPhoto(const QByteArray &data);
};

#endif // EMPLOYE_H
