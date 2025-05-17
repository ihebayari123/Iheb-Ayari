#include "employe.h"
#include <QSqlQuery>
#include <QVariant>
#include <QSqlError>
#include <QDebug>

Employe::Employe(QString nom, QString prenom, QString email, QString motdepasse, QString role)
    : nom(nom), prenom(prenom), email(email), motdepasse(motdepasse), role(role) {}

Employe::Employe() {}

// Getters et setters
QString Employe::getNom() const { return nom; }
void Employe::setNom(const QString &value) { nom = value; }

QString Employe::getPrenom() const { return prenom; }
void Employe::setPrenom(const QString &value) { prenom = value; }

QString Employe::getEmail() const { return email; }
void Employe::setEmail(const QString &value) { email = value; }

QString Employe::getMotdepasse() const { return motdepasse; }
void Employe::setMotdepasse(const QString &value) { motdepasse = value; }

QString Employe::getRole() const { return role; }
void Employe::setRole(const QString &value) { role = value; }

QByteArray Employe::getPhotoData() const {
    return photoData;
}

void Employe::setPhotoData(const QByteArray &value) {
    photoData = value;
}

QByteArray Employe::getAudioData() const {
    return audioData;
}

void Employe::setAudioData(const QByteArray &value) {
    audioData = value;
}

// Définition de setPhoto (à utiliser dans MainWindow)
void Employe::setPhoto(const QByteArray &data)
{
    photoData = data;
}

bool Employe::createEmploye() {
    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || motdepasse.isEmpty() || role.isEmpty()) {
        qDebug() << "Erreur : Un ou plusieurs champs sont vides.";
        return false;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM employes WHERE EMAIL = ?");
    checkQuery.addBindValue(email);
    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "Erreur lors de la vérification de l'email :" << checkQuery.lastError().text();
        return false;
    }
    if (checkQuery.value(0).toInt() > 0) {
        qDebug() << "Erreur : Un employé avec cet email existe déjà.";
        return false;
    }

    QSqlQuery query;
    // Assurez-vous que votre table employes possède bien les colonnes IMAGE (pour la photo) et AUDIO (pour l'audio)
    query.prepare("INSERT INTO EMPLOYES (NOM, PRENOM, EMAIL, MOTDEPASS, ROLE, IMAGE, AUDIO) VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(nom);
    query.addBindValue(prenom);
    query.addBindValue(email);
    query.addBindValue(motdepasse);
    query.addBindValue(role);
    query.addBindValue(photoData);
    query.addBindValue(audioData);
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout de l'employé :" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQuery Employe::afficher()
{
    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, EMAIL, MOTDEPASS, ROLE FROM EMPLOYES");
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête d'affichage :" << query.lastError().text();
    }
    return query;
}

bool Employe::updateEmploye(int id) {
    QSqlQuery query;
    query.prepare("UPDATE employes SET NOM = ?, PRENOM = ?, EMAIL = ?, MOTDEPASS = ?, ROLE = ?, IMAGE = ?, AUDIO = ? WHERE ID = ?");
    query.addBindValue(nom);
    query.addBindValue(prenom);
    query.addBindValue(email);
    query.addBindValue(motdepasse);
    query.addBindValue(role);
    query.addBindValue(photoData);
    query.addBindValue(audioData);
    query.addBindValue(id);
    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification de l'employé :" << query.lastError().text();
        return false;
    }
    return true;
}

bool Employe::deleteEmploye(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM employes WHERE ID = ?");
    query.addBindValue(id);
    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression de l'employé :" << query.lastError().text();
        return false;
    }
    return true;
}
