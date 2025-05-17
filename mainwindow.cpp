#include "mainwindow.h"
#include "ressources.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Consultationwindow.h"
#include "client.h"
#include "ui_client.h"
#include "ui_consultationwindow.h"
#include <QVBoxLayout>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QPushButton>
#include <QFile>
#include <QMap>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QTimer>
#include <QDateTime>
#include <QWidget>
#include <QFileDialog>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QBarSet>
#include <QBarSeries>
#include <QFont>
#include "SettingsDialog.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QPixmap>
#include "gestion_projet.h"
#include "ui_gestion_projet.h"
#include "employeinterface.h"
#include "login.h"
#include "dashboard.h"
#include "ui_dashboard.h"



MainWindow::MainWindow(const QString &role, const QString &name, const QString &email, const QPixmap &photo, QWidget *parent) {
    ui.setupUi(this);  // Set up the UI
    configureAccess(role);
    ui.username->setText(name);

    // Afficher l'email
    ui.email1->setText(email);

    // Afficher la photo
    QPixmap pixmap(photo);
    if (!pixmap.isNull()) {
        ui.image->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio)); // Ajuster la taille
    } else {
        ui.image->setText("Photo non disponible");
    }
    customizeListWidget();
    customizeListWidget_3();
    evaluationCount = 0;
    for(int i = 1; i <= 4; i++) {
        starRatings[i] = 0;
    }


    connect(ui.csv, &QPushButton::clicked,
            this, &MainWindow::populateConsultationsComboBox);
    //loadExistingEvaluations();

    // Connect the item clicked signal to the slot
    connect(ui.consultationComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onConsultationSelected);
    connect(ui.listWidget, &QListWidget::itemClicked, this, &MainWindow::onItemClicked);
    connect(ui.listWidget_3, &QListWidget::itemClicked, this, &MainWindow::onItemClicked_3);
    connect(ui.afficher_arduino, &QPushButton::clicked, this, [=]() {
        QString message = QString("Avg: %1").arg(averageRating, 0, 'f', 1);
        sendMessageToLCD(message);
    });



    // In MainWindow constructor:
    int ret = A.connect_arduino("COM6");  // Force COM6 for MainWindow
    switch(ret) {
    case 0:
        qDebug() << "MainWindow connected to COM6";
        connect(A.getserial(), &QSerialPort::readyRead, this, &MainWindow::update_label);
        break;
        // ... error cases ...
    }
    chart = new QChart();
    chart->setTheme(QChart::ChartThemeLight); // Thème clair
    chart->setBackgroundBrush(QBrush(Qt::white));
    chart->setMargins(QMargins(0, 0, 0, 0)); // Réduire les marges

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRubberBand(QChartView::RectangleRubberBand); // Zoom rectangle
    ui.verticalLayout->addWidget(chartView);
    // Ajoutez chartView à votre interface (par exemple dans un layout)

    connect(ui.csv, &QPushButton::clicked, this, &MainWindow::loadExistingEvaluations);
    connect(ui.csv, &QPushButton::clicked, this, &MainWindow::showEvaluationStats);
}

