#include "dashboard.h"
#include "ui_dashboard.h"
#include "client.h"
#include "gestion_projet.h"
#include "ressources.h"
#include "employeinterface.h"
#include "consultations.h"
#include <QScrollArea>
#include <QMessageBox>

Dashboard::Dashboard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dashboard)
{


    ui->setupUi(this);

    // Apply this in your Dashboard constructor after ui->setupUi(this);
    this->setStyleSheet(R"(
    /* Main window background */
    QWidget {
        background-color: #f5f7fa;
    }

    /* Scroll Area styling */
    QScrollArea {
        border: none;
        background: transparent;
    }

    /* Scroll Area viewport */
    QScrollArea > QWidget > QWidget {
        background: transparent;
    }

    /* Vertical scrollbar */
    QScrollBar:vertical {
        border: none;
        background: #e0e5ec;
        width: 10px;
        margin: 0px 0px 0px 0px;
    }

    QScrollBar::handle:vertical {
        background: #a7b4c8;
        min-height: 20px;
        border-radius: 5px;
    }

    QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
        height: 0px;
    }

    QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
        background: transparent;
    }

    /* Horizontal scrollbar (hidden in our case) */
    QScrollBar:horizontal {
        height: 0px;
    }

    /* Chart containers */
    QWidget#chartContainer {
        background: white;
        border-radius: 8px;
        padding: 15px;
    }

    /* Section headers */
    QLabel[sectionHeader="true"] {
        font-size: 18px;
        font-weight: bold;
        color: #2c3e50;
        padding: 5px 0px;
    }

    /* Summary cards */
    QWidget[summaryCard="true"] {
        background: white;
        border-radius: 8px;
        padding: 15px;
        border-left: 5px solid #3498db;
    }

    QWidget[summaryCard="true"] QLabel[cardTitle="true"] {
        font-size: 14px;
        color: #7f8c8d;
    }

    QWidget[summaryCard="true"] QLabel[cardValue="true"] {
        font-size: 24px;
        font-weight: bold;
        color: #2c3e50;
    }

    /* Different card colors */
    QWidget[cardType="clients"] { border-left-color: #3498db; }
    QWidget[cardType="projects"] { border-left-color: #2ecc71; }
    QWidget[cardType="resources"] { border-left-color: #e74c3c; }
    QWidget[cardType="employees"] { border-left-color: #f39c12; }
)");
    ui->exportBtn->setParent(this);
    ui->exportBtn->move(910, 10);
    ui->exportBtn->raise();  // Bring to front
    ui->exportBtn->setAttribute(Qt::WA_TransparentForMouseEvents, false);

    connect(ui->exportBtn, &QPushButton::clicked, this, &Dashboard::exportDashboard);

    setupLayout();
    loadAllStatistics();
}

Dashboard::~Dashboard()
{
    // Clear charts safely
    clearExistingCharts();
    delete ui;
}

void Dashboard::setupLayout()
{
    // Create a grid layout for the dashboard
    QGridLayout *gridLayout = new QGridLayout(ui->scrollAreaWidgetContents);
    gridLayout->setSpacing(20);
    gridLayout->setContentsMargins(15, 15, 15, 15);

    // Add labels for each section
    QLabel *clientLabel = new QLabel("Client Statistics");
    QLabel *projectLabel = new QLabel("Project Statistics");
    QLabel *resourceLabel = new QLabel("Resource Statistics");
    QLabel *employeeLabel = new QLabel("Employee Statistics");
    QLabel *consultationLabel = new QLabel("Consultation Statistics");

    // Add to grid
    gridLayout->addWidget(clientLabel, 0, 0);
    gridLayout->addWidget(projectLabel, 0, 1);
    gridLayout->addWidget(resourceLabel, 1, 0);
    gridLayout->addWidget(employeeLabel, 1, 1);
    gridLayout->addWidget(consultationLabel, 2, 0, 1, 2);

    // Set the layout
    ui->scrollAreaWidgetContents->setLayout(gridLayout);
}

void Dashboard::clearExistingCharts()
{
    // Safe deletion - checks for nullptr first
    if (clientChartView) {
        delete clientChartView;
        clientChartView = nullptr;
    }
    if (projectChartView) {
        delete projectChartView;
        projectChartView = nullptr;
    }
    if (resourceChartView) {
        delete resourceChartView;
        resourceChartView = nullptr;
    }
    if (employeeChartView) {
        delete employeeChartView;
        employeeChartView = nullptr;
    }
    if (consultationChartView) {
        delete consultationChartView;
        consultationChartView = nullptr;
    }
}

void Dashboard::loadAllStatistics()
{
    try {
        clearExistingCharts();

        // Get the grid layout from the scroll area widget
        QGridLayout *gridLayout = qobject_cast<QGridLayout*>(ui->scrollAreaWidgetContents->layout());
        if (!gridLayout) return;

        // Remove existing chart widgets (keep labels)
        for (int i = gridLayout->count() - 1; i >= 5; i--) {
            QLayoutItem *item = gridLayout->takeAt(i);
            if (item->widget()) {
                delete item->widget();
            }
            delete item;
        }

        // 1. Client Statistics
        Client client;
        QMap<QDate, int> growthData = client.getClientGrowthData();

        if (!growthData.isEmpty()) {
            // Create bar series
            QBarSeries *series = new QBarSeries();
            series->setName("Client Growth");

            // Calculate cumulative counts
            int cumulativeCount = 0;
            QBarSet *barSet = new QBarSet("Clients");
            QStringList categories;

            for (auto it = growthData.begin(); it != growthData.end(); ++it) {
                cumulativeCount += it.value();
                *barSet << cumulativeCount;
                categories << it.key().toString("dd MMM yyyy");
            }

            barSet->setColor(QColor(70, 130, 180));
            series->append(barSet);

            // Create chart
            QChart *clientChart = new QChart();
            clientChart->addSeries(series);
            clientChart->setTitle("Client Growth");
            clientChart->legend()->setVisible(true);

            // X-axis
            QBarCategoryAxis *axisX = new QBarCategoryAxis();
            axisX->append(categories);
            axisX->setTitleText("Date");
            axisX->setLabelsAngle(-45);
            clientChart->addAxis(axisX, Qt::AlignBottom);
            series->attachAxis(axisX);

            // Y-axis
            QValueAxis *axisY = new QValueAxis();
            axisY->setTitleText("Number of Clients");
            axisY->setLabelFormat("%i");
            axisY->setMin(0);
            axisY->setMax(cumulativeCount * 1.1);
            clientChart->addAxis(axisY, Qt::AlignLeft);
            series->attachAxis(axisY);

            clientChartView = new QChartView(clientChart);
            clientChartView->setRenderHint(QPainter::Antialiasing);
            clientChartView->setMinimumSize(400, 300);
            gridLayout->addWidget(clientChartView, 0, 0);



        }

        // 2. Project Statistics
        Projet projet;
        QChartView *projectChartView = projet.createStatisticsChart();
        if (projectChartView) {
            projectChartView->setRenderHint(QPainter::Antialiasing);
            projectChartView->setMinimumSize(400, 300);
            gridLayout->addWidget(projectChartView, 0, 1);
        }



        // 3. Resource Statistics
        Res resource(this);
        QSqlQuery query;
        int enPanne = 0, enMarche = 0;

        if (query.exec("SELECT STATUS FROM ressources")) {
            while (query.next()) {
                QString status = query.value(0).toString();
                if (status == "En Panne") enPanne++;
                else if (status == "En marche") enMarche++;
            }

            QPieSeries *pieSeries = new QPieSeries();
            pieSeries->append("En panne", enPanne);
            pieSeries->append("En marche", enMarche);

            QList<QPieSlice*> slices = pieSeries->slices();
            for (int i = 0; i < slices.size(); ++i) {
                QPieSlice *slice = slices.at(i);
                slice->setLabelVisible(true);
                slice->setLabelColor(Qt::black);
                slice->setLabelPosition(QPieSlice::LabelOutside);
            }

            QChart *resourceChart = new QChart();
            resourceChart->addSeries(pieSeries);
            resourceChart->setTitle("Resource Status");
            resourceChart->setAnimationOptions(QChart::SeriesAnimations);

            resourceChartView = new QChartView(resourceChart);
            resourceChartView->setRenderHint(QPainter::Antialiasing);
            resourceChartView->setMinimumSize(400, 300);
            gridLayout->addWidget(resourceChartView, 1, 0);
        }

        // 4. Employee Statistics
        EmployeInterface employee;
        query.prepare("SELECT role, COUNT(*) FROM employes GROUP BY role");
        QPieSeries *employeeSeries = new QPieSeries();

        if (query.exec()) {
            while (query.next()) {
                QString role = query.value(0).toString();
                int count = query.value(1).toInt();
                employeeSeries->append(role, count);
            }

            QChart *employeeChart = new QChart();
            employeeChart->addSeries(employeeSeries);
            employeeChart->setTitle("Employees by Role");

            employeeChartView = new QChartView(employeeChart);
            employeeChartView->setRenderHint(QPainter::Antialiasing);
            employeeChartView->setMinimumSize(400, 300);
            gridLayout->addWidget(employeeChartView, 1, 1);
        }

        // 5. Consultation Statistics
        CONSULTATIONS consultation;
        int planifie = 0, enCours = 0, termine = 0, annulee = 0;

        if (query.exec("SELECT STATUS FROM CONSULTATIONS")) {
            while (query.next()) {
                QString status = query.value(0).toString();
                if (status == "Planifie") planifie++;
                else if (status == "En cours") enCours++;
                else if (status == "Termine") termine++;
                else if (status == "Annule") annulee++;
            }

            QPieSeries *consultationSeries = new QPieSeries();
            consultationSeries->append("Planifie", planifie);
            consultationSeries->append("En cours", enCours);
            consultationSeries->append("Termine", termine);
            consultationSeries->append("Annule", annulee);

            QStringList colors = {"#3498db", "#f1c40f", "#2ecc71", "#e74c3c"};
            QList<QPieSlice *> slices = consultationSeries->slices();
            for (int i = 0; i < slices.size(); i++) {
                slices[i]->setBrush(QBrush(QColor(colors[i])));
                slices[i]->setLabelVisible(true);
                slices[i]->setLabel(QString("%1: %2").arg(slices[i]->label()).arg(slices[i]->value()));
            }

            QChart *consultationChart = new QChart();
            consultationChart->addSeries(consultationSeries);
            consultationChart->setTitle("Consultation Status");
            consultationChart->legend()->setAlignment(Qt::AlignRight);
            consultationChart->setAnimationOptions(QChart::SeriesAnimations);

            consultationChartView = new QChartView(consultationChart);
            consultationChartView->setRenderHint(QPainter::Antialiasing);
            consultationChartView->setMinimumSize(800, 300);
            gridLayout->addWidget(consultationChartView, 2, 0, 1, 2);
        }

    } catch (...) {
        QMessageBox::critical(this, "Error", "Failed to load all statistics");
    }
}

void Dashboard::exportDashboard() {
    QString fileName = QFileDialog::getSaveFileName(this, "Export Dashboard",
                                                    "", "PNG Image (*.png);;PDF File (*.pdf)");
    if (fileName.isEmpty()) return;

    if (fileName.endsWith(".png")) {
        QPixmap pixmap(ui->scrollAreaWidgetContents->size());
        ui->scrollAreaWidgetContents->render(&pixmap);
        pixmap.save(fileName);
    }
    else if (fileName.endsWith(".pdf")) {
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);

        QPainter painter(&printer);
        ui->scrollAreaWidgetContents->render(&painter);
        painter.end();
    }
}
