#ifndef PROJET_H
#define PROJET_H

#include <QString>
#include <QSqlQuery> // Inclure QSqlQuery
#include <QSqlQueryModel>
#include <QDate>
#include <QtCharts> // Inclure QtCharts pour les graphiques
#include <QGeoCoordinate>
#include <QGeoPositionInfoSource>

class Projet
{
private:
    int id;
    QString nom;
    QString description;
    QString etat;
    QDate dateDebut;
    QDate dateFin;
    QString pdfPath;
    QString localisation;
    QGeoCoordinate m_localisation;
    double latitude;
    double longitude;


public:
    Projet();
    Projet(QString nom, QString description, QString etat,QString localisation, QDate dateDebut, QDate dateFin, QString pdfPath = "");

    // Getters et Setters
    int getId() const;
    QString getNom() const;
    QString getDescription() const;
    QString getEtat() const;
    QDate getDateDebut() const;
    QDate getDateFin() const;
    QString getPdfPath() const;
    QString getlocalisation() const;
    double getLatitude() const;
    double getLongitude() const;



    void setId(int id);
    void setNom(QString nom);
    void setDescription(QString description);
    void setEtat(QString etat);
    void setDateDebut(QDate dateDebut);
    void setDateFin(QDate dateFin);
    void setPdfPath(QString pdfPath);
    void setlocalisation(QString localisation);
    void setLatitude(double lat);
    void setLongitude(double lon);


    // Méthodes CRUD
    bool ajouter();
    bool modifier(int id);
    bool supprimer(int id);
    QSqlQueryModel* afficher(const QString& sortOrder) ;
    QSqlQueryModel* rechercherParNom(const QString& nom);


    int getTotalProjects() const;
    int getCompletedProjects() const;
    int getInProgressProjects() const;
    // Méthode pour afficher les statistiques sous forme de graphique
    QChartView* createStatisticsChart(QWidget* parent = nullptr) const;

};

#endif // PROJET_H
