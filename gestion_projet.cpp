#include "gestion_projet.h"
#include "ui_gestion_projet.h"
#include "projet.h"
#include <QDebug>
#include <QMessageBox>
#include <QDate>
#include <QSqlError>
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QtCharts/QChartView> // Inclure QtCharts pour les graphiques
#include <QQmlContext>
#include <QGeoCoordinate>
#include <QPrinter>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QPageSize>  // Add this include
#include <QMarginsF>  // Add this include
#include <QPdfWriter>
#include <QStandardPaths>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QFontDatabase>
#include <QScopedPointer>
#include <QSqlQueryModel>
#include <QGeoPositionInfoSource>
#include <QQuickView>
#include <QQuickItem>
#include <QGeoCoordinate>
#include <QMetaObject>


gestion_projet::gestion_projet(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::gestion_projet)
{
    ui->setupUi(this);
    this->setStyleSheet(
        "QPushButton {"
        "    background-color: #3498db;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 4px;"
        "    padding: 8px 16px;"
        "    font-weight: bold;"
        "}"
        ""
        "QPushButton:hover {"
        "    background-color: #2980b9;"
        "}"
        ""
        "QPushButton:pressed {"
        "    background-color: #1f618d;"
        "}"
        ""
        "QGroupBox {"
        "    border: 1px solid #bdc3c7;"
        "    border-radius: 5px;"
        "    margin-top: 10px;"
        "    padding-top: 15px;"
        "    font-weight: bold;"
        "    color: #2c3e50;"
        "}"
        ""
        "QComboBox {"
        "    border: 1px solid #bdc3c7;"
        "    border-radius: 3px;"
        "    padding: 5px;"
        "    background-color: white;"
        "}"
        ""
        "QComboBox:hover {"
        "    border: 1px solid #3498db;"
        "}"
        ""
        "QComboBox::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"
        "    width: 20px;"
        "    border-left: 1px solid #bdc3c7;"
        "}"
        ""
        "QComboBox QAbstractItemView {"
        "    background-color: white;"
        "    border: 1px solid #bdc3c7;"
        "    selection-background-color: #3498db;"
        "    selection-color: white;"
        "}"
        ""
        ""
        "QTableWidget::item {"
        "    padding: 5px;"
        "}"
        ""
        "QTableWidget::item:selected {"
        "    background-color: #3498db;"
        "    color: white;"
        "}"
        ""
        "QCheckBox {"
        "    spacing: 5px;"
        "}"
        ""
        "QCheckBox::indicator {"
        "    width: 16px;"
        "    height: 16px;"
        "}"
        ""
        "QCheckBox::indicator:checked {"
        "    background-color: #3498db;"
        "    border: 1px solid #2980b9;"
        "}"
        ""
        "QCheckBox::indicator:unchecked {"
        "    background-color: white;"
        "    border: 1px solid #bdc3c7;"
        "}"
        ""
        "QDateEdit {"
        "    border: 1px solid #bdc3c7;"
        "    border-radius: 3px;"
        "    padding: 5px;"
        "    background-color: white;"
        "}"
        ""
        "QDateEdit::drop-down {"
        "    subcontrol-origin: padding;"
        "    subcontrol-position: top right;"
        "    width: 20px;"
        "    border-left: 1px solid #bdc3c7;"
        "}"
        ""
        "QScrollBar:vertical {"
        "    background: #ecf0f1;"
        "    width: 10px;"
        "}"
        ""
        "QScrollBar::handle:vertical {"
        "    background: #bdc3c7;"
        "    min-height: 20px;"
        "}"
        ""
        "QScrollBar::handle:vertical:hover {"
        "    background: #95a5a6;"
        "}"
        ""
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    background: none;"
        "}"
        );
    initializeChatbotResponses();
    openAIKey = "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";
    stabilityAIKey = "sk-2x7fae7jYwzed6nG6kvSrSldhh2x8igcZWvXhpjVSxkwpU7F";
    networkManager = new QNetworkAccessManager(this);

    // Connecter les boutons aux slots
    //connect(ui->UploadPDF, &QPushButton::clicked, this, &MainWindow::on_UploadPDF_clicked);
    connect(ui->OuvrirPDF, &QPushButton::clicked, this, &gestion_projet::on_OuvrirPDF_clicked);
    connect(ui->etatComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &gestion_projet::on_etatComboBox_changed);
    connect(ui->rechercherButton, &QPushButton::clicked, this, &gestion_projet::on_rechercherButton_clicked);
    connect(ui->rechercheLineEdit, &QLineEdit::textChanged, this, &gestion_projet::on_rechercheLineEdit_textChanged);
    connect(ui->stat, &QPushButton::clicked, this, &gestion_projet::on_stat_clicked);

    connect(ui->messageInput, &QLineEdit::returnPressed, this, &gestion_projet::onUserMessageSent);

    connect(ui->send, &QPushButton::clicked, this, &gestion_projet::onUserMessageSent);
    connect(ui->btnExportPDF, &QPushButton::clicked, this, &gestion_projet::genererPDFProjets);
    connect(ui->projectNameComboBox_3, QOverload<int>::of(&QComboBox::currentIndexChanged),this, &gestion_projet::on_projectNameComboBox_currentIndexChanged);


    connect(ui->ajouter_2, &QPushButton::clicked, this, &gestion_projet::on_ajouter_clicked);


    // Ouvrir la connexion à la base de données
    if (cnx.createconnect()) {
        qDebug() << "Database connection successful.";
        afficher("Tous"); // Charger tous les projets au démarrage
    } else {
        QMessageBox::critical(this, "Error", "Failed to connect to the database.");
    }

    ui->mapWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->mapWidget->setSource(QUrl("qrc:/new/image/icons/map.qml"));
    if (ui->mapWidget->status() == QQuickWidget::Error) {
        qDebug() << "QML Error:" << ui->mapWidget->errors();
    }
    if (!ui->mapWidget->rootObject()) {
        qDebug() << "QML failed to load!";
    }
    ui->mapWidget->setClearColor(Qt::transparent);

    if (ui->mapWidget->layout()) {
        QLayout *layout = ui->mapWidget->layout();

        // Clear the layout safely
        QLayoutItem *child;
        while ((child = layout->takeAt(0)) != nullptr) {
            // Only delete the widget if it exists and isn't already deleted
            if (child->widget()) {
                child->widget()->setParent(nullptr); // Remove from layout
                child->widget()->deleteLater();     // Schedule for deletion
            }
            delete child; // Delete the layout item
        }
    }



    // Load project names into the combo box
    loadProjectNames();

    // Connect signals

    // In gestion_projet constructor:
    int connectionStatus = arduino.connect_arduino("COM5");  // Force COM5 for gestion_projet
    if (connectionStatus == 0) {
        ui->statusLabel->setText("Connected to Arduino on COM5");
        connect(arduino.getserial(), &QSerialPort::readyRead,
                this, &gestion_projet::checkKeypadInput);
    } else {
        ui->statusLabel->setText("Failed to connect to COM5");
    }


    QTimer *keypadTimer = new QTimer(this);
    connect(keypadTimer, &QTimer::timeout, this, &gestion_projet::checkKeypadInput);
    keypadTimer->start(100); // Check every 100ms


}


