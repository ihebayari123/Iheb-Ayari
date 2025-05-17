#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "arduin.h"
#include <QMap>
#include <QDateTime>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QMainWindow>
#include <QListWidget>
#include "login.h"


#include "ui_mainwindow.h"
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
     explicit MainWindow(const QString &role, const QString &name, const QString &email, const QPixmap &photo, QWidget *parent = nullptr);
    ~MainWindow();
    void populateConsultationsComboBox();
    void updateSatisfactionIcon(int satisfied);
    void loadExistingEvaluations();
    void openGestionProjet();
private slots:
    void onItemClicked(QListWidgetItem *item);
    void onItemClicked_3(QListWidgetItem *item);
    void showEvaluationStats();
    void handleEvaluationData(int stars);
    void exportToCSV();
    void importFromCSV();
    void applyTheme(const QString &theme);
    void onConsultationSelected(int index);
    void sendMessageToLCD(const QString &message);


private:
    QChart *chart;
    QString userRole;
    QChartView *chartView;
    Ui::MainWindow ui;
    void customizeListWidget();
    void customizeListWidget_3();
    arduin A;
    QByteArray data;
    void update_label();
    void saveEvaluationToFile(int stars);
    int evaluationCount = 0;
    QMap<int, int> starRatings;
    void onCsvItemClicked(QListWidgetItem *item);
    double averageRating;
    void configureAccess(const QString &role);


    QWidget *currentWidget = nullptr;
};

#endif // MAINWINDOW_H
