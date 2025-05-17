#include "projet.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>
#include <QString>
#include <QSqlError>
#include <QDebug>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

// Constructeurs
Projet::Projet() {}

Projet::Projet(QString nom, QString description, QString etat,QString localisation, QDate dateDebut, QDate dateFin, QString pdfPath)
    : nom(nom), description(description), etat(etat), dateDebut(dateDebut), dateFin(dateFin), pdfPath(pdfPath) , localisation(localisation) {}

// Getters
int Projet::getId() const { return id; }
QString Projet::getNom() const { return nom; }
QString Projet::getDescription() const { return description; }
QString Projet::getEtat() const { return etat; }
QDate Projet::getDateDebut() const { return dateDebut; }
QDate Projet::getDateFin() const { return dateFin; }
QString Projet::getPdfPath() const { return pdfPath; }
QString Projet::getlocalisation() const { return localisation; }

// Setters
void Projet::setId(int id) { this->id = id; }
void Projet::setNom(QString nom) { this->nom = nom; }
void Projet::setDescription(QString description) { this->description = description; }
void Projet::setEtat(QString etat) { this->etat = etat; }
void Projet::setDateDebut(QDate dateDebut) { this->dateDebut = dateDebut; }
void Projet::setDateFin(QDate dateFin) { this->dateFin = dateFin; }
void Projet::setPdfPath(QString pdfPath) { this->pdfPath = pdfPath; }
void Projet::setlocalisation(QString localisation) { this->localisation = localisation; }


// Ajouter un projet
bool Projet::ajouter() {
    QSqlQuery query;

    query.prepare("INSERT INTO PROJETS (nom, description,localisation, etat, date_debut, date_fin, pdf_path) "
                  "VALUES (:nom, :description, :localisation, :etat, :date_debut, :date_fin, :pdf_path)");
    query.bindValue(":nom", nom);
    query.bindValue(":localisation", localisation);
    query.bindValue(":description", description);
    query.bindValue(":etat", etat);
    query.bindValue(":date_debut", dateDebut);
    query.bindValue(":date_fin", dateFin);
    query.bindValue(":pdf_path", pdfPath);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'ajout du projet : " << query.lastError().text();
        return false;
    }
    return true;
}

// Supprimer un projet
bool Projet::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM PROJETS WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la suppression du projet : " << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* Projet::afficher(const QString& sortOrder) {
    QSqlQueryModel* model = new QSqlQueryModel();

    // Single, consistent query definition
    QString queryStr = "SELECT id, nom, description, localisation, date_debut, date_fin, etat, pdf_path FROM PROJETS";

    // Gestion du tri
    if (!sortOrder.isEmpty() && (sortOrder.toUpper() == "ASC" || sortOrder.toUpper() == "DESC")) {
        queryStr += " ORDER BY date_debut " + sortOrder;
    } else {
        // Default sorting that works across databases
        queryStr += " ORDER BY id"; // Or another meaningful default column
    }

    qDebug() << "Requête SQL:" << queryStr;

    QSqlQuery query;
    if (!query.exec(queryStr)) {
        qDebug() << "Erreur Oracle:" << query.lastError().text();
        qDebug() << "Requête complète:" << query.lastQuery();
        delete model;
        return nullptr;
    }

    model->setQuery(query);
    return model;
}
// Modifier un projet
bool Projet::modifier(int id) {
    QSqlQuery query;
    query.prepare("UPDATE PROJETS SET nom = :nom, description = :description, etat = :etat, "
                  "date_debut = :date_debut, date_fin = :date_fin, pdf_path = :pdf_path ,localisation = :localisation WHERE id = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":description", description);
    query.bindValue(":etat", etat);
    query.bindValue(":date_debut", dateDebut);
    query.bindValue(":date_fin", dateFin);
    query.bindValue(":pdf_path", pdfPath);
    query.bindValue(":localisation", localisation);


    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification du projet : " << query.lastError().text();
        return false;
    }

    return true;
}

// Rechercher un projet par nom
QSqlQueryModel* Projet::rechercherParNom(const QString& nom) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    // Construire la requête SQL pour filtrer par nom
    query.prepare("SELECT id, nom, description,localisation, etat, date_debut, date_fin, pdf_path FROM PROJETS WHERE nom LIKE :nom");
    query.bindValue(":nom", "%" + nom + "%"); // Utiliser LIKE pour une recherche partielle

    if (!query.exec()) {
        qDebug() << "Erreur lors de la recherche des projets : " << query.lastError().text();
        delete model;
        return nullptr;
    }

    model->setQuery(query);
    return model;
}

// Méthodes pour les statistiques

// Obtenir le nombre total de projets
int Projet::getTotalProjects() const {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM PROJETS");
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    qDebug() << "Erreur lors de la récupération du nombre total de projets : " << query.lastError().text();
    return 0;
}

// Obtenir le nombre de projets terminés
int Projet::getCompletedProjects() const {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM PROJETS WHERE etat = 'Traité'");
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    qDebug() << "Erreur lors de la récupération du nombre de projets terminés : " << query.lastError().text();
    return 0;
}

// Obtenir le nombre de projets en cours
int Projet::getInProgressProjects() const {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM PROJETS WHERE etat = 'En cours'");
    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    qDebug() << "Erreur lors de la récupération du nombre de projets en cours : " << query.lastError().text();
    return 0;
}

// Créer un graphique des statistiques
QChartView* Projet::createStatisticsChart(QWidget* parent) const {
    // Récupérer les statistiques
    int totalProjects = getTotalProjects();
    int completedProjects = getCompletedProjects();
    int inProgressProjects = getInProgressProjects();

    // Créer les ensembles de données
    QBarSet *set0 = new QBarSet("Total Projects");
    QBarSet *set1 = new QBarSet("Completed Projects");
    QBarSet *set2 = new QBarSet("In Progress Projects");

    *set0 << totalProjects;
    *set1 << completedProjects;
    *set2 << inProgressProjects;

    // Créer la série de barres
    QBarSeries *series = new QBarSeries();
    series->append(set0);
    series->append(set1);
    series->append(set2);

    // Créer le graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Statistiques des projets");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Axes et légende
    QStringList categories;
    categories << "Projets";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, totalProjects + 2); // Ajuster la plage selon le besoin
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Afficher le graphique dans une vue
    QChartView *chartView = new QChartView(chart, parent);
    chartView->setRenderHint(QPainter::Antialiasing);
    return chartView;
}