gestion_projet::~gestion_projet() {
    if (arduino.getserial() && arduino.getserial()->isOpen()) {
        arduino.getserial()->disconnect();
        arduino.close_arduino();
    }
}

void gestion_projet::genererPDFProjets() {
    // 1. Configuration du fichier
    QString fullPath = QDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation))
                           .filePath(QString("Rapport_Projets_%1.pdf")
                                         .arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss")));

    // 2. Initialisation du PDF
    QPdfWriter writer(fullPath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(96);
    writer.setTitle("Rapport des Projets");

    // Solution pour le numéro de page - variable manuelle
    int currentPageNumber = 1;

    // 3. Initialisation du painter
    QPainter painter(&writer);
    if (!painter.isActive()) {
        QMessageBox::critical(this, "Erreur", "Échec d'initialisation du générateur PDF");
        return;
    }

    try {
        // Paramètres de mise en page
        const int margin = 20;
        const int pageWidth = writer.width();
        const int pageHeight = writer.height();
        int yPos = margin;

        // 4. Configuration des polices
        QFont titleFont("Arial", 20, QFont::Bold);
        QFont headerFont("Arial", 11, QFont::Bold);
        QFont dataFont("Arial", 10);
        QFont infoFont("Arial", 9);

        // 5. En-tête
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

        // Titre
        painter.setFont(titleFont);
        painter.drawText(QRect(0, yPos, pageWidth, 50),
                         Qt::AlignCenter, QString::fromUtf8(u8"RAPPORT COMPLET DES PROJETS"));
        yPos += 50;

        // Informations
        painter.setFont(infoFont);
        QString infoText = QString::fromUtf8(u8"Généré le %1 | www.votresite.com")
                               .arg(QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm"));
        painter.drawText(QRect(margin, yPos, pageWidth - 2*margin, 20),
                         Qt::AlignCenter, infoText);
        yPos += 30;

        // 6. Récupération des données
        Projet P;
        QScopedPointer<QSqlQueryModel> model(P.afficher("Tous"));
        if (!model || model->rowCount() == 0) {
            painter.setFont(dataFont);
            painter.drawText(QRect(0, yPos, pageWidth, 40),
                             Qt::AlignCenter, QString::fromUtf8(u8"Aucun projet disponible"));
            painter.end();
            QMessageBox::information(this, "Information", "Aucun projet à exporter");
            return;
        }

        // 7. Configuration des colonnes
        const int colCount = 6;
        int colWidths[colCount] = {
            (pageWidth - 2*margin) * 0.07,  // ID
            (pageWidth - 2*margin) * 0.18,  // Nom
            (pageWidth - 2*margin) * 0.35,  // Description
            (pageWidth - 2*margin) * 0.15,  // Début
            (pageWidth - 2*margin) * 0.15,  // Fin
            (pageWidth - 2*margin) * 0.10   // État
        };

        // 8. En-tête du tableau
        painter.setFont(headerFont);
        painter.setBrush(QColor(50, 100, 150));
        painter.setPen(Qt::NoPen);
        painter.drawRect(margin, yPos, pageWidth - 2*margin, 30);
        painter.setPen(Qt::white);

        QStringList headers = {
            QString::fromUtf8(u8"ID"),
            QString::fromUtf8(u8"NOM"),
            QString::fromUtf8(u8"DESCRIPTION"),
            QString::fromUtf8(u8"DÉBUT"),
            QString::fromUtf8(u8"FIN"),
            QString::fromUtf8(u8"ÉTAT")
        };

        int xPos = margin;
        for (int i = 0; i < colCount; ++i) {
            painter.drawText(QRect(xPos, yPos, colWidths[i], 30),
                             Qt::AlignCenter, headers[i]);
            xPos += colWidths[i];
        }
        yPos += 35;

        // 9. Données des projets
        painter.setFont(dataFont);
        painter.setPen(Qt::black);
        const int rowHeight = 25;

        for (int row = 0; row < model->rowCount(); ++row) {
            painter.setBrush(row % 2 ? QColor(245, 245, 245) : Qt::white);
            painter.setPen(Qt::NoPen);
            painter.drawRect(margin, yPos, pageWidth - 2*margin, rowHeight);
            painter.setPen(Qt::black);

            xPos = margin;
            for (int col = 0; col < colCount; ++col) {
                QString text = model->data(model->index(row, col)).toString();

                if (col == 3 || col == 4) {
                    QDate date = QDate::fromString(text, "yyyy-MM-dd");
                    if (date.isValid()) text = date.toString("dd/MM/yy");
                }

                painter.drawText(QRect(xPos + 5, yPos, colWidths[col] - 10, rowHeight),
                                 Qt::AlignLeft | Qt::TextWordWrap, text);
                xPos += colWidths[col];
            }
            yPos += rowHeight;

            // Gestion des sauts de page
            if (yPos > pageHeight - margin - rowHeight && row < model->rowCount() - 1) {
                // Pied de page avant saut
                painter.setFont(infoFont);
                painter.drawText(QRect(margin, pageHeight - 30, pageWidth - 2*margin, 20),
                                 Qt::AlignRight, QString::fromUtf8(u8"Page %1").arg(currentPageNumber++));

                writer.newPage();
                yPos = margin;

                // En-tête sur nouvelle page
                painter.setFont(headerFont);
                painter.setBrush(QColor(50, 100, 150));
                painter.setPen(Qt::NoPen);
                painter.drawRect(margin, yPos, pageWidth - 2*margin, 30);
                painter.setPen(Qt::white);

                xPos = margin;
                for (int i = 0; i < colCount; ++i) {
                    painter.drawText(QRect(xPos, yPos, colWidths[i], 30),
                                     Qt::AlignCenter, headers[i]);
                    xPos += colWidths[i];
                }
                yPos += 35;
                painter.setFont(dataFont);
                painter.setPen(Qt::black);
            }
        }

        // 10. Pied de page final
        painter.setFont(infoFont);
        painter.drawText(QRect(margin, pageHeight - 30, pageWidth - 2*margin, 20),
                         Qt::AlignRight, QString::fromUtf8(u8"Page %1").arg(currentPageNumber));

    } catch (...) {
        painter.end();
        QFile::remove(fullPath);
        QMessageBox::critical(this, "Erreur", "Erreur lors de la génération du PDF");
        return;
    }

    // 11. Finalisation
    painter.end();

    // 12. Vérification et ouverture
    QFileInfo fileInfo(fullPath);
    if (fileInfo.size() < 100) {
        QFile::remove(fullPath);
        QMessageBox::critical(this, "Erreur", "Le fichier PDF généré est invalide");
        return;
    }

    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(fullPath))) {
        QMessageBox::information(this, "Succès",
                                 QString("PDF généré avec succès\nEmplacement : %1").arg(fullPath));
    }
}
void gestion_projet::initializeChatbotResponses() {
    responses["hello"] = {"Hi there!", "Hello!", "Greetings!"};
    responses["how are you"] = {"I'm just a program, but I'm functioning well!", "All systems operational!"};
    responses["bye"] = {"Goodbye!", "See you later!", "Bye bye!"};
    responses["bonjour"] = {
        "Bonjour ! Bienvenue dans StartWise Assistant, votre outil de gestion de projets startup.",
        "Salut ! Prêt à booster votre gestion de projet ?",
        "Bonjour entrepreneur ! Comment puis-je vous aider aujourd'hui ?"
    };

    responses["gestion de projet"] = {
        "Chez StartWise, nous recommandons la méthode agile pour les startups.",
        "La gestion de projet efficace commence par une bonne planification des sprints.",
        "Voulez-vous des conseils sur Trello, Asana ou Jira pour votre startup ?"
    };

    responses["startup"] = {
        "Le succès d'une startup repose sur 3 piliers : équipe, produit, marché !",
        "Avez-vous établi votre MVP (Minimum Viable Product) ?",
        "StartWise propose des templates spécialisés pour les jeunes pousses."
    };

    responses["financement"] = {
        "Les options : bootstrapping, love money, business angels, VC...",
        "Avez-vous préparé votre pitch deck pour les investisseurs ?",
        "StartWise peut vous aider à modéliser votre prévisionnel financier."
    };

    responses["équipe"] = {
        "Dans une startup, privilégiez les profils polyvalents et autonomes.",
        "Astuce : utilisez des OKR (Objectives and Key Results) pour aligner votre équipe.",
        "StartWise propose des outils de gestion des ressources humaines adaptés."
    };

    responses["méthodologie"] = {
        "Scrum est idéal pour les projets à forte incertitude.",
        "Kanban fonctionne bien pour les petites équipes startups.",
        "Le Lean Startup permet de valider rapidement vos hypothèses marché."
    };

    responses["outils"] = {
        "Top 3 outils StartWise : 1) Notion 2) Slack 3) Figma",
        "Nous avons un comparatif complet des outils SaaS pour startups.",
        "Voulez-vous des templates gratuits pour votre business plan ?"
    };

    responses["échec"] = {
        "90% des startups échouent, mais chaque échec est un apprentissage !",
        "Pivot ou persévérer ? Analysons vos metrics ensemble.",
        "StartWise propose un module post-mortem pour tirer les leçons."
    };

    responses["croissance"] = {
        "Focus sur l'ACV (Average Customer Value) et le CAC (Customer Acquisition Cost).",
        "Avez-vous identifié vos canaux d'acquisition les plus efficaces ?",
        "Notre module Growth Hacking peut vous aider à scaler."
    };

    responses["merci"] = {
        "Avec plaisir ! N'hésitez pas si vous avez d'autres questions sur StartWise.",
        "Merci à vous ! Bonne continuation avec votre projet entrepreneurial.",
        "Ravi de vous aider. Passez à l'action maintenant !"
    };

}

