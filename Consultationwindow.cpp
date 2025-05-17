#include "Consultationwindow.h"
#include "ui_consultationwindow.h"
#include "connection.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QTimer>
#include <QListWidgetItem>
#include <QDateTime>
#include <QWidget>
#include <QFileDialog>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QBarSet>
#include <QBarSeries>
#include <QFont>
#include "CONSULTATIONS.h"
#include "modification_consultation.h"
#include "window.h"
#include <QPdfWriter>
#include <QPainter>
#include <QMessageBox>
#include <QFileDialog>
#include <QSqlQueryModel>
#include "QXlsx/header/xlsxdocument.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QStringList>
#include <QCompleter>
#include <QStringListModel>
#include <QDebug>
#include <QCalendarWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLocale>
#include <QTextCharFormat>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QOverload>

Consultationwindow::Consultationwindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::Consultationwindow)

{
    ui->setupUi(this);
    evaluationCount = 0;
    for(int i = 1; i <= 4; i++) {
        starRatings[i] = 0;
    }
    ui->tableView->setModel(Etmp.afficher());
    connect(ui->btn, &QPushButton::clicked,
            this, &Consultationwindow::populateConsultationsComboBox);
    connect(ui->consultationComboBox1, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Consultationwindow::onConsultationSelected);

    connect(ui->pushButton_modifier_Consultation, &QPushButton::clicked, this, &Consultationwindow::onModifierConsultation);
    connect(ui->pushButton_exporter_exel, &QPushButton::clicked, this, &Consultationwindow::exporterVersExcel);

    CONSULTATIONS consultation;
    consultation.createChart(ui->widgetChart);

    connect(ui->lineEdit_rechercge, &QLineEdit::textChanged, this, &Consultationwindow::rechercherEnDirect);
    connect(ui->comboBox_tri, &QComboBox::currentIndexChanged, this, &Consultationwindow::trierTable);
    connect(ui->pushButton_rapport_PDF, &QPushButton::clicked, this, &Consultationwindow::exporterVersPDF);
    int ret = A.connect_arduino();
    switch(ret) {
    case(0):
        qDebug() << "Arduino is available and connected to: " << A.getarduino_port_name();
        break;
    case(1):
        qDebug() << "Arduino is available and not connected to: " << A.getarduino_port_name();
        break;
    case(-1):
        qDebug() << "Arduino is not available.";
    }

    // Ensure getserial() returns a pointer to QSerialPort
    QObject::connect(A.getserial(), &QSerialPort::readyRead, this, &Consultationwindow::update_label);
    chart = new QChart();
    chart->setTheme(QChart::ChartThemeLight); // Thème clair
    chart->setBackgroundBrush(QBrush(Qt::white));
    chart->setMargins(QMargins(0, 0, 0, 0)); // Réduire les marges

    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setRubberBand(QChartView::RectangleRubberBand); // Zoom rectangle
    ui->layout1->addWidget(chartView);
    // Ajoutez chartView à votre interface (par exemple dans un layout)

    

    connect(ui->btn, &QPushButton::clicked, this, &Consultationwindow::loadExistingEvaluations);
    connect(ui->btn, &QPushButton::clicked, this, &Consultationwindow::showEvaluationStats);
}