MainWindow::~MainWindow() {
    if (A.getserial() && A.getserial()->isOpen()) {
        A.getserial()->disconnect(); // Disconnect all signals
        A.close_arduino();
    }
}
void MainWindow::configureAccess(const QString &role) {
    for (int i = 0; i < ui.listWidget->count(); ++i) {
        QListWidgetItem *item = ui.listWidget->item(i);

        // Toujours garder "Paramètres" et "Se déconnecter" visibles
        if (item->text() == "Paramétres" || item->text() == "Se déconnecter") {
            item->setHidden(false); // Toujours visible
            continue; // Passer à l'élément suivant
        }

        // Configurer les autres éléments en fonction du rôle
        if (role == "Admin") {
            item->setHidden(false); // Admin a accès à tout
        } else if (role == "Responsable des ressources") {
            if (item->text() == "ressources") {
                item->setHidden(false); // Montrer "ressources"
            } else {
                item->setHidden(true); // Masquer les autres
            }
        } else if (role == "Consultant") {
            if (item->text() == "Consultations" || item->text() == "Projects") {
                item->setHidden(false); // Montrer "Consultations" et "Projects"
            } else {
                item->setHidden(true); // Masquer les autres
            }
        } else if (role == "RH") {
            if (item->text() == "Employé") {
                item->setHidden(false); // Montrer "Employé"
            } else {
                item->setHidden(true); // Masquer les autres
            }
        } else {
            // Aucun accès pour les autres rôles
            item->setHidden(true);
        }
    }

    qDebug() << "Access configured for role:" << role;
}
// When opening gestion_projet from MainWindow
void MainWindow::openGestionProjet() {
    if (A.getserial() && A.getserial()->isOpen()) {
        A.getserial()->disconnect(this); // Disconnect only MainWindow's connections
        A.close_arduino();
    }
    gestion_projet gp;
    gp.exec();
    // Reconnect when returning
    if (A.connect_arduino() == 0) {
        connect(A.getserial(), &QSerialPort::readyRead, this, &MainWindow::update_label);
    }
}