QString gestion_projet::getChatbotResponse(const QString &message) {


    QString lowerMsg1 = message.toLower().trimmed();

    // Check if message is a client ID (numeric)
    bool isNumeric;
    int clientId = lowerMsg1.toInt(&isNumeric);

    if (isNumeric) {
        QString projects = getClientProjects(clientId);
        arduino.write_to_arduino(projects.toUtf8() + "\n");
        return QString("Displaying projects for client %1 on LCD").arg(clientId);
    }
    // Better cleaning: remove ALL punctuation + accents
    QString lowerMsg = message.toLower()
                           .normalized(QString::NormalizationForm_KD)  // Handle accents
                           .remove(QRegularExpression("[^a-z0-9\\s]")); // Keep only letters/numbers

    // 1. Exact match (strict)
    if (responses.contains(lowerMsg.trimmed())) {
        return responses[lowerMsg].at(QRandomGenerator::global()->bounded(responses[lowerMsg].size()));
    }

    // 2. Substring match (relaxed)
    for (const auto &[pattern, replies] : responses.asKeyValueRange()) {
        if (lowerMsg.contains(pattern)) {  // Simple contains
            return replies.at(QRandomGenerator::global()->bounded(replies.size()));
        }
    }

    // Fallback
    return "Je ne suis pas sûr de comprendre.";
}

