#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QDialog>
#include <QChartView>
#include <QGridLayout>

namespace Ui {
class Dashboard;
}

class Dashboard : public QDialog
{
    Q_OBJECT

public:
    explicit Dashboard(QWidget *parent = nullptr);
    ~Dashboard();

    void loadAllStatistics();
    void exportDashboard();

private:
    Ui::Dashboard *ui;

    // Initialize pointers to nullptr
    QChartView *clientChartView = nullptr;
    QChartView *projectChartView = nullptr;
    QChartView *resourceChartView = nullptr;
    QChartView *employeeChartView = nullptr;
    QChartView *consultationChartView = nullptr;

    void setupLayout();
    void clearExistingCharts();
};

#endif // DASHBOARD_H