void MainWindow::onConsultationSelected(int index) {
    if (index < 0) {
        qDebug() << "Invalid index";
        return;
    }

    QVariant idData = ui.consultationComboBox->itemData(index);
    if (!idData.isValid() || idData.toInt() == 0) {
        qDebug() << "Invalid or placeholder item selected";
        ui.satisfaction->setPixmap(QPixmap());
        ui.satisfaction->setToolTip("");
        return;
    }

    int consultationId = idData.toInt();
    QSqlQuery query;
    query.prepare("SELECT SATISFACATION FROM CONSULTATIONS WHERE ID = :id");
    query.bindValue(":id", consultationId);

    if (!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        QString satisfaction = query.value("SATISFACATION").toString();
        if (satisfaction == "satisfait") {
            updateSatisfactionIcon(1);
        } else if (satisfaction == "non satisfait") {
            updateSatisfactionIcon(0);
        } else {
            ui.satisfaction->setPixmap(QPixmap());
            ui.satisfaction->setToolTip("");
        }
    }
}
void MainWindow::update_label() {
    if(!A.getserial()->isOpen()) {
        qDebug() << "Serial port not open!";
        return;
    }

    static QString buffer; // Buffer pour accumuler les données

    while(A.getserial()->bytesAvailable()) {
        QByteArray data = A.getserial()->readAll();
        buffer += QString::fromUtf8(data);

        // Vérifier si le buffer contient un message complet (terminé par \n ou \r)
        int endPos = buffer.indexOf('\n');
        if(endPos == -1) endPos = buffer.indexOf('\r');

        if(endPos != -1) {
            QString message = buffer.left(endPos).trimmed();
            buffer = buffer.mid(endPos + 1); // Retirer le message traité

            qDebug() << "Complete message:" << message;

            if(message.startsWith("EVAL:")) {
                QString evalStr = message.mid(5);
                bool ok;
                int stars = evalStr.toInt(&ok);

                if(ok && stars >= 1 && stars <= 4) {
                    handleEvaluationData(stars);
                    ui.label_3->setText(QString("New Evaluation: %1 stars").arg(stars));
                }
            }
            else if(message.startsWith("SATISFIED:")) {
                QString satisfactionStr = message.mid(10);
                bool ok;
                int satisfied = satisfactionStr.toInt(&ok);

                if(ok) {
                    updateSatisfactionIcon(satisfied);
                }
            }
            else if(message == "EVAL:CANCEL") {
                ui.label_3->setText("Evaluation cancelled");
            }
            else if(message == "ARDUINO_READY") {
                ui.label_3->setText("Arduino connected");
            }
        }
    }
}
void MainWindow::loadExistingEvaluations() {
    // Reset counters
    QSqlDatabase db = QSqlDatabase::database();
    if(!db.isOpen()) {
        qDebug() << "Database is not connected!";
        return;
    }
    evaluationCount = 0;
    for(int i = 1; i <= 4; i++) {
        starRatings[i] = 0;
    }

    // Load evaluation data from database
    QSqlQuery query("SELECT ID, EVALUATION, SATISFACATION FROM CONSULTATIONS WHERE STATUS = 'Termine'");

    while(query.next()) {
        int id = query.value("ID").toInt();
        int evaluation = query.value("EVALUATION").toInt();
        QString satisfaction = query.value("SATISFACATION").toString();

        // Update evaluation stats
        if(evaluation >= 1 && evaluation <= 4) {

            evaluationCount++;
            starRatings[evaluation]++;
        }

        // Update satisfaction display if it's the currently selected consultation
        if(id == ui.consultationComboBox->currentData().toInt()) {
            if(satisfaction == "satisfait") {
                updateSatisfactionIcon(1);
            } else if(satisfaction == "non satisfait") {
                updateSatisfactionIcon(0);
            }
        }
    }

    // Update the UI with loaded data
    showEvaluationStats();
}
void MainWindow::updateSatisfactionIcon(int satisfied) {
    // Get the currently selected consultation ID
    int currentConsultationId = ui.consultationComboBox->currentData().toInt();

    // Validate we have a real consultation selected (not the placeholder)
    if (currentConsultationId == 0) {
        qDebug() << "No valid consultation selected";
        return;
    }

    QString iconUrl;
    QString satisfactionText;
    QString satisfactionDbValue;

    if (satisfied == 1) {
        iconUrl = ":/img/images/smile.png";
        satisfactionText = tr("Customer is satisfied");
        ui.satisfaction->setToolTip("Customer is satisfied");
        satisfactionDbValue = "satisfait";
    } else {
        iconUrl = ":/img/images/angry.png";
        satisfactionText = tr("Customer is not satisfied");
        ui.satisfaction->setToolTip("Customer is satisfied");
        satisfactionDbValue = "non satisfait";
    }

    // Update UI
    ui.satisfaction->setToolTip(satisfactionText);
    QPixmap pixmap(iconUrl);
    if (!pixmap.isNull()) {
        ui.satisfaction->setPixmap(pixmap.scaled(32, 32, Qt::KeepAspectRatio));
    } else {
        qDebug() << "Failed to load satisfaction icon:" << iconUrl;
    }

    // Update database
    QSqlQuery query;
    query.prepare("UPDATE CONSULTATIONS SET SATISFACATION = :satisfaction WHERE ID = :id");
    query.bindValue(":satisfaction", satisfactionDbValue);
    query.bindValue(":id", currentConsultationId);

    if (!query.exec()) {
        qDebug() << "Failed to update satisfaction in database for consultation ID"
                 << currentConsultationId << ":" << query.lastError().text();

        // Optional: Show error to user
        QMessageBox::warning(this, tr("Database Error"),
                             tr("Failed to update satisfaction status in database."));
    } else {
        qDebug() << "Successfully updated satisfaction for consultation ID"
                 << currentConsultationId << "to" << satisfactionDbValue;
    }
}
void MainWindow::populateConsultationsComboBox() {
    ui.consultationComboBox->clear();

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Database not connected!";
        ui.consultationComboBox->addItem("Database not connected", 0);
        ui.consultationComboBox->setEnabled(false);
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT ID, SUJET FROM CONSULTATIONS WHERE STATUS = 'Termine'")) {
        qDebug() << "Query error:" << query.lastError().text();
        ui.consultationComboBox->addItem("Error loading consultations", 0);
        ui.consultationComboBox->setEnabled(false);
        return;
    }

    bool hasItems = false;
    while (query.next()) {
        int id = query.value("ID").toInt();
        QString sujet = query.value("SUJET").toString();
        ui.consultationComboBox->addItem(QString("%1 - %2").arg(id).arg(sujet), id);
        hasItems = true;
    }

    if (!hasItems) {
        ui.consultationComboBox->addItem("No completed consultations", 0);
        ui.consultationComboBox->setEnabled(false);
    } else {
        ui.consultationComboBox->setCurrentIndex(0);
        // Force update for the first item
        QMetaObject::invokeMethod(this, "onConsultationSelected",
                                  Qt::QueuedConnection,
                                  Q_ARG(int, 0));
    }
}