void gestion_projet::onUserMessageSent() {
    QString userMessage;

    // Check for keypad input first
    if(arduino.getserial()->bytesAvailable()) {
        userMessage = arduino.read_from_arduino().trimmed();
        ui->messageInput->setText(userMessage); // Display in input field
    }
    else {
        // Original keyboard input handling
        userMessage = ui->messageInput->text().trimmed();
    }

    if(userMessage.isEmpty()) return;

    // Display user message in chat
    ui->chatDisplay->append("You: " + userMessage);

    bool isNumeric;
    int clientId = userMessage.toInt(&isNumeric);
    QString botResponse;

    if (isNumeric) {
        QString clientInfo = getClientInfo(clientId);
        botResponse = "Showing info for client " + QString::number(clientId);

        // Send to Arduino (format: "Client Name|project1, project2")
        QByteArray data = clientInfo.toUtf8() + "\n";
        arduino.write_to_arduino(data);

        qDebug() << "Sent to Arduino:" << clientInfo;
    }
    else {
        botResponse = getChatbotResponse(userMessage);
    }

    // Display bot response
    ui->chatDisplay->append("Bot: " + botResponse);

    // Auto-scroll and clear input
    QTextCursor cursor = ui->chatDisplay->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->chatDisplay->setTextCursor(cursor);

    // Only clear if the message came from the input field (not keypad)
    if(!arduino.getserial()->bytesAvailable()) {
        ui->messageInput->clear();
    }
    ui->messageInput->setFocus();
}


void gestion_projet::checkKeypadInput() {
    QSerialPort *serial = arduino.getserial();
    if(serial && serial->isOpen() && serial->bytesAvailable()) {
        QString keypadInput = arduino.read_from_arduino().trimmed();
        if(!keypadInput.isEmpty()) {
            ui->messageInput->setText(ui->messageInput->text() + keypadInput);
        }
    }
}