Consultationwindow::~Consultationwindow()
{
    delete ui;
}
void Consultationwindow::onConsultationSelected(int index) {
    if (index < 0) {
        qDebug() << "Invalid index";
        return;
    }

    QVariant idData = ui->consultationComboBox1->itemData(index);
    if (!idData.isValid() || idData.toInt() == 0) {
        qDebug() << "Invalid or placeholder item selected";
        ui->satisfaction->setPixmap(QPixmap());
        ui->satisfaction->setToolTip("");
        return;
    }

    int consultationId = idData.toInt();
    QSqlQuery query;
    query.prepare("SELECT SATISFACATION FROM MOLKA.CONSULTATIONS WHERE ID = :id");
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
            ui->satisfaction->setPixmap(QPixmap());
            ui->satisfaction->setToolTip("");
        }
    }
}
void Consultationwindow::update_label() {
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
                    ui->labelstat->setText(QString("New Evaluation: %1 stars").arg(stars));
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
                ui->labelstat->setText("Evaluation cancelled");
            }
            else if(message == "ARDUINO_READY") {
                ui->labelstat->setText("Arduino connected");
            }
        }
    }
}
void Consultationwindow::loadExistingEvaluations() {
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
    QSqlQuery query("SELECT ID, EVALUATION, SATISFACATION FROM MOLKA.CONSULTATIONS WHERE STATUS = 'Termine'");

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
        if(id == ui->consultationComboBox1->currentData().toInt()) {
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
void Consultationwindow::updateSatisfactionIcon(int satisfied) {
    // Get the currently selected consultation ID
    int currentConsultationId = ui->consultationComboBox1->currentData().toInt();

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
        ui->satisfaction->setToolTip("Customer is satisfied");
        satisfactionDbValue = "satisfait";
    } else {
        iconUrl = ":/img/images/angry.png";
        satisfactionText = tr("Customer is not satisfied");
        ui->satisfaction->setToolTip("Customer is satisfied");
        satisfactionDbValue = "non satisfait";
    }

    // Update UI
    ui->satisfaction->setToolTip(satisfactionText);
    QPixmap pixmap(iconUrl);
    if (!pixmap.isNull()) {
        ui->satisfaction->setPixmap(pixmap.scaled(32, 32, Qt::KeepAspectRatio));
    } else {
        qDebug() << "Failed to load satisfaction icon:" << iconUrl;
    }

    // Update database
    QSqlQuery query;
    query.prepare("UPDATE MOLKA.CONSULTATIONS SET SATISFACATION = :satisfaction WHERE ID = :id");
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
// Modify populateConsultationsComboBox()
void Consultationwindow::populateConsultationsComboBox() {
    ui->consultationComboBox1->clear();

    // Add placeholder item first
    ui->consultationComboBox1->addItem("Select a consultation...", 0);

    QSqlQuery query("SELECT ID, SUJET FROM MOLKA.CONSULTATIONS WHERE STATUS = 'Termine'");
    while (query.next()) {
        int id = query.value("ID").toInt();
        QString sujet = query.value("SUJET").toString();
        ui->consultationComboBox1->addItem(QString("Consultation %1 - %2").arg(id).arg(sujet), id);
    }

    // Disconnect before manual invocation to avoid double triggers
    disconnect(ui->consultationComboBox1, nullptr, this, nullptr);

    // Manually trigger selection changed if items exist
    if (ui->consultationComboBox1->count() > 1) {
        ui->consultationComboBox1->setCurrentIndex(1); // Skip placeholder
    }

    // Reconnect after population
    connect(ui->consultationComboBox1, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &Consultationwindow::onConsultationSelected);

}

void Consultationwindow::handleEvaluationData(int stars) {
    if(stars < 1 || stars > 4) return;  // Validate input


    evaluationCount++;
    starRatings[stars]++;

    // Save to file (keep your current logic)
    saveEvaluationToFile(stars);

    // Get selected consultation ID from combo box
    int selectedConsultationId = ui->consultationComboBox1->currentData().toInt();

    // Save to database
    QSqlQuery query;
    query.prepare("UPDATE MOLKA.CONSULTATIONS SET EVALUATION = :eval WHERE ID = :id");
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
void Consultationwindow::onCsvItemClicked(QListWidgetItem *item) {
    bool ok;
    int stars = item->text().toInt(&ok); // suppose que l’item contient un chiffre
    if (ok && stars >= 1 && stars <= 4) {
        handleEvaluationData(stars);
    }
}


void Consultationwindow::saveEvaluationToFile(int stars)
{
    QFile file("evaluations.csv");
    if(file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")
            << "," << stars << "\n";
        file.close();
    }
}

void Consultationwindow::showEvaluationStats() {
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
    ui->labelstat->setText(
        QString("Total Evaluations: %1\nAverage Rating: %2\n\n"
                "4 stars: %3\n3 stars: %4\n2 stars: %5\n1 star: %6")
            .arg(totalRatings)
            .arg(average, 0, 'f', 1)
            .arg(starRatings.value(4, 0))
            .arg(starRatings.value(3, 0))
            .arg(starRatings.value(2, 0))
            .arg(starRatings.value(1, 0)));
}
void Consultationwindow::on_pushButton_ajouter_Consultation_clicked() {


    QString SUJET = ui->lineEdit_SUJET->text();
    if (SUJET.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le sujet ne peut pas être vide.");
        return;
    }


    QRegularExpression re("^[A-Za-zéèàâêîôûäëïöüç]+$");
    if (!re.match(SUJET).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le sujet doit être uniquement un texte contenant des lettres.");
        return;
    }


    QDate CONSULTATIONDATE = ui->dateEdit->date();
    if (!CONSULTATIONDATE.isValid()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une date valide.");
        return;
    }

    QDateTime consultationDateTime(CONSULTATIONDATE, QTime(0, 0, 0));


    QString HEURECONSULTATION = ui->lineEdit_HEURECONSULTATION->text();
    QTime time = QTime::fromString(HEURECONSULTATION, "HH:mm");

    if (!time.isValid()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une heure valide au format HH:mm.");
        return;
    }


    QString STATUS = ui->comboBox_STATUS->currentText().trimmed();
    if (STATUS != "Planifie" && STATUS != "En cours" && STATUS != "Termine" && STATUS != "Annule") {
        QMessageBox::critical(this, QObject::tr("Erreur"), QObject::tr("Statut invalide! Les valeurs autorisées sont : Planifiée, En cours, Terminée, Annulée."));
        return;
    }


    CONSULTATIONS C(SUJET, consultationDateTime, HEURECONSULTATION, STATUS);


    bool test = C.ajouter();

    if (test) {
        ui->tableView->setModel(Etmp.afficher());
        QMessageBox::information(this, QObject::tr("OK"), QObject::tr("Ajout effectué\nClick OK to exit."));
        CONSULTATIONS consultation;
        consultation.createChart(ui->widgetChart);
    } else {
        QMessageBox::critical(this, QObject::tr("Erreur"), QObject::tr("Ajout non effectué\nClick OK to exit."));
    }

    // Nettoyage de l'ancien contenu du groupBox s'il existe
    if (ui->groupBox_calendrier->layout()) {
        QLayoutItem* child;
        while ((child = ui->groupBox_calendrier->layout()->takeAt(0)) != nullptr) {
            if (child->widget()) {
                child->widget()->setParent(nullptr);
                delete child->widget();
            }
            delete child;
        }
        delete ui->groupBox_calendrier->layout();
    }

    // Création et insertion d'une nouvelle fenêtre de calendrier
    calendarWindow = new Window();
    calendarWindow->setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout *layout = new QVBoxLayout(ui->groupBox_calendrier);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(calendarWindow);
    calendarWindow->setMinimumSize(ui->groupBox_calendrier->size());
    calendarWindow->adjustSize();
}




void Consultationwindow::on_pushButton_supprimer_Consultation_clicked() {


    int id =ui->lineEdit_supprtion_modifier_Consultation->text().toInt();
    QSqlQuery query;
    query.prepare("SELECT SUJET, CONSULTATIONDATE, HEURECONSULTATION, STATUS FROM CONSULTATIONS WHERE ID = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Aucune consultation", "Aucune consultation trouvée avec cet ID !");
        return;
    }else{

        bool test=Etmp.supprimer(id);

        if (test) {
            ui->tableView->setModel(Etmp.afficher());
            QMessageBox::information(this, QObject::tr("OK"), QObject::tr("suppression effectué\nClick OK to exit."));
            CONSULTATIONS consultation;
            consultation.createChart(ui->widgetChart);
        } else {
            QMessageBox::critical(this, QObject::tr("not ok"), QObject::tr("suppression non effectué\nClick OK to exit."));
        }
    }

    if (ui->groupBox_calendrier->layout()) {
        QLayoutItem* child;
        while ((child = ui->groupBox_calendrier->layout()->takeAt(0)) != nullptr) {
            if (child->widget()) {
                child->widget()->setParent(nullptr);
                delete child->widget();
            }
            delete child;
        }
        delete ui->groupBox_calendrier->layout();
    }

    // Création et insertion d'une nouvelle fenêtre de calendrier
    calendarWindow = new Window();
    calendarWindow->setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout *layout = new QVBoxLayout(ui->groupBox_calendrier);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(calendarWindow);
    calendarWindow->setMinimumSize(ui->groupBox_calendrier->size());
    calendarWindow->adjustSize();

}

void Consultationwindow::onModifierConsultation() {

    QString id = ui->lineEdit_supprtion_modifier_Consultation->text();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID !");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT ID, SUJET, CONSULTATIONDATE, HEURECONSULTATION, STATUS FROM CONSULTATIONS WHERE ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Aucune consultation", "Aucune consultation trouvée avec cet ID !");
        return;
    }

    CONSULTATIONS consultation;
    consultation.setID(query.value(0).toInt());
    consultation.setSujet(query.value(1).toString());
    consultation.setConsultationDate(query.value(2).toDateTime());
    consultation.setHeureConsultation(query.value(3).toString());
    consultation.setStatus(query.value(4).toString());

    ModifierConsultationDialog dialog(this);
    dialog.setData(consultation);

    if (dialog.exec() == QDialog::Accepted) {
        CONSULTATIONS modifiedConsultation = dialog.getModifiedConsultation();

        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE CONSULTATIONS SET SUJET = :sujet, CONSULTATIONDATE = TO_DATE(:date, 'YYYY-MM-DD'), HEURECONSULTATION = :heure, STATUS = :status WHERE ID = :id");
        updateQuery.bindValue(":sujet", modifiedConsultation.getSujet());
        updateQuery.bindValue(":date", modifiedConsultation.getConsultationDate().toString("yyyy-MM-dd"));
        updateQuery.bindValue(":heure", modifiedConsultation.getHeureConsultation());
        updateQuery.bindValue(":status", modifiedConsultation.getStatus());
        updateQuery.bindValue(":id", id);

        if (!updateQuery.exec()) {
            QMessageBox::critical(this, "Erreur SQL", updateQuery.lastError().text());
        } else {
            QMessageBox::information(this, "Succès", "Consultation modifiée avec succès !");
            ui->tableView->setModel(Etmp.afficher());
            consultation.createChart(ui->widgetChart);
        }
    }
    if (ui->groupBox_calendrier->layout()) {
        QLayoutItem* child;
        while ((child = ui->groupBox_calendrier->layout()->takeAt(0)) != nullptr) {
            if (child->widget()) {
                child->widget()->setParent(nullptr);
                delete child->widget();
            }
            delete child;
        }
        delete ui->groupBox_calendrier->layout();
    }

    // Création et insertion d'une nouvelle fenêtre de calendrier
    calendarWindow = new Window();
    calendarWindow->setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout *layout = new QVBoxLayout(ui->groupBox_calendrier);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(calendarWindow);
    calendarWindow->setMinimumSize(ui->groupBox_calendrier->size());
    calendarWindow->adjustSize();
}