void MainWindow::handleEvaluationData(int stars) {
    if(stars < 1 || stars > 4) return;  // Validate input


    evaluationCount++;
    starRatings[stars]++;

    // Save to file (keep your current logic)
    saveEvaluationToFile(stars);

    // Get selected consultation ID from combo box
    int selectedConsultationId = ui.consultationComboBox->currentData().toInt();

    // Save to database
    QSqlQuery query;
    query.prepare("UPDATE CONSULTATIONS SET EVALUATION = :eval WHERE ID = :id");
    query.bindValue(":eval", stars);
    query.bindValue(":id", selectedConsultationId);

    if (!query.exec()) {
        qDebug() << "Failed to update evaluation:" << query.lastError().text();
    } else {
        qDebug() << "Evaluation saved for consultation ID:" << selectedConsultationId;
    }

    // Update the UI
    showEvaluationStats();
    A.write_to_arduino(QString("ACK:%1").arg(stars).toUtf8());
}

void MainWindow::onCsvItemClicked(QListWidgetItem *item) {
    bool ok;
    int stars = item->text().toInt(&ok); // suppose que l’item contient un chiffre
    if (ok && stars >= 1 && stars <= 4) {
        handleEvaluationData(stars);
    }
}


void MainWindow::saveEvaluationToFile(int stars)
{
    QFile file("evaluations.csv");
    if(file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
            << "," << stars << "\n";
        file.close();
    }
}

void MainWindow::showEvaluationStats() {
    double average = 0;
    int totalRatings = 0;

    // Calcul des statistiques
    for(auto it = starRatings.constBegin(); it != starRatings.constEnd(); ++it) {
        average += it.key() * it.value();
        totalRatings += it.value();
    }

    if(totalRatings > 0) {
        average /= totalRatings;
    }

    averageRating = average;

    // Nettoyage du graphique existant
    chart->removeAllSeries();


    // Suppression des anciens axes (nouvelle méthode)
    QList<QAbstractAxis*> oldAxes = chart->axes();
    for(QAbstractAxis* axis : oldAxes) {
        chart->removeAxis(axis);
        delete axis;
    }

    // Création de la nouvelle série
    QBarSeries *series = new QBarSeries();
    QBarSet *set = new QBarSet("Évaluations");
    QVector<QColor> starColors = {
        QColor(255, 215, 0),   // Or - 4 étoiles
        QColor(192, 192, 192), // Argent - 3 étoiles
        QColor(205, 127, 50),  // Bronze - 2 étoiles
        QColor(160, 160, 160)   // Marron (1 étoile)
    };
    // Remplissage des données
    for(int i = 4; i >= 1; i--) {
        *set << starRatings.value(i, 0);
        set->setColor(starColors[4-i]); // Appliquer la couleur
    }
    series->append(set);
    chart->addSeries(series);

    // Configuration des axes
    QStringList categories;
    for(int i = 4; i >= 1; i--) {
        categories << QString("⭐").repeated(i); // Utilisation d'émojis étoiles
    }

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText("Niveau de satisfaction");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre d'évaluations");
    axisY->setLabelFormat("%d");
    axisY->setTickCount(qMin(10, totalRatings+1)); // Adapte les ticks au nombre d'évals
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Style du graphique
    chart->setTitle("Répartition des évaluations");
    chart->legend()->setVisible(true);
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->setTitle("<h2>Répartition des évaluations</h2>");
    chart->setTitleBrush(QBrush(Qt::darkBlue));
    chart->legend()->setVisible(false); // On cache la légende pour plus de place

    // Animation fluide
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setAnimationDuration(1000); // 1 seconde d'animation

    // Ajout d'ombre pour la profondeur


    // Personnalisation des labels
    QFont labelsFont;
    labelsFont.setPointSize(10);
    labelsFont.setBold(true);
    axisX->setLabelsFont(labelsFont);
    axisY->setLabelsFont(labelsFont);

    // Mise à jour du texte
    ui.label_3->setText(
        QString("Total Evaluations: %1\nAverage Rating: %2\n\n"
                "4 stars: %3\n3 stars: %4\n2 stars: %5\n1 star: %6")
            .arg(totalRatings)
            .arg(average, 0, 'f', 1)
            .arg(starRatings.value(4, 0))
            .arg(starRatings.value(3, 0))
            .arg(starRatings.value(2, 0))
            .arg(starRatings.value(1, 0)));
}
void MainWindow::exportToCSV() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Exporter les données", "", "Fichiers CSV (*.csv)");

    if(fileName.isEmpty()) return;

    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Date,Évaluation\n";

        QFile sourceFile("evaluations.csv");
        if(sourceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            out << sourceFile.readAll();
            sourceFile.close();
        }
        file.close();
    }
}