void gestion_projet::on_sendButton_clicked()
{    QString message = ui->messageInput->text();
    QByteArray data = message.toUtf8() + "\n";

    qDebug() << "Sending:" << data;
    arduino.write_to_arduino(data);

    // Check if data was actually written
    qDebug() << "Bytes written:" << arduino.getserial()->bytesToWrite();
}

QString gestion_projet::getClientProjects(int clientId) {
    QSqlQuery query;
    query.prepare("SELECT nom FROM PROJETS WHERE CLIENT_ID = :clientId");
    query.bindValue(":clientId", clientId);

    if (!query.exec()) {
        qDebug() << "Query error:" << query.lastError().text();
        return "DB Error";
    }

    if (!query.next()) {
        return QString("No projects");
    }

    QString result;
    do {
        result += query.value(0).toString() + "\n"; // Only project name
    } while (query.next());

    return result.trimmed(); // Remove trailing newline
}

QString gestion_projet::getClientInfo(int clientId) {
    // Get client name
    QSqlQuery clientQuery;
    clientQuery.prepare("SELECT nom, prenom FROM CLIENTS WHERE ID = :clientId");
    clientQuery.bindValue(":clientId", clientId);

    if (!clientQuery.exec() || !clientQuery.next()) {
        return "Unknown Client|No projects"; // Format for Arduino parsing
    }

    QString fullName = clientQuery.value(0).toString() + " " + clientQuery.value(1).toString();

    // Get projects
    QSqlQuery projectQuery;
    projectQuery.prepare("SELECT nom FROM PROJETS WHERE CLIENT_ID = :clientId");
    projectQuery.bindValue(":clientId", clientId);

    QString projects;
    if (projectQuery.exec()) {
        while (projectQuery.next()) {
            projects += projectQuery.value(0).toString() + ", ";
        }
        projects.chop(2); // Remove trailing ", "
    }

    if (projects.isEmpty()) {
        return fullName + "|No projects"; // Special format Arduino will recognize
    }

    return fullName + "|" + projects;
}
void gestion_projet::afficher(const QString& sortOrder) {
    Projet P;
    QSqlQueryModel* model = P.afficher(sortOrder);

    if (model != nullptr) {
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
    } else {
        QMessageBox::warning(this, "Erreur", "Impossible de charger les projets.");
    }
}
void gestion_projet::on_ajouter_clicked()
{
    // Vérifier si les champs sont remplis
    if (ui->name->text().isEmpty() || ui->description->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }

    // Vérifier si un PDF a été uploadé
    if (pdfFilePath.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un fichier PDF.");
        return;
    }
    QString locationName = ui->localisation_2->text();
    QGeoCoordinate coordinates = geocodeLocation(locationName);

    if (!coordinates.isValid()) {
        QMessageBox::warning(this, "Error", "Could not determine coordinates for this location");
        return;
    }

    // Récupérer les données du formulaire
    QString nom = ui->name->text();
    QString description = ui->description->text();
    QString etat = ui->etat_5->isChecked() ? "Traité" : "En cours";
    QDate dateDebut = ui->dated->date();
    QDate dateFin = ui->datef->date();
    QString localisation = ui->localisation->text();


    // Valider les dates
    if (dateFin < dateDebut) {
        QMessageBox::warning(this, "Erreur", "La date de fin ne peut pas être avant la date de début.");
        return;
    }

    // Enregistrer le projet dans la base de données
    QSqlQuery query;
    query.prepare("INSERT INTO PROJETS (nom, description, localisation, latitude, longitude, etat, date_debut, date_fin, pdf_path) "
                  "VALUES (:nom, :description, :localisation, :latitude, :longitude, :etat, :date_debut, :date_fin, :pdf_path)");
    query.bindValue(":nom", nom);
    query.bindValue(":description", description);
    query.bindValue(":etat", etat);
    query.bindValue(":date_debut", dateDebut);
    query.bindValue(":date_fin", dateFin);
    query.bindValue(":pdf_path", pdfFilePath);
    query.bindValue(":localisation", localisation);
    query.bindValue(":latitude", coordinates.latitude());
    query.bindValue(":longitude", coordinates.longitude());


    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Projet ajouté avec succès.");
        afficher("Tous"); // Rafraîchir la liste des projets
        refreshMap();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout du projet.");
    }
}

void gestion_projet::on_supprimer_clicked()
{
    QModelIndex index = ui->tableView->selectionModel()->currentIndex();
    if (!index.isValid())
    {
        QMessageBox::warning(this, "Selection Required", "Please select a project to delete.", QMessageBox::Ok);
        return;
    }

    int id = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirmation", "Are you sure you want to delete this project?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        Projet P;
        bool success = P.supprimer(id);

        if (success)
        {
            QMessageBox::information(this, "Success", "Project deleted.", QMessageBox::Ok);
            afficher("Tous");  // Refresh after deletion
            refreshMap();
        }
        else
        {
            QMessageBox::critical(this, "Error", "Failed to delete project.", QMessageBox::Ok);
        }
    }
}

