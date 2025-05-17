#ifndef CONSULTATIONWINDOW_H
#define CONSULTATIONWINDOW_H
#include "Arduino.h"
#include <QMap>
#include <QDateTime>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QMainWindow>
#include "CONSULTATIONS.h"
#include "window.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDialog>
#include <QVBoxLayout>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class Consultationwindow;
}
QT_END_NAMESPACE

class QPushButton;
class CalendarDialog;

// Consultationwindow.h
class Consultationwindow : public QDialog


{
    Q_OBJECT

public:
    Consultationwindow(QWidget *parent = nullptr);
    ~Consultationwindow();

    void onCsvItemClicked(QListWidgetItem *item);


    void afficherStatsSurArduino();



    Ui::Consultationwindow *ui;

private slots:
    void on_pushButton_ajouter_Consultation_clicked();
    void on_pushButton_supprimer_Consultation_clicked();
    void rechercherEnDirect(const QString &searchText);
    void onModifierConsultation();
    void colorerIncoherences(QSqlQueryModel *model);
    void on_pushButton_supprimerAnnule_clicked();
    void on_pushButton_calendrier_clicked();
    void exporterVersExcel();
    void exporterVersPDF();
    void trierTable();
    void on_prioriserButton_clicked();
    void onConsultationSelected(int index);
    void showEvaluationStats();
    void handleEvaluationData(int stars);
    void populateConsultationsComboBox();
    void updateSatisfactionIcon(int satisfied);
    void loadExistingEvaluations();

private:
    Window *calendarWindow = nullptr;
    QChart *chart;
    QChartView *chartView;
    Arduino A;
    QByteArray data;
    void update_label();
    void saveEvaluationToFile(int stars);
    int evaluationCount = 0;
    QMap<int, int> starRatings;
    CONSULTATIONS Etmp;
};

#endif // CONSULTATIONWINDOW_H
