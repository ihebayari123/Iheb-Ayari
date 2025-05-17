#include "modification_consultation.h"
#include "CONSULTATIONS.h"
#include "connection.h"
#include <QtSql>
#include <QSqlRecord>
#include <QMessageBox>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QTimer>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QFileDialog>
#include <QPixmap>
#include <QBuffer>
#include "QXlsx/header/xlsxdocument.h"
#include <QPdfWriter>
#include <QPainter>
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

// Constructor to initialize a CONSULTATION object with given parameters
CONSULTATIONS::CONSULTATIONS(QString SUJET, QDateTime CONSULTATIONDATE, QString HEURECONSULTATION, QString STATUS) {
    this->SUJET = SUJET;
    this->CONSULTATIONDATE = CONSULTATIONDATE;
    this->HEURECONSULTATION = HEURECONSULTATION;
    this->STATUS = STATUS;
}

// Method to add a consultation to the database
bool CONSULTATIONS::ajouter() {
    QSqlQuery query;

    // Prepare SQL query with placeholders
    query.prepare("INSERT INTO CONSULTATIONS (SUJET, CONSULTATIONDATE, HEURECONSULTATION, STATUS) "
                  "VALUES (:SUJET, TO_DATE(:CONSULTATIONDATE, 'YYYY-MM-DD HH24:MI:SS'), :HEURECONSULTATION, :STATUS)");

    // Format QDateTime to string
    QString consultationDateStr = CONSULTATIONDATE.toString("yyyy-MM-dd HH:mm:ss");

    // Bind values to the placeholders
    query.bindValue(":SUJET", SUJET);
    query.bindValue(":CONSULTATIONDATE", consultationDateStr);
    query.bindValue(":HEURECONSULTATION", HEURECONSULTATION);
    query.bindValue(":STATUS", STATUS);

    // Execute the query and return the result
    if (!query.exec()) {
        qDebug() << "Erreur SQL:" << query.lastError().text();
        return false;
    }

    return true;
}