void gestion_projet::on_modifier_clicked()
{
    static int idProjectToModify = -1;  // Variable to store the ID of the project being modified

    // Check if a project is being modified
    if (idProjectToModify == -1) {
        // Step 1: Select a project for modification
        QModelIndex index = ui->tableView->selectionModel()->currentIndex();
        if (!index.isValid()) {
            QMessageBox::warning(this, "Selection Required", "Please select a project to modify.", QMessageBox::Ok);
            return;
        }

        // Retrieve the selected project's ID
        idProjectToModify = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 0)).toInt();
        QString name = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 1)).toString();
        QString description = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 2)).toString();
        QDate startDate = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 3)).toDate();
        QDate endDate = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 4)).toDate();
        QString status = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 5)).toString();
        QString localisation = ui->tableView->model()->data(ui->tableView->model()->index(index.row(), 6)).toString();


        // Fill the fields with the fetched data
        ui->name->setText(name);
        ui->description->setText(description);
        ui->dated->setDate(startDate);
        ui->datef->setDate(endDate);
        ui->etat_5->setChecked(status == "Traité");
        ui->localisation->setText(localisation);

        // Change the button text to indicate modification
        ui->modifier->setText("Confirm Modification");

    } else {
        // Step 2: Confirm modifications
        QString name = ui->name->text();
        QString description = ui->description->text();
        QDate startDate = ui->dated->date();
        QDate endDate = ui->datef->date();
        QString status = ui->etat_5->isChecked() ? "Traité" : "En cours";
        QString localisation =ui->localisation->text();
        // Validate fields
        if (name.isEmpty() || description.isEmpty()) {
            QMessageBox::warning(this, "Invalid Input", "Name and description must be filled in.", QMessageBox::Ok);
            return;
        }

        if (endDate < startDate) {
            QMessageBox::warning(this, "Invalid Date", "End date cannot be before start date.", QMessageBox::Ok);
            return;
        }

        Projet P(name, description, status,localisation, startDate, endDate);
        bool success = P.modifier(idProjectToModify);

        if (success) {
            QMessageBox::information(this, "Success", "Project modified successfully.", QMessageBox::Ok);
            afficher("Tous");  // Refresh the table after modification
            refreshMap();

            // Reset the form
            ui->name->clear();
            ui->description->clear();
            ui->etat_5->setChecked(false);
            ui->dated->setDate(QDate::currentDate());
            ui->datef->setDate(QDate::currentDate());
            ui->localisation->clear();


            // Reset the ID of the project being modified
            idProjectToModify = -1;

            // Change button text back to original
            ui->modifier->setText("Modify");
        } else {
            QMessageBox::critical(this, "Error", "Failed to modify project.", QMessageBox::Ok);
        }
    }
}

void gestion_projet::on_tableView_activated(const QModelIndex &index)
{
    if (!index.isValid()) return;

    int id = index.model()->data(index.model()->index(index.row(), 0)).toInt();

    QMessageBox::information(this, "Selected Project", QString("Project ID: %1").arg(id));
}

void gestion_projet::on_UploadPDF_clicked()
{
    pdfFilePath = QFileDialog::getOpenFileName(this, "Sélectionner un PDF", "", "Fichiers PDF (*.pdf)");
}

void gestion_projet::on_OuvrirPDF_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un projet.");
        return;
    }

    // Récupérer le chemin du PDF depuis la base de données
    QString pdfPath = index.model()->data(index.model()->index(index.row(), 6)).toString(); // Colonne 6 = pdf_path

    if (pdfPath.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun PDF associé à ce projet.");
        return;
    }

    // Vérifier si le fichier existe
    if (!QFile::exists(pdfPath)) {
        QMessageBox::warning(this, "File Not Found", "The PDF file does not exist at the specified path.", QMessageBox::Ok);
        return;
    }

    // Ouvrir le PDF dans un lecteur externe
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(pdfPath))) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le PDF. Vérifiez que vous avez un lecteur PDF installé.");
    }
}

void gestion_projet::on_etatComboBox_changed(int index) {
    QString selectedText = ui->etatComboBox->itemText(index);
    QString sortOrder = "";

    if (selectedText.contains("ASC")) {
        sortOrder = "ASC";
    }
    else if (selectedText.contains("DESC")) {
        sortOrder = "DESC";
    }

    qDebug() << "Tri sélectionné:" << sortOrder;
    afficher(sortOrder);
}

void gestion_projet::on_rechercherButton_clicked() {
    QString nom = ui->rechercheLineEdit->text(); // Récupérer le nom saisi par l'utilisateur

    if (nom.isEmpty()) {
        // Si le champ de recherche est vide, afficher tous les projets
        afficher("ASC");
        return;
    }

    Projet P;
    QSqlQueryModel* model = P.rechercherParNom(nom); // Appeler la méthode de recherche

    if (model != nullptr) {
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();
        qDebug() << "Recherche effectuée avec succès.";
    } else {
        qDebug() << "Erreur: Le modèle est NULL. Vérifiez la requête SQL.";
        QMessageBox::warning(this, "Erreur", "Aucun projet trouvé avec ce nom.");
        delete model;
    }
}

void gestion_projet::on_rechercheLineEdit_textChanged(const QString& text) {
    if (text.isEmpty()) {
        // Si le champ de recherche est vide, afficher tous les projets
        afficher("ASC");

    }
}