// Fonction pour exporter le graphique (widgetChart) en fichier PDF
void Consultationwindow::exporterVersPDF()
{
    // Boîte de dialogue pour choisir l’emplacement de sauvegarde du fichier PDF
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter en PDF", "", "PDF Files (*.pdf)");
    if (fileName.isEmpty()) return;

    // Création d’un objet QPdfWriter pour écrire le contenu en PDF
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4)); // Format A4
    pdfWriter.setResolution(300); // Résolution 300 DPI

    QPainter painter(&pdfWriter); // Peintre utilisé pour dessiner dans le PDF
    if (!painter.isActive()) {
        QMessageBox::critical(this, "Erreur", "Échec de l'exportation en PDF !");
        return;
    }

    // Rendu du widget du graphique dans le PDF
    ui->widgetChart->render(&painter);

    painter.end(); // Terminer le dessin

    QMessageBox::information(this, "Succès", "Le fichier PDF a été enregistré avec succès !");
}

// Fonction pour exporter les données de la base (consultations) vers un fichier Excel
void Consultationwindow::exporterVersExcel() {
    // Boîte de dialogue pour choisir l’emplacement du fichier Excel
    QString fileName = QFileDialog::getSaveFileName(this, "Save as", "", "Excel Files (*.xlsx)");
    if (fileName.isEmpty()) return;

    QXlsx::Document xlsx; // Création du document Excel
    QSqlQueryModel *model = Etmp.afficher(); // Récupération des données à exporter

    // Écriture de l’en-tête (ligne des titres de colonnes)
    for (int col = 0; col < model->columnCount(); col++) {
        xlsx.write(1, col + 1, model->headerData(col, Qt::Horizontal).toString());
    }

    // Écriture des lignes de données
    for (int row = 0; row < model->rowCount(); row++) {
        for (int col = 0; col < model->columnCount(); col++) {
            xlsx.write(row + 2, col + 1, model->data(model->index(row, col)).toString());
        }
    }

    // Sauvegarde du fichier Excel et affichage du message
    if (xlsx.saveAs(fileName)) {
        QMessageBox::information(this, "Succès", "Le fichier a été enregistré avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'enregistrement du fichier !");
    }
}