void MainWindow::importFromCSV() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Importer des données", "", "Fichiers CSV (*.csv)");

    if(fileName.isEmpty()) return;

    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        // Réinitialiser les stats
        starRatings.clear();
        evaluationCount = 0;

        QTextStream in(&file);
        while(!in.atEnd()) {
            QString line = in.readLine();
            if(line.contains(",")) {
                QStringList parts = line.split(",");
                if(parts.size() >= 2 && parts[1].toInt() > 0) {
                    int stars = parts[1].toInt();
                    starRatings[stars]++;
                    evaluationCount++;
                }
            }
        }
        file.close();

        // Mettre à jour l'affichage
        showEvaluationStats();
    }
}
void MainWindow::customizeListWidget() {
    QListWidget *listWidget = ui.listWidget;
    listWidget->setDragDropMode(QAbstractItemView::NoDragDrop);


    // Apply spacing and item size
    listWidget->setSpacing(1);


    for (int i = 0; i < listWidget->count(); ++i) {
        QListWidgetItem *item = listWidget->item(i);
        item->setSizeHint(QSize(item->sizeHint().width(), 20));



    }

    // Apply stylesheet
    listWidget->setStyleSheet(
        "QListWidget {"
        "   background-color: rgb(221, 221, 221);"
        "   border-radius: 10px;"
        "   padding: 5px;"
        "}"
        "QListWidget::item {"
        "   padding: 10px;"
        "   color: #555;"
        "   font-size: 25px;"
        "   font-family: Arial, sans-serif;"
        "   icon-size: 5px 5px;"
        "}"
        "QListWidget::item:hover {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border-radius: 5px;"
        "}"
        "QListWidget::item:selected {"
        "   background-color: #2980b9;"
        "   color: white;"
        "   border-radius: 5px;"
        "   font-weight: bold;"
        "}"

        );
}
void MainWindow::onItemClicked(QListWidgetItem *item) {
    QString itemText = item->text();
    QWidget *widget = ui.widget;  // Le widget conteneur dans votre interface

    if (currentWidget && currentWidget != ui.widget) {
        currentWidget->hide();
    }

    if (itemText == "ressources") {
        if (currentWidget != nullptr && currentWidget->inherits("Res")) {
            currentWidget->show();
        } else {
            Res *res = new Res(this);
            QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(widget->layout());

            if (!layout) {
                layout = new QVBoxLayout(widget);
                widget->setLayout(layout);
            }

            // Supprime les anciens widgets du layout
            QLayoutItem* child;
            while ((child = layout->takeAt(0)) != nullptr) {
                delete child->widget();
                delete child;
            }

            layout->addWidget(res);
            res->show();
            currentWidget = res;
        }
    }
    else if (itemText == "Consultations") {
        if (currentWidget != nullptr && currentWidget->inherits("ConsultationWindow")) {
            currentWidget->show();
        } else {
            Consultationwindow *consultation = new Consultationwindow(this);
            QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(widget->layout());

            if (!layout) {
                layout = new QVBoxLayout(widget);
                widget->setLayout(layout);
            }

            // Supprime les anciens widgets du layout
            QLayoutItem* child;
            while ((child = layout->takeAt(0)) != nullptr) {
                delete child->widget();
                delete child;
            }

            layout->addWidget(consultation);
            consultation->show();
            currentWidget = consultation;
        }
    }
    else if (itemText == "Clients") {
        if (currentWidget != nullptr && currentWidget->inherits("Clients")) {
            currentWidget->show();
        } else {
            Client *client = new Client(this);
            QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(widget->layout());

            if (!layout) {
                layout = new QVBoxLayout(widget);
                widget->setLayout(layout);
            }

            // Supprime les anciens widgets du layout
            QLayoutItem* child;
            while ((child = layout->takeAt(0)) != nullptr) {
                delete child->widget();
                delete child;
            }

            layout->addWidget(client);
            client->show();
            currentWidget = client;
        }
    }
    else if (itemText == "Employé") {
        if (currentWidget != nullptr && currentWidget->inherits("EmployeInterface")) {
            currentWidget->show();
        } else {
            EmployeInterface *employe = new EmployeInterface(this);
            QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(widget->layout());

            if (!layout) {
                layout = new QVBoxLayout(widget);
                widget->setLayout(layout);
            }

            // Supprime les anciens widgets du layout
            QLayoutItem* child;
            while ((child = layout->takeAt(0)) != nullptr) {
                delete child->widget();
                delete child;
            }

            layout->addWidget(employe);
            employe->show();
            currentWidget = employe;
        }
    }
    else if (itemText == "Dashboard") {
        if (currentWidget != nullptr && currentWidget->inherits("Dashboard")) {
            currentWidget->show();
        } else {
            Dashboard *dashboard = new Dashboard(this);
            QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(widget->layout());

            if (!layout) {
                layout = new QVBoxLayout(widget);
                widget->setLayout(layout);
            }

            // Clear existing widgets
            QLayoutItem* child;
            while ((child = layout->takeAt(0)) != nullptr) {
                delete child->widget();
                delete child;
            }

            layout->addWidget(dashboard);
            dashboard->show();
            currentWidget = dashboard;
        }
    }
    else if (itemText == "Projects") {
        QWidget *widget = ui.widget;  // Assurez-vous que 'widget' est correctement défini dans ton .ui

        if (widget) {
            // Si un autre widget est déjà affiché, on le cache
            if (currentWidget && currentWidget != ui.widget) {
                currentWidget->hide();
            }

            // Si le currentWidget est déjà une instance de Res, on ne recrée pas un nouveau
            if (currentWidget != nullptr && currentWidget->inherits("gestion_projet")) {
                // Le widget est déjà un Res, on l'affiche à nouveau
                currentWidget->show();
            } else {
                // Crée et affiche la nouvelle fenêtre Res dans le widget
                gestion_projet *projet = new gestion_projet(this);  // Crée un nouveau Res
                QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(widget->layout());  // Récupère le layout actuel du widget

                if (!layout) {
                    layout = new QVBoxLayout(widget);  // Si pas de layout existant, crée-en un
                    widget->setLayout(layout);
                }

                layout->addWidget(projet);  // Ajoute le nouveau Res au layout
                projet->show();  // Affiche le Res
                currentWidget = projet;  // Définit le Res comme widget actuel
            }
        }
    }
    else {
        if (currentWidget) {
            currentWidget->hide();
            currentWidget = nullptr;
        }
    }
}
void MainWindow::customizeListWidget_3() {
    QListWidget *listWidget_3 = ui.listWidget_3;
    listWidget_3->setDragDropMode(QAbstractItemView::NoDragDrop);

    // Apply spacing and item size for listWidget_3
    listWidget_3->setSpacing(1);

    for (int i = 0; i < listWidget_3->count(); ++i) {
        QListWidgetItem *item = listWidget_3->item(i);
        item->setSizeHint(QSize(item->sizeHint().width(), 20));
    }

    // Apply stylesheet

}