void gestion_projet::on_stat_clicked() {
    QChartView* chartView = projet.createStatisticsChart(this); // Create chart
    chartView->setRenderHint(QPainter::Antialiasing);          // Smooth rendering
    chartView->resize(800, 600);                               // Resize chart

    // Display in a QDialog
    QDialog* chartWindow = new QDialog(this);
    QVBoxLayout* layout = new QVBoxLayout(chartWindow);        // Add a layout
    layout->addWidget(chartView);                              // Add chart to layout
    chartWindow->setWindowTitle("Statistiques des projets");
    chartWindow->resize(800, 600);
    chartWindow->show();
    qDebug() << "nesnes" ;
}
void gestion_projet::loadProjectNames()
{
    QSqlQuery query("SELECT id, nom FROM PROJETS ORDER BY nom");
    ui->projectNameComboBox_3->clear();
    ui->projectNameComboBox_3->addItem("Select a project", -1);

    while (query.next()) {
        ui->projectNameComboBox_3->addItem(query.value(1).toString(), query.value(0));
    }
}

void gestion_projet::on_searchLocationButton_clicked()
{
    QString projectName = ui->projectNameComboBox_3->currentText();
    if (projectName == "Select a project") {
        QMessageBox::warning(this, "Warning", "Please select a project first.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT latitude, longitude FROM PROJETS WHERE nom = :name");
    query.bindValue(":name", projectName);

    if (!query.exec() || !query.next()) {
        QMessageBox::critical(this, "Error", "Could not retrieve project location.");
        return;
    }

    double lat = query.value(0).toDouble();
    double lon = query.value(1).toDouble();

    // Center the map on this location
    QMetaObject::invokeMethod(ui->mapWidget->rootObject(), "centerMap",
                              Q_ARG(QVariant, lat),
                              Q_ARG(QVariant, lon));

    // Add a marker
    QMetaObject::invokeMethod(ui->mapWidget->rootObject(), "addMarker",
                              Q_ARG(QVariant, lat),
                              Q_ARG(QVariant, lon),
                              Q_ARG(QVariant, projectName));
}

void gestion_projet::on_projectNameComboBox_currentIndexChanged(int index)
{
    if (index <= 0) return; // Skip the "Select a project" item

    // Get the project ID from the combo box data
    int projectId = ui->projectNameComboBox_3->currentData().toInt();

    // Fetch project details including coordinates
    QSqlQuery query;
    query.prepare("SELECT nom, latitude, longitude FROM PROJETS WHERE id = :id");
    query.bindValue(":id", projectId);

    if (!query.exec() || !query.next()) {
        qDebug() << "Failed to fetch project location:" << query.lastError().text();
        return;
    }

    if (!ui->mapWidget || !ui->mapWidget->rootObject()) {
        qDebug() << "Map not ready yet!";
        return;
    }

    QString projectName = query.value(0).toString();
    double latitude = query.value(1).toDouble();
    double longitude = query.value(2).toDouble();

    // Update the location label
    ui->locationLabel->setText(QString("Selected: %1 (%2, %3)")
                                   .arg(projectName)
                                   .arg(latitude)
                                   .arg(longitude));

    // Center the map on these coordinates
    if (ui->mapWidget && ui->mapWidget->rootObject()) {
        // Use mapWidget instead of ui->mapWidget
        QMetaObject::invokeMethod(ui->mapWidget->rootObject(), "centerMap",
                                  Q_ARG(QVariant, latitude),
                                  Q_ARG(QVariant, longitude));

        // Clear existing markers and add new one
        QMetaObject::invokeMethod(ui->mapWidget->rootObject(), "clearMarkers");
        QMetaObject::invokeMethod(ui->mapWidget->rootObject(), "addMarker",
                                  Q_ARG(QVariant, latitude),
                                  Q_ARG(QVariant, longitude),
                                  Q_ARG(QVariant, projectName));
    } else {
        qDebug() << "Map widget or root object not initialized";
    }

}

QGeoCoordinate gestion_projet::geocodeLocation(const QString &locationName)
{
    // Simple implementation - you might want to use a real geocoding service
    // This is just a basic example using well-known locations

    QMap<QString, QGeoCoordinate> knownLocations = {
        {"Paris, France", QGeoCoordinate(48.8566, 2.3522)},
        {"New York, USA", QGeoCoordinate(40.7128, -74.0060)},
        {"London, UK", QGeoCoordinate(51.5074, -0.1278)},
        {"Tokyo, Japan", QGeoCoordinate(35.6762, 139.6503)},
        {"Tunis, Tunisia", QGeoCoordinate(36.8065, 10.1815)},
        {"Sfax, Tunisia", QGeoCoordinate(34.7406, 10.7603)},
        {"Sousse, Tunisia", QGeoCoordinate(35.8256, 10.6084)},
        {"Gabès, Tunisia", QGeoCoordinate(33.8815, 10.0982)},
        {"Bizerte, Tunisia", QGeoCoordinate(37.2744, 9.8739)},
        {"Kairouan, Tunisia", QGeoCoordinate(35.6781, 10.0963)},
        {"Tozeur, Tunisia", QGeoCoordinate(33.9197, 8.1335)},
        {"Monastir, Tunisia", QGeoCoordinate(35.7770, 10.8262)},
        {"Nabeul, Tunisia", QGeoCoordinate(36.4561, 10.7376)},
        {"Mahdia, Tunisia", QGeoCoordinate(35.5047, 11.0622)}
    };

    if (knownLocations.contains(locationName)) {
        return knownLocations[locationName];
    }

    // Try to parse as coordinates directly (if user entered "48.8566,2.3522")
    QStringList parts = locationName.split(',');
    if (parts.size() == 2) {
        bool latOk, lonOk;
        double lat = parts[0].trimmed().toDouble(&latOk);
        double lon = parts[1].trimmed().toDouble(&lonOk);

        if (latOk && lonOk) {
            return QGeoCoordinate(lat, lon);
        }
    }

    return QGeoCoordinate(); // Invalid coordinate
}

void gestion_projet::refreshMap() {
    // 1. Clear existing markers
    if (ui->mapWidget && ui->mapWidget->rootObject()) {
        QMetaObject::invokeMethod(ui->mapWidget->rootObject(), "clearMarkers");
    }

    // 2. Reload projects from DB
    QSqlQuery query("SELECT nom, latitude, longitude FROM PROJETS");
    while (query.next()) {
        QString name = query.value(0).toString();
        double lat = query.value(1).toDouble();
        double lon = query.value(2).toDouble();

        // 3. Add new markers
        if (ui->mapWidget && ui->mapWidget->rootObject()) {
            QMetaObject::invokeMethod(
                ui->mapWidget->rootObject(),
                "addMarker",
                Q_ARG(QVariant, lat),
                Q_ARG(QVariant, lon),
                Q_ARG(QVariant, name)
                );
        }
    }
}

void gestion_projet::on_send_clicked()
{

}

void gestion_projet::sendToOpenAI(const QString &message) {
    // 1. Safety checks
    if(!networkManager) {
        qDebug() << "Network manager not initialized";
        return;
    }

    if(openAIKey.isEmpty()) {
        ui->chatDisplay->append("System: OpenAI API key not configured");
        return;
    }

    // 2. Prepare request
    QNetworkRequest request(QUrl("https://api.openai.com/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + openAIKey).toUtf8());

    // 3. Prepare JSON payload
    QJsonObject json;
    json["model"] = "gpt-3.5-turbo";

    QJsonArray messages;
    QJsonObject systemMessage;
    systemMessage["role"] = "system";
    systemMessage["content"] = "You are a helpful assistant for project management.";
    messages.append(systemMessage);

    QJsonObject userMessage;
    userMessage["role"] = "user";
    userMessage["content"] = message;
    messages.append(userMessage);

    json["messages"] = messages;
    json["temperature"] = 0.7;

    // 4. Send request
    QNetworkReply *reply = networkManager->post(request, QJsonDocument(json).toJson());

    if(!reply) {
        qDebug() << "Failed to create network reply";
        return;
    }

    // 5. Connect signals
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
            QJsonObject jsonObject = jsonResponse.object();

            QString aiResponse = jsonObject["choices"].toArray()[0].toObject()["message"].toObject()["content"].toString();
            ui->chatDisplay->append("AI: " + aiResponse);
        } else {
            ui->chatDisplay->append("Error: " + reply->errorString());
        }
        reply->deleteLater();
    });
}