// Fonction pour effectuer une recherche en direct (live search) dans les consultations
void Consultationwindow::rechercherEnDirect(const QString &searchText)
{
    CONSULTATIONS consultation;
    QSqlQueryModel* model = consultation.rechercherConsultation(searchText); // Appel à la fonction de recherche
    ui->tableView->setModel(model); // Mise à jour de l’affichage
}

// Fonction pour trier les consultations selon le critère choisi dans la comboBox
void Consultationwindow::trierTable()
{
    int index = ui->comboBox_tri->currentIndex(); // Récupération de l’option sélectionnée
    QString orderByColumn;
    QString orderType;

    switch (index) {
    case 0: // Trier de A → Z par sujet
        orderByColumn = "SUJET";
        orderType = "ASC";
        break;
    case 1: // Trier de Z → A par sujet
        orderByColumn = "SUJET";
        orderType = "DESC";
        break;
    case 2: // Trier par date de consultation (récent → ancien)
        orderByColumn = "CONSULTATIONDATE";
        orderType = "DESC";
        break;
    case 3: // Trier par date de consultation (ancien → récent)
        orderByColumn = "CONSULTATIONDATE";
        orderType = "ASC";
        break;
    }

    // Mise à jour du tableau avec les données triées
    ui->tableView->setModel(Etmp.afficher(orderByColumn, orderType));
}