void MainWindow::onItemClicked_3(QListWidgetItem *item) {
    QString itemText = item->text();

    if (itemText == "Se déconnecter") {
        // Afficher une boîte de confirmation avant de déconnecter
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmation", "Êtes-vous sûr de vouloir vous déconnecter ?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            this->close(); // Ferme la fenêtre principale

            // Rouvrir la fenêtre de connexion
            Login loginDialog;
            if (loginDialog.exec() == QDialog::Accepted) {
                QString userRole = loginDialog.getUserRole(); // Récupère le rôle de l'utilisateur
                QString userName = loginDialog.getUserName(); // Récupère le nom de l'utilisateur
                QString userEmail = loginDialog.getUserEmail(); // Récupère l'email de l'utilisateur
                QPixmap userPhoto = loginDialog.getUserPhoto(); // Récupère la photo de l'utilisateur

                MainWindow *mainWindow = new MainWindow(userRole, userName, userEmail, userPhoto);
                mainWindow->show();
            }
        }
    }
    else if (itemText == "ressources") {
        QWidget *widget = ui.widget;  // Assurez-vous que 'widget' est correctement défini dans ton .ui

        if (widget) {
            // Si un autre widget est déjà affiché, on le cache
            if (currentWidget && currentWidget != ui.widget) {
                currentWidget->hide();
            }

            // Si le currentWidget est déjà une instance de Res, on ne recrée pas un nouveau
            if (currentWidget != nullptr && currentWidget->inherits("Res")) {
                // Le widget est déjà un Res, on l'affiche à nouveau
                currentWidget->show();
            } else {
                // Crée et affiche la nouvelle fenêtre Res dans le widget
                Res *res = new Res(this);  // Crée un nouveau Res
                QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(widget->layout());  // Récupère le layout actuel du widget

                if (!layout) {
                    layout = new QVBoxLayout(widget);  // Si pas de layout existant, crée-en un
                    widget->setLayout(layout);
                }

                layout->addWidget(res);  // Ajoute le nouveau Res au layout
                res->show();  // Affiche le Res
                currentWidget = res;  // Définit le Res comme widget actuel
            }
        }
    }
    else if (itemText == "Paramétres") {
        QWidget *widget = ui.widget;  // Assurez-vous que 'widget' est correctement défini dans ton .ui

        if (widget) {
            // Si un autre widget est déjà affiché, on le cache
            if (currentWidget && currentWidget != ui.widget) {
                currentWidget->hide();
            }

            // Si le currentWidget est déjà une instance de Res, on ne recrée pas un nouveau
            if (currentWidget != nullptr && currentWidget->inherits("SettingsDialog")) {
                // Le widget est déjà un Res, on l'affiche à nouveau
                currentWidget->show();
            } else {
                // Crée et affiche la nouvelle fenêtre Res dans le widget
                SettingsDialog *Settings = new SettingsDialog(this);  // Crée un nouveau Res
                QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(widget->layout());  // Récupère le layout actuel du widget

                if (!layout) {
                    layout = new QVBoxLayout(widget);  // Si pas de layout existant, crée-en un
                    widget->setLayout(layout);
                }

                layout->addWidget(Settings);  // Ajoute le nouveau Res au layout
                Settings->show();  // Affiche le Res
                currentWidget = Settings;  // Définit le Res comme widget actuel
            }
        }  // Ouvre la boîte de dialogue en modal
    }

    else {
        // Si un autre élément est sélectionné, cache le widget actuel
        if (currentWidget) {
            currentWidget->hide();
            currentWidget = nullptr;
        }
    }
}
void MainWindow::applyTheme(const QString &theme)
{
    if (theme == "Sombre") {
        qApp->setStyleSheet("QWidget { background-color: #121212; color: white; }"
                            "QPushButton { background-color: #1E1E1E; color: white; }");
    } else {
        qApp->setStyleSheet(""); // Par défaut, clair
    }
}

void MainWindow::sendMessageToLCD(const QString &message) {
    QByteArray data = message.toUtf8();
    data.append('\n');
    A.write_to_arduino(data);
}