void gestion_projet::generateImage(const QString &prompt) {
    QNetworkRequest request(QUrl("https://api.stability.ai/v1/generation/stable-diffusion-xl-1024-v1-0/text-to-image"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + stabilityAIKey).toUtf8());
    request.setRawHeader("Accept", "application/json");

    QJsonObject json;
    json["text_prompts"] = QJsonArray{QJsonObject{{"text", prompt}, {"weight", 1.0}}};
    json["cfg_scale"] = 7;
    json["height"] = 1024;
    json["width"] = 1024;
    json["samples"] = 1;
    json["steps"] = 30;

    QNetworkReply *reply = networkManager->post(request, QJsonDocument(json).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
            QJsonArray artifacts = jsonResponse.object()["artifacts"].toArray();

            if (!artifacts.isEmpty()) {
                QString base64Image = artifacts[0].toObject()["base64"].toString();
                displayGeneratedImage(base64Image);
            }
        } else {
            ui->chatDisplay->append("Image generation error: " + reply->errorString());
        }
        reply->deleteLater();
    });
}

void gestion_projet::displayGeneratedImage(const QString &base64Image) {
    QByteArray imageData = QByteArray::fromBase64(base64Image.toUtf8());
    QPixmap pixmap;
    pixmap.loadFromData(imageData);

    // Create a dialog to display the image
    QDialog *imageDialog = new QDialog(this);
    QVBoxLayout *layout = new QVBoxLayout(imageDialog);
    QLabel *imageLabel = new QLabel(imageDialog);
    imageLabel->setPixmap(pixmap.scaled(512, 512, Qt::KeepAspectRatio));
    layout->addWidget(imageLabel);
    imageDialog->setWindowTitle("Generated Image");
    imageDialog->exec();
}