void Consultationwindow::colorerIncoherences(QSqlQueryModel *model) {
    CONSULTATIONS consultation;
    QList<int> incoherentes = consultation.detecterIncoherences(model);

    QStandardItemModel *standardModel = new QStandardItemModel();
    standardModel->setColumnCount(model->columnCount() + 1); // Ajouter une colonne pour l'alerte

    // Copier les données du modèle SQL vers un modèle standard
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            QStandardItem *item = new QStandardItem(model->data(model->index(row, col)).toString());
            standardModel->setItem(row, col, item);
        }
    }

    // Mise à jour de la base de données et marquage des incohérences
    QSqlQuery updateQuery;
    for (int row : incoherentes) {
        // Récupérer l'ID de la ligne incohérente
        int consultationId = model->data(model->index(row, 0)).toInt(); // نفترض أن العمود 0 يحتوي على ID

        // Mise à jour du champ STATUS dans la base de données
        updateQuery.prepare("UPDATE CONSULTATIONS SET STATUS = 'Annule' WHERE ID = :id");
        updateQuery.bindValue(":id", consultationId);
        if (!updateQuery.exec()) {
            qDebug() << "Erreur lors de la mise à jour du statut:" << updateQuery.lastError().text();
        }

        // Ajouter l'alerte dans le modèle d'affichage
        QStandardItem *alertItem = new QStandardItem("⚠️ Alerte: L'ordre n'est pas cohérent avec l'état ou la date.\nLe statut a été changé en 'Annulé'.");
        standardModel->setItem(row, model->columnCount(), alertItem);

        // Mise à jour du modèle pour refléter la modification de l'état
        for (int col = 0; col < model->columnCount(); ++col) {
            // Vérifier si la colonne est le statut
            if (col == 4) { // فرضنا أن العمود 4 يحتوي على الـ STATUS
                standardModel->setItem(row, col, new QStandardItem("Annule"));
            }
        }
    }

    // Affichage du modèle dans le tableau et ajustement des colonnes
    ui->tableView_priorisation->setModel(standardModel);
    ui->tableView_priorisation->resizeColumnToContents(standardModel->columnCount() - 1);
    ui->tableView->setModel(model);
    // Afficher un message d'avertissement s'il y a des incohérences
    if (!incoherentes.isEmpty()) {
        QMessageBox *msgBox = new QMessageBox(this);
        msgBox->setWindowTitle("⚠️ Alerte de cohérence");
        msgBox->setIcon(QMessageBox::Warning);
        msgBox->setText("Des incohérences ont été détectées entre l'état et la date de certaines consultations.");
        msgBox->setInformativeText("Le statut a été automatiquement changé en 'Annulé'. Veuillez vérifier les lignes concernées.");
        msgBox->setStandardButtons(QMessageBox::Ok);
        msgBox->setDefaultButton(QMessageBox::Ok);
        msgBox->setStyleSheet("QLabel{min-width: 400px; min-height: 60px; font-size: 14px;}");

        msgBox->exec();
    }
}