// Method to fetch and display consultations with optional sorting
QSqlQueryModel* CONSULTATIONS::afficher(QString orderByColumn, QString orderType) {
    QSqlQueryModel *model = new QSqlQueryModel();
    QString query = "SELECT * FROM CONSULTATIONS";

    // Add ORDER BY clause if sorting parameters are provided
    if (!orderByColumn.isEmpty() && !orderType.isEmpty()) {
        query += " ORDER BY " + orderByColumn + " " + orderType;
    }

    model->setQuery(query);

    // Set headers for columns
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("SUJET"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("CONSULTATIONDATE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("HEURECONSULTATION"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("STATUS"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("CLIENTS_ID"));

    return model;
}

// Method to delete a consultation by ID
bool CONSULTATIONS::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM CONSULTATIONS WHERE ID = :id");
    query.bindValue(":id", id);
    return query.exec();
}

// Method to create a pie chart showing consultation status statistics
void CONSULTATIONS::createChart(QWidget *widgetChart) {
    if (!widgetChart) {
        qDebug() << "widgetChart non défini!";
        return;
    }

    int planifie = 0, enCours = 0, termine = 0, annulee = 0;

    // Query all statuses from the database
    QSqlQuery query;
    if (query.exec("SELECT STATUS FROM CONSULTATIONS")) {
        while (query.next()) {
            QString status = query.value(0).toString();
            if (status == "Planifie") planifie++;
            else if (status == "En cours") enCours++;
            else if (status == "Termine") termine++;
            else if (status == "Annule") annulee++;
        }
    } else {
        QMessageBox::warning(nullptr, "Erreur", "Impossible de récupérer les données des consultations !");
        return;
    }

    // Create a pie series and add values
    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->append("Planifie", planifie);
    pieSeries->append("En cours", enCours);
    pieSeries->append("Termine", termine);
    pieSeries->append("Annule", annulee);

    // Set custom colors and labels
    QStringList colors = {"#3498db", "#f1c40f", "#2ecc71", "#e74c3c"};
    QList<QPieSlice *> slices = pieSeries->slices();
    for (int i = 0; i < slices.size(); i++) {
        slices[i]->setBrush(QBrush(QColor(colors[i])));
        slices[i]->setLabelVisible(true);
        slices[i]->setLabel(QString("%1: %2").arg(slices[i]->label()).arg(slices[i]->value()));
    }

    // Display the chart inside the widget
    QChart *chart = new QChart();
    chart->addSeries(pieSeries);
    chart->setTitle("Statistiques des consultations par statut");
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QChartView *chartView = new QChartView(chart, widgetChart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Clear existing layout if exists
    QLayout *layout = widgetChart->layout();
    if (layout != nullptr) {
        QLayoutItem *item;
        while ((item = layout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete layout;
    }

    // Set new layout with chart
    QVBoxLayout *newLayout = new QVBoxLayout(widgetChart);
    newLayout->addWidget(chartView);
    widgetChart->setLayout(newLayout);
}

// Method to search consultations by keyword in subject or status
QSqlQueryModel* CONSULTATIONS::rechercherConsultation(const QString& searchValue) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM CONSULTATIONS WHERE SUJET LIKE :search OR STATUS LIKE :search");
    query.bindValue(":search", "%" + searchValue + "%");

    if (!query.exec()) {
        qDebug() << "Erreur dans la recherche :" << query.lastError().text();
        return nullptr;
    }

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("SUJET"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("CONSULTATIONDATE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("HEURECONSULTATION"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("STATUS"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("CLIENTS_ID"));

    return model;
}

// Method to prioritize consultations by status (Planifie > En cours > Termine > Annule)
QSqlQueryModel* CONSULTATIONS::afficherPriorisationParStatus() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CONSULTATIONS "
                    "ORDER BY CASE "
                    "WHEN STATUS = 'Planifie' THEN 1 "
                    "WHEN STATUS = 'En cours' THEN 2 "
                    "WHEN STATUS = 'Termine' THEN 3 "
                    "WHEN STATUS = 'Annule' THEN 4 "
                    "END");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("SUJET"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("CONSULTATIONDATE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("HEURECONSULTATION"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("STATUS"));
    return model;
}

// Method to prioritize consultations by date
QSqlQueryModel* CONSULTATIONS::afficherPriorisationParDate() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CONSULTATIONS ORDER BY CONSULTATIONDATE ASC");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("SUJET"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("CONSULTATIONDATE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("HEURECONSULTATION"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("STATUS"));
    return model;
}

// Method to prioritize consultations first by status, then by date
QSqlQueryModel* CONSULTATIONS::afficherPriorisationMixte() {
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM CONSULTATIONS "
                    "ORDER BY CASE "
                    "WHEN STATUS = 'Planifie' THEN 1 "
                    "WHEN STATUS = 'En cours' THEN 2 "
                    "WHEN STATUS = 'Termine' THEN 3 "
                    "WHEN STATUS = 'Annule' THEN 4 "
                    "END, CONSULTATIONDATE ASC");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("SUJET"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("CONSULTATIONDATE"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("HEURECONSULTATION"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("STATUS"));
    return model;
}

// Method to detect incoherent consultations (e.g. 'Planifie' in the past, 'Termine' in the future)
QList<int> CONSULTATIONS::detecterIncoherences(QSqlQueryModel *model) {
    QList<int> lignesIncoherentes;
    QDate aujourdHui = QDate::currentDate();

    for (int row = 0; row < model->rowCount(); ++row) {
        int id = model->data(model->index(row, 0)).toInt();
        QString status = model->data(model->index(row, 4)).toString();
        QDate date = model->data(model->index(row, 2)).toDate();

        if ((status == "Planifie" && date < aujourdHui) ||
            (status == "Termine" && date > aujourdHui)) {

            lignesIncoherentes.append(row);


            /*QSqlQuery query;
            query.prepare("UPDATE CONSULTATIONS SET STATUS = 'Annule' WHERE ID = :id");
            query.bindValue(":id", id);
            query.exec();*/

        }
    }

    return lignesIncoherentes;
}

