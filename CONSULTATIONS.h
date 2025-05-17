#ifndef CONSULTATIONS_H
#define CONSULTATIONS_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDateTime>
#include <QDate>
#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>

#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>

#include <QSqlDatabase>
#include <QMap>
#include <QSet>
#include <QVector>
#include <QQueue>
#include <QStandardItemModel>
#include <QDebug>
class CONSULTATIONS
{
private:
    int ID,CLIENTS_ID;
    QString SUJET, HEURECONSULTATION, STATUS;
    QDateTime CONSULTATIONDATE;
    void createChart();
    void animateChart();

    QChart *chart;
    QChartView *chartView;
public:
    CONSULTATIONS() {}

    CONSULTATIONS( QString SUJET, QDateTime CONSULTATIONDATE, QString HEURECONSULTATION, QString STATUS );

    int getID() const { return ID; }
    QString getSujet() const {
        qDebug() << "getSujet retourne:" << SUJET;
        return SUJET;
    }
    QDateTime getConsultationDate() const { return CONSULTATIONDATE; }
    QString getHeureConsultation() const { return HEURECONSULTATION; }
    QString getStatus() const { return STATUS; }
    int getClientsID() const { return CLIENTS_ID; }

    void setID(int id) { ID = id; }
    void setSujet(const QString& sujet) {
        qDebug() << "setSujet appelé avec:" << sujet;
        SUJET = sujet;
    }
    void setConsultationDate(const QDateTime& date) { CONSULTATIONDATE = date; }
    void setHeureConsultation(const QString& heure) { HEURECONSULTATION = heure; }
    void setStatus(const QString& status) { STATUS = status; }
    void setClientsID(int clientId) { CLIENTS_ID = clientId; }

    bool ajouter();
    QSqlQueryModel* afficher(QString orderByColumn = "", QString orderType = "");
    QSqlQueryModel* afficherPriorisationParStatus();
    QSqlQueryModel* afficherPriorisationParDate();
    QSqlQueryModel* afficherPriorisationMixte();
    QList<int> detecterIncoherences(QSqlQueryModel *model);
    bool supprimer(int);
    void updateChart();
    void createChart(QWidget *widgetChart);
    QSqlQueryModel* rechercherConsultation(const QString& searchValue);


};

#endif // CONSULTATIONS_H