// Bouton de priorisation des consultations
void Consultationwindow::on_prioriserButton_clicked() {
    CONSULTATIONS consultation;
    QSqlQueryModel *model = nullptr;

    // Vérifier quel critère de tri est sélectionné
    if (ui->radioButton_1->isChecked()) {
        model = consultation.afficherPriorisationParStatus();
    } else if (ui->radioButton_2->isChecked()) {
        model = consultation.afficherPriorisationParDate();
    } else if (ui->radioButton_3->isChecked()) {
        model = consultation.afficherPriorisationMixte();
    }

    // Appliquer le modèle et détecter les incohérences
    if (model != nullptr) {
        colorerIncoherences(model);
    } else {
        qDebug() << "Aucun modèle de priorisation n'a été sélectionné.";
    }
}

// Suppression des consultations ayant le statut "Annulé"
void Consultationwindow::on_pushButton_supprimerAnnule_clicked() {
    QSqlQuery query;
    if (query.exec("DELETE FROM CONSULTATIONS WHERE STATUS = 'Annule'")) {
        QMessageBox::information(this, "Suppression réussie",
                                 "✅ Toutes les consultations annulées ont été supprimées avec succès.");

        // Rafraîchir la table de priorisation après suppression
        on_prioriserButton_clicked();
    } else {
        QMessageBox::critical(this, "Erreur",
                              "❌ Une erreur est survenue lors de la suppression.");
    }
}

// Affichage du calendrier dans la section prévue
void Consultationwindow::on_pushButton_calendrier_clicked()
{
    // Nettoyage de l'ancien contenu du groupBox s'il existe
    if (ui->groupBox_calendrier->layout()) {
        QLayoutItem* child;
        while ((child = ui->groupBox_calendrier->layout()->takeAt(0)) != nullptr) {
            if (child->widget()) {
                child->widget()->setParent(nullptr);
                delete child->widget();
            }
            delete child;
        }
        delete ui->groupBox_calendrier->layout();
    }

    // Création et insertion d'une nouvelle fenêtre de calendrier
    calendarWindow = new Window();
    calendarWindow->setAttribute(Qt::WA_DeleteOnClose);

    QVBoxLayout *layout = new QVBoxLayout(ui->groupBox_calendrier);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(calendarWindow);
    calendarWindow->setMinimumSize(ui->groupBox_calendrier->size());
    calendarWindow->adjustSize();
}
