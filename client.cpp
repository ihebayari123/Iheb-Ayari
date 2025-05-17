#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    initializeCallHistoryTable();


    // Connect signals

    callTimer = new QTimer(this);
    connect(callTimer, &QTimer::timeout, this, &Client::updateCallDuration);
    currentCallClientId.clear();
    setupAudioPlayback();

    tableWidget = ui->tableWidget;
    ui->sortComboBox->addItems({"ID","NOM"});
    ui->sortOrderButton->setIcon(QIcon(":/new/image/icons/sort-ascending.png"));// You'll need to add these icons
    currentSortAscending = true;
    connect(ui->deleteImageButton, &QPushButton::clicked, this, &Client::on_deleteImageButton_clicked);
    ui->deleteImageButton->setEnabled(false);
    connect(ui->exportExel, &QPushButton::clicked, this, &Client::exporterVersExcel);
    connect(ui->ddd, &QPushButton::clicked, this, &Client::on_searchButton_clicked);
    connect(ui->searchLineEdit, &QLineEdit::returnPressed, this, &Client::on_searchLineEdit_returnPressed);
    connect(ui->sortComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Client::on_sortComboBox_currentIndexChanged);
    connect(ui->sortOrderButton, &QPushButton::clicked, this, &Client::on_sortOrderButton_clicked);


    ui->pdfLabel->setMouseTracking(true);
    ui->pdfLabel->installEventFilter(this);
    // Set initial text
    ui->pdfLabel->setText("CV Affiché\n\nCliquez pour ajouter un CV");
    ui->pdfLabel->setAlignment(Qt::AlignCenter);
    ui->pdfLabel->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    pdfDocument = new QPdfDocument(this);
    pageRenderer = new QPdfPageRenderer(this);
    pageRenderer->setDocument(pdfDocument);


    connect(pageRenderer, &QPdfPageRenderer::pageRendered,this, &Client::onPdfPageRendered);

    connect(ui->verifyButton, &QPushButton::clicked, this, [this](){
        if (pdfFilePath.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez téléverser un CV");
            return;
        }

        QString projectName = ui->projectNameEdit->text();
        if (projectName.isEmpty()) {
            QMessageBox::warning(this, "Erreur", "Veuillez spécifier un projet");
            return;
        }

        // Get the current client ID from the selected row
        int currentRow = ui->tableWidget->currentRow();
        if (currentRow < 0) {
            QMessageBox::warning(this, "Error", "Please select a client first");
            return;
        }
        int clientId = ui->tableWidget->item(currentRow, 0)->text().toInt();

        // Variables to store results
        int score;
        QStringList foundSkills;
        QStringList missingSkills;

        // Call the unified analyzeCompatibility function with UI update
        analyzeCompatibility(pdfFilePath, projectName, score, foundSkills, missingSkills, true, clientId);
    });

    connect(ui->effacerCvButton, &QPushButton::clicked, this, &Client::on_effacerCvButton_clicked);

    connect(ui->loadStatsButton, &QPushButton::clicked, this, &Client::loadStatistics);



    mediaRecorder = new QMediaRecorder(this);

    QAudioDevice inputDevice = QMediaDevices::defaultAudioInput();
    if (inputDevice.isNull()) {
        qDebug() << "No audio input device available";
    }

    audioInput = new QAudioInput(inputDevice, this);
    callSavePath = "F:/esprit 2/QT/StartWise2A17-int-gration_final/calls";
    callSavePath = QDir::cleanPath(callSavePath);

    // Create directory if it doesn't exist
    if (!QDir().mkpath(callSavePath)) {
        qDebug() << "Failed to create call directory!";
        // Fallback to documents folder if needed
        callSavePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
                       + "/StartWise_Calls";
        QDir().mkpath(callSavePath);
    }

    qDebug() << "Using call storage path:" << callSavePath;
    qDebug() << "Directory exists:" << QDir(callSavePath).exists();
    qDebug() << "Is writable:" << QFileInfo(callSavePath).isWritable();


    // Connect call buttons
    connect(ui->callButton, &QPushButton::clicked, this, &Client::on_callButton_clicked);

    connect(ui->callButton, &QPushButton::clicked, this, [this]() {
        int row = ui->tableWidget->currentRow();
        if (row >= 0) {
            QString phoneNumber = ui->tableWidget->item(row, 5)->text(); // Assuming phone is column 5
            startWhatsAppCall(phoneNumber);
        }
    });

connect(ui->playCallButton, &QPushButton::clicked, this, [this]() {
    int row = ui->callHistoryTable->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a call record first");
        return;
    }

        QTableWidgetItem *item = ui->callHistoryTable->item(row, 3); // Status column has the audio path
        if (!item) {
            QMessageBox::warning(this, "Error", "No call record selected");
            return;
        }

        QString audioPath = item->data(Qt::UserRole).toString();
        qDebug() << "Attempting to play audio from path:" << audioPath;

        if (audioPath.isEmpty()) {
            QMessageBox::warning(this, "Error", "No audio recording available for this call");
            return;
        }

        // Check if path is relative and convert to absolute if needed
        QFileInfo fileInfo(audioPath);
        if (!fileInfo.exists()) {
            // Try prepending the callSavePath if the path is relative
            QString possiblePath = callSavePath + "/" + audioPath;
            qDebug() << "Trying alternative path:" << possiblePath;
            if (QFileInfo(possiblePath).exists()) {
                audioPath = possiblePath;
            }
        }

        playRecordedCall(audioPath);
    });

    connect(ui->endCallButton, &QPushButton::clicked, this, &Client::on_endCallButton_clicked);




    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged,
            this, &Client::on_tableWidget_itemSelectionChanged);


    setupDragAndDrop();
    setupTable();
    refreshTable();
}

Client::~Client()
{
    delete ui;
}

Client::Client(int id, QString nom, QString prenom, QString email, QString adresse, int tel, QByteArray imageData, QWidget *parent)
    : QDialog(parent), ui(new Ui::Client)
{
    ui->setupUi(this);

    this->id = id;
    this->nom = nom;
    this->prenom = prenom;
    this->email = email;
    this->adresse = adresse;
    this->tel = tel;
    this->imageData = imageData;

}

bool Client::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO CLIENTS (nom, prenom, email, adresse, tel, cimage) "
                  "VALUES (:nom, :prenom, :email, :adresse, :tel, :image)");

    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":adresse", adresse);
    query.bindValue(":tel", QString::number(tel));
    query.bindValue(":image", imageData);


    return query.exec();
}



QSqlQueryModel* Client::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT id, nom, prenom, email, adresse, tel, cimage FROM clients");
    if (!model) {
        qDebug() << "Failed to allocate model!";
        return nullptr;
    }

    if (model->lastError().isValid()) {
        qDebug() << "Error in afficher(): " << model->lastError().text();
        delete model;
        return nullptr;
    }

    qDebug() << "Number of rows fetched:" << model->rowCount();
    return model;
}





bool Client::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM clients WHERE ID = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Delete error: " << query.lastError().text();
        return false;
    }
}

bool Client::update(int id, const QString &nom, const QString &prenom, const QString &email, const QString &adresse, int tel) {
    QSqlQuery query;
    query.prepare("UPDATE clients SET nom = :nom, prenom = :prenom, email = :email, adresse = :adresse, tel = :tel WHERE id = :id");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":adresse", adresse);
    query.bindValue(":tel", QString::number(tel));
    query.bindValue(":id", id);

    if (query.exec()) {
        return true;
    } else {
        qDebug() << "Update error:" << query.lastError().text();
        return false;
    }
}

// Validation methods
bool Client::validateNom(const QString &nom) {

    QRegularExpression regex("^[A-Za-z\\s]+$");
    return regex.match(nom).hasMatch() && !nom.isEmpty();
}

bool Client::validatePrenom(const QString &prenom) {

    QRegularExpression regex("^[A-Za-z\\s]+$");
    return regex.match(prenom).hasMatch() && !prenom.isEmpty();
}

bool Client::validateEmail(const QString &email) {

    QRegularExpression regex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    return regex.match(email).hasMatch() && !email.isEmpty();
}

bool Client::validateAdresse(const QString &adresse) {

    return !adresse.isEmpty();
}

bool Client::validateTel(const QString &tel) {

    QRegularExpression regex("^\\d{8}$");
    return regex.match(tel).hasMatch() && !tel.isEmpty();
}


void Client::setupTable()
{
    if (!tableWidget) {
        qDebug() << "Error: tableWidget not found!";
        return;
    }

    // Configuration de base du tableau
    tableWidget->setColumnCount(9);
    tableWidget->setHorizontalHeaderLabels({"ID", "Nom", "Prenom", "Email", "Adresse", "Tel", "Image", "Modify", "Delete"});
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void Client::refreshTable()
{
    if (!tableWidget) return;

    tableWidget->setRowCount(0); // Effacer les lignes existantes

    QSqlQueryModel* model = afficher();
    if (!model) return;

    for (int row = 0; row < model->rowCount(); ++row) {
        tableWidget->insertRow(row);

        // Ajout des données (colonnes 0 à 5)
        for (int col = 0; col < 6; ++col) {
            QModelIndex index = model->index(row, col);
            if (!index.isValid()) continue;
            QString value = model->data(index).toString();
            QTableWidgetItem* item = new QTableWidgetItem(value);
            tableWidget->setItem(row, col, item);
        }

        // Image (colonne 6)
        QModelIndex imageIndex = model->index(row, 6);
        QByteArray imageData = model->data(imageIndex).toByteArray();

        if (!imageData.isEmpty()) {
            QLabel* imageLabel = new QLabel();
            QPixmap pixmap;
            pixmap.loadFromData(imageData);
            if (!pixmap.isNull()) {
                pixmap = pixmap.scaled(30, 30, Qt::KeepAspectRatio);
                imageLabel->setPixmap(pixmap);
                imageLabel->setAlignment(Qt::AlignCenter);
                tableWidget->setCellWidget(row, 6, imageLabel);
            }
        }

        QPushButton* modifyButton = new QPushButton();
        modifyButton->setIcon(QIcon(":/new/image/icons/stylo.png"));
        modifyButton->setProperty("row", row);
        modifyButton->setToolTip("Modify");
        modifyButton->setIconSize(QSize(24, 24));
        connect(modifyButton, &QPushButton::clicked, this, &Client::onModifyButtonClicked);
        tableWidget->setCellWidget(row, model->columnCount(), modifyButton);

        // Add Delete button with icon
        QPushButton* deleteButton = new QPushButton();
        deleteButton->setIcon(QIcon(":/new/image/icons/bouton-supprimer.png"));
        deleteButton->setProperty("row", row);
        deleteButton->setToolTip("Delete");
        deleteButton->setIconSize(QSize(24, 24));
        connect(deleteButton, &QPushButton::clicked, this, &Client::onDeleteButtonClicked);
        tableWidget->setCellWidget(row, model->columnCount() + 1, deleteButton);
    }

    delete model;
}

void Client::on_addClientButton_clicked()
{
    QString nom = ui->nomLineEdit->text();
    QString prenom = ui->prenomLineEdit->text();
    QString email = ui->emailLineEdit->text();
    QString adresse = ui->adresseLineEdit->text();
    int tel = ui->telLineEdit->text().toInt();

    // Validate inputs
    if (!validateNom(nom)) {
        QMessageBox::warning(this, "Input Error", "Nom invalide. Veuillez entrer un nom valide.");
        return;
    }
    if (!validatePrenom(prenom)) {
        QMessageBox::warning(this, "Input Error", "Prénom invalide. Veuillez entrer un prénom valide.");
        return;
    }
    if (!validateEmail(email)) {
        QMessageBox::warning(this, "Input Error", "Email invalide. Veuillez entrer un email valide.");
        return;
    }
    if (!validateAdresse(adresse)) {
        QMessageBox::warning(this, "Input Error", "Adresse invalide. Veuillez entrer une adresse valide.");
        return;
    }
    if (!validateTel(QString::number(tel))) {
        QMessageBox::warning(this, "Input Error", "Téléphone invalide. Veuillez entrer un numéro de téléphone valide (8 chiffres).");
        return;
    }

    // Ensure image is uploaded before adding
    if (imageData.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Veuillez télécharger une image.");
        return;
    }
    qDebug() << "Before adding client";
    Client *c = new Client(0, nom, prenom, email, adresse, tel, imageData, this);
    qDebug() << "Client object created";
    if (c->ajouter()) {
        qDebug() << "Client added to database";
        QMessageBox::information(this, "Success", "Client ajouté avec succès.");
        refreshTable();
        qDebug() << "Table refreshed";
        // Clear the image data after successful addition
        imageData.clear();
        ui->imageLabel->clear();
    } else {
        QMessageBox::critical(this, "Database Error", "Échec de l'ajout du client.");
    }

    delete c;

}

void Client::on_uploadImageButton_clicked() {
    // If an image exists, confirm replacement
    if (!imageData.isEmpty()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Replace Image",
                                      "An image is already uploaded. Replace it?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
    }

    QString selectedFile = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.jpeg *.bmp)");

    if (!selectedFile.isEmpty()) {
        QFile file(selectedFile);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, "Error", "Could not open image file.");
            return;
        }

        imageData = file.readAll();
        file.close();

        QPixmap pixmap;
        if (pixmap.loadFromData(imageData)) {
            ui->imageLabel->setPixmap(pixmap.scaled(ui->imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->deleteImageButton->setEnabled(true); // Enable delete button
        } else {
            QMessageBox::warning(this, "Error", "Invalid image format.");
        }
    }
}

void Client::on_deleteImageButton_clicked()
{
    // Clear the image data
    imageData.clear();

    // Clear the image display
    ui->imageLabel->clear();


    // Disable the delete button
    ui->deleteImageButton->setEnabled(false);

    // Enable upload button if it was disabled
    ui->uploadImageButton->setEnabled(true);
}



void Client::onModifyButtonClicked() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        int row = button->property("row").toInt(); // Get the row index from the button's property

        // Fetch the client data from the table
        int id = tableWidget->item(row, 0)->text().toInt(); // Assuming ID is in the first column
        QString nom = tableWidget->item(row, 1)->text();
        QString prenom = tableWidget->item(row, 2)->text();
        QString email = tableWidget->item(row, 3)->text();
        QString adresse = tableWidget->item(row, 4)->text();
        int tel = tableWidget->item(row, 5)->text().toInt();

        // Open the modifyclient dialog
        modifyclient dialog(this);
        dialog.setData(nom, prenom, email, adresse, tel);

        if (dialog.exec() == QDialog::Accepted) {
            // Update the client data in the database
            QString newNom = dialog.getNom();
            QString newPrenom = dialog.getPrenom();
            QString newEmail = dialog.getEmail();
            QString newAdresse = dialog.getAdresse();
            int newTel = dialog.getTel();

            // Call the update method
            Client client;
            if (client.update(id, newNom, newPrenom, newEmail, newAdresse, newTel)) {
                QMessageBox::information(this, "Success", "Client updated successfully.");
                refreshTable(); // Refresh the table to reflect the changes
                //showClientGrowthChart();
            } else {
                QMessageBox::critical(this, "Error", "Failed to update client.");
            }
        }
    }
}

void Client::onDeleteButtonClicked() {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        int row = button->property("row").toInt(); // Get the row index from the button's property
        qDebug() << "Delete button clicked for row:" << row;

        // Fetch the client ID from the table
        int id = tableWidget->item(row, 0)->text().toInt(); // Assuming ID is in the first column

        // Confirm deletion
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete Client", "Are you sure you want to delete this client?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // Delete the client from the database
            Client client;
            if (client.supprimer(id)) {
                QMessageBox::information(this, "Success", "Client deleted successfully.");
                refreshTable(); // Refresh the table to reflect the changes
                //updateClientCount();
                //showClientGrowthChart();
            } else {
                QMessageBox::critical(this, "Error", "Failed to delete client.");
            }
        }
    }
}



void Client::exporterVersExcel()
{
    // Demander à l'utilisateur de choisir un emplacement pour enregistrer le fichier
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le fichier Excel", "", "Fichiers Excel (*.xlsx)");

    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun emplacement sélectionné !");
        return;
    }

    // Créer un document Excel
    QXlsx::Document xlsx;

    // Ajouter les en-têtes de colonnes (de 0 à 7)
    for (int col = 0; col < 6; ++col) {  // On s'arrête à la colonne 7 (index 6)
        QString header = ui->tableWidget->model()->headerData(col, Qt::Horizontal).toString();
        xlsx.write(1, col + 1, header);  // La première ligne contient les en-têtes
    }

    // Ajouter les données de la table (de 0 à 7)
    for (int row = 0; row < ui->tableWidget->model()->rowCount(); ++row) {
        for (int col = 0; col < 7; ++col) {  // On s'arrête à la colonne 6 (index 5)
            QModelIndex index = ui->tableWidget->model()->index(row, col);
            QString data = ui->tableWidget->model()->data(index).toString();

            // Si c'est la colonne "Image" (colonne 7), exporter le chemin de l'image
            if (col == 7) {
                QByteArray imageData = ui->tableWidget->model()->data(index, Qt::UserRole).toByteArray();
                if (!imageData.isEmpty()) {
                    // Sauvegarder l'image dans un fichier temporaire
                    QString imagePath = QDir::tempPath() + QString("/image_%1.png").arg(row);
                    QFile file(imagePath);
                    if (file.open(QIODevice::WriteOnly)) {
                        file.write(imageData);
                        file.close();
                    }
                    data = imagePath;  // Utiliser le chemin de l'image comme donnée
                }
            }

            xlsx.write(row + 2, col + 1, data);  // Les données commencent à la deuxième ligne
        }
    }

    // Sauvegarder le fichier Excel
    if (xlsx.saveAs(filePath)) {
        QMessageBox::information(this, "Succès", "Exportation réussie vers : " + filePath);
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'exportation !");
    }
}


QSqlQueryModel* Client::rechercher(const QString& searchTerm) {
    QSqlQueryModel* model = new QSqlQueryModel();

    if (searchTerm.isEmpty()) {
        model->setQuery("SELECT id, nom, prenom, email, adresse, tel, cimage FROM clients");
    } else {
        QString queryStr = "SELECT id, nom, prenom, email, adresse, tel, cimage FROM clients WHERE "
                           "TO_CHAR(id) LIKE '%' || :search || '%' OR "
                           "nom LIKE '%' || :search || '%' OR "
                           "prenom LIKE '%' || :search || '%' OR "
                           "email LIKE '%' || :search || '%' OR "
                           "adresse LIKE '%' || :search || '%' OR "
                           "tel LIKE '%' || :search || '%'";

        QSqlQuery query;
        query.prepare(queryStr);
        query.bindValue(":search", searchTerm);

        if (!query.exec()) {
            qDebug() << "Search error:" << query.lastError().text();
            delete model;
            return nullptr;
        }

        model->setQuery(query);
    }

    return model;
}




void Client::on_searchButton_clicked() {
    QString searchTerm = ui->searchLineEdit->text();
    refreshTableWithSearch(searchTerm);
}

void Client::on_searchLineEdit_returnPressed() {
    on_searchButton_clicked();
}

QSqlQueryModel* Client::trierPar(const QString& column, bool ascending)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString order = ascending ? "ASC" : "DESC";

    QString queryString = QString(
                              "SELECT ID, NOM, PRENOM, EMAIL, ADRESSE, TEL, CIMAGE "
                              "FROM CLIENTS ORDER BY %1 %2").arg(column).arg(order);

    model->setQuery(queryString);

    if (model->lastError().isValid()) {
        qDebug() << "Sorting error:" << model->lastError().text();
        delete model;
        return nullptr;
    }

    return model;
}

void Client::on_sortComboBox_currentIndexChanged(int index) {
    Q_UNUSED(index);
    QString column = ui->sortComboBox->currentText().toLower();
    refreshTableWithSort(column, currentSortAscending);
}

void Client::on_sortOrderButton_clicked() {
    currentSortAscending = !currentSortAscending;

    // Update the button icon
    if (currentSortAscending) {
        ui->sortOrderButton->setIcon(QIcon(":/new/image/icons/ascending-sort.png"));
    } else {
        ui->sortOrderButton->setIcon(QIcon(":/new/image/icons/sort.png")); // DESC icon
    }

    // Get the current sort column and refresh
    QString column = ui->sortComboBox->currentText().toLower();
    refreshTableWithSort(column, currentSortAscending);
}

void Client::refreshTableWithSort(const QString& column, bool ascending)
{
    if (!tableWidget) return;

    // Clear existing content
    tableWidget->clearContents();
    tableWidget->setRowCount(0);

    // Convert the UI column name to database column name if needed
    QString dbColumn;
    if (column == "id") dbColumn = "ID";
    else if (column == "nom") dbColumn = "NOM";
    else dbColumn = "ID"; // Default sort column

    // Get the sorted model
    QSqlQueryModel* model = trierPar(dbColumn, ascending);

    if (model) {
        // Use the same population logic as populateTableWithModel
        tableWidget->setRowCount(model->rowCount());
        tableWidget->setColumnCount(model->columnCount() + 2); // +2 for action buttons
        tableWidget->setHorizontalHeaderLabels({"ID", "Nom", "Prenom", "Email", "Adresse", "Tel", "Image", "Modify", "Delete"});

        for (int row = 0; row < model->rowCount(); ++row) {
            // Data columns (0-5)
            for (int col = 0; col < 6; ++col) {
                QModelIndex index = model->index(row, col);
                QString value = model->data(index).toString();
                QTableWidgetItem* item = new QTableWidgetItem(value);
                tableWidget->setItem(row, col, item);
            }

            // Image column (6)
            QModelIndex imageIndex = model->index(row, 6);
            QByteArray imageData = model->data(imageIndex).toByteArray();

            if (!imageData.isEmpty()) {
                QLabel* imageLabel = new QLabel();
                QPixmap pixmap;
                pixmap.loadFromData(imageData);

                if (!pixmap.isNull()) {
                    pixmap = pixmap.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    imageLabel->setPixmap(pixmap);
                    imageLabel->setAlignment(Qt::AlignCenter);
                    tableWidget->setCellWidget(row, 6, imageLabel);
                }
            }

            // Modify button (7)
            QPushButton* modifyButton = new QPushButton();
            modifyButton->setIcon(QIcon(":/new/image/icons/stylo.png"));
            modifyButton->setProperty("row", row);
            modifyButton->setToolTip("Modify");
            modifyButton->setIconSize(QSize(24, 24));
            connect(modifyButton, &QPushButton::clicked, this, &Client::onModifyButtonClicked);
            tableWidget->setCellWidget(row, 7, modifyButton);

            // Delete button (8)
            QPushButton* deleteButton = new QPushButton();
            deleteButton->setIcon(QIcon(":/new/image/icons/bouton-supprimer.png"));
            deleteButton->setProperty("row", row);
            deleteButton->setToolTip("Delete");
            deleteButton->setIconSize(QSize(24, 24));
            connect(deleteButton, &QPushButton::clicked, this, &Client::onDeleteButtonClicked);
            tableWidget->setCellWidget(row, 8, deleteButton);
        }

        //tableWidget->resizeColumnsToContents();
        delete model;
    }
}

void Client::refreshTableWithSearch(const QString& searchTerm) {
    if (tableWidget) {
        tableWidget->clearContents();
        tableWidget->setRowCount(0);

        QSqlQueryModel* model = Client::rechercher(searchTerm);
        populateTableWithModel(model);
    }
}


void Client::populateTableWithModel(QSqlQueryModel* model) {
    if (model) {
        tableWidget->setRowCount(model->rowCount());
        tableWidget->setColumnCount(model->columnCount() + 2);
        tableWidget->setHorizontalHeaderLabels({"ID", "Nom", "Prenom", "Email", "Adresse", "Tel", "Image", "Modify", "Delete"});

        for (int row = 0; row < model->rowCount(); ++row) {
            for (int col = 0; col < model->columnCount() - 1; ++col) {
                QModelIndex index = model->index(row, col);
                QString value = model->data(index).toString();
                QTableWidgetItem* item = new QTableWidgetItem(value);
                tableWidget->setItem(row, col, item);
            }

            // Handle image column
            QModelIndex imageIndex = model->index(row, model->columnCount() - 1);
            QByteArray imageData = model->data(imageIndex).toByteArray();

            if (!imageData.isEmpty()) {
                QLabel* imageLabel = new QLabel();
                QPixmap pixmap;
                pixmap.loadFromData(imageData);

                if (!pixmap.isNull()) {
                    pixmap = pixmap.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    imageLabel->setPixmap(pixmap);
                    imageLabel->setAlignment(Qt::AlignCenter);
                    tableWidget->setCellWidget(row, model->columnCount() - 1, imageLabel);
                }
            }

            // Add Modify button
            QPushButton* modifyButton = new QPushButton();
            modifyButton->setIcon(QIcon(":/new/image/icons/stylo.png"));
            modifyButton->setProperty("row", row);
            modifyButton->setToolTip("Modify");
            modifyButton->setIconSize(QSize(24, 24));
            connect(modifyButton, &QPushButton::clicked, this, &Client::onModifyButtonClicked);
            tableWidget->setCellWidget(row, model->columnCount(), modifyButton);

            // Add Delete button
            QPushButton* deleteButton = new QPushButton();
            deleteButton->setIcon(QIcon(":/new/image/icons/bouton-supprimer.png"));
            deleteButton->setProperty("row", row);
            deleteButton->setToolTip("Delete");
            deleteButton->setIconSize(QSize(24, 24));
            connect(deleteButton, &QPushButton::clicked, this, &Client::onDeleteButtonClicked);
            tableWidget->setCellWidget(row, model->columnCount() + 1, deleteButton);
        }

        tableWidget->resizeColumnsToContents();
        delete model;
    }
}

void Client::setupDragAndDrop() {
    setAcceptDrops(true);
    ui->pdfLabel->setAcceptDrops(true);
    ui->pdfLabel->setScaledContents(true);

    // Set a PDF icon as default
    QPixmap pdfIcon(":/icons/pdf-icon.png"); // Make sure you have this icon in your resources
    ui->pdfLabel->setPixmap(pdfIcon.scaled(100, 100, Qt::KeepAspectRatio));
    ui->pdfLabel->setAlignment(Qt::AlignCenter);
}

void Client::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        QUrl url = event->mimeData()->urls().first();
        if (url.fileName().endsWith(".pdf", Qt::CaseInsensitive)) {
            event->acceptProposedAction();
        }
    }
}

void Client::dropEvent(QDropEvent *event) {
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QUrl url = mimeData->urls().first();
        QString filePath = url.toLocalFile();

        // Check if it's a PDF
        if (filePath.endsWith(".pdf", Qt::CaseInsensitive)) {
            // Clear previous PDF if exists
            if (!pdfFilePath.isEmpty()) {
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "Remplacer CV",
                                              "Un CV est déjà chargé. Voulez-vous le remplacer?",
                                              QMessageBox::Yes | QMessageBox::No);

                if (reply == QMessageBox::No) {
                    return;
                }
            }

            pdfDocument->close();
            pdfFilePath = filePath;
            displayPdfInfo(filePath);
        }
    }
}

void Client::displayPdfInfo(const QString& filePath) {
    if (pdfDocument->load(filePath) != QPdfDocument::Error::None) {
        QMessageBox::warning(this, "Error", "Could not load PDF file");
        return;
    }

    // Render the first page
    if (pdfDocument->pageCount() > 0) {
        pageRenderer->requestPage(0, QSize(200, 200));
    }

    // Set file info tooltip
    QFileInfo fileInfo(filePath);
    ui->pdfLabel->setToolTip(QString("PDF: %1\nSize: %2 KB")
                                 .arg(fileInfo.fileName())
                                 .arg(fileInfo.size() / 1024));
}

// Add this to handle double-click to open PDF
void Client::on_pdfLabel_doubleClicked() {
    if (!pdfFilePath.isEmpty()) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(pdfFilePath));
    }
}

QByteArray Client::getClientImage(int id) {
    QSqlQuery query;
    query.prepare("SELECT cimage FROM clients WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return query.value(0).toByteArray();
    }
    return QByteArray();
}

void Client::on_uploadPdfButton_clicked() {
    // If a PDF already exists, ask if they want to replace it
    if (!pdfFilePath.isEmpty()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Remplacer CV",
                                      "Un CV est déjà chargé. Voulez-vous le remplacer?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::No) {
            return;
        }
    }

    QString selectedFile = QFileDialog::getOpenFileName(this,
                                                        "Sélectionner un fichier PDF",
                                                        "",
                                                        "Fichiers PDF (*.pdf)");

    if (!selectedFile.isEmpty()) {
        // Clear previous PDF first
        pdfDocument->close();

        pdfFilePath = selectedFile;
        displayPdfInfo(selectedFile);

        // Update UI to show new PDF is loaded
        ui->pdfLabel->setToolTip(QFileInfo(selectedFile).fileName());
    }
}

bool Client::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->pdfLabel) {
        if (event->type() == QEvent::MouseButtonDblClick) {
            on_pdfLabel_doubleClicked();
            return true;
        }
    }
    return QDialog::eventFilter(watched, event);
}

void Client::on_effacerCvButton_clicked()
{
    // Check if there's a CV currently displayed
    if (pdfFilePath.isEmpty() && ui->pdfLabel->text().isEmpty() && ui->pdfLabel->pixmap().isNull())
    {
        QMessageBox::information(this, "Aucun CV", "Il n'y a pas de CV à effacer.");
        return;
    }

    // Ask for confirmation before clearing
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Êtes-vous sûr de vouloir effacer ce CV?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Clear the stored PDF file path
    pdfFilePath.clear();

    // Clear the PDF document
    pdfDocument->close();

    // Clear the label display and reset to default state
    ui->pdfLabel->clear();
    ui->pdfLabel->setText("CV Affiché\n\nCliquez pour ajouter un CV");
    ui->pdfLabel->setAlignment(Qt::AlignCenter);

    // Clear any tooltip
    ui->pdfLabel->setToolTip("");

    // Reset the compatibility results if they were shown
    ui->compatibilityProgress->setValue(0);
    ui->strengthsList->clear();
    ui->weaknessesList->clear();
    ui->clientPhotoLabel->clear();

    // Show confirmation message
    QMessageBox::information(this, "CV Effacé", "Le CV a été supprimé avec succès.");
}


QString Client::extractPdfText(const QString& filePath) {
    QPdfDocument doc;
    if (doc.load(filePath) != QPdfDocument::Error::None) {
        qWarning() << "Failed to load PDF";
        return "";
    }

    QString fullText;
    for (int i = 0; i < doc.pageCount(); ++i) {
        fullText += doc.getAllText(i).text() + "\n";
    }
    return fullText.simplified();
}




bool Client::analyzeCompatibility(const QString& cvPath, const QString& projectName,
                                  int& score, QStringList& foundSkills, QStringList& missingSkills,
                                  bool updateUI, int clientId) {
    // 1. Extract text from PDF
    QString cvText = extractPdfText(cvPath);
    if (cvText.isEmpty()) {
        if (updateUI) {
            QMessageBox::warning(this, "Error", "Could not extract text from CV");
        }
        return false;
    }

    // 2. Get required skills for the project
    QSqlQuery reqQuery;
    reqQuery.prepare("SELECT REQUIRED_SKILLS FROM PROJETS WHERE NOM = :name");
    reqQuery.bindValue(":name", projectName);

    if (!reqQuery.exec() || !reqQuery.next()) {
        if (updateUI) {
            QMessageBox::warning(this, "Error", "Could not find project requirements");
        }
        return false;
    }

    QStringList requiredSkills = reqQuery.value(0).toString().split(",");

    // 3. Calculate matches
    int matches = 0;
    foundSkills.clear();
    missingSkills.clear();

    for (const QString& skill : requiredSkills) {
        if (cvText.contains(skill, Qt::CaseInsensitive)) {
            matches++;
            foundSkills << skill;
        } else {
            missingSkills << skill;
        }
    }

    // 4. Calculate score (0-100%)
    score = (matches * 100) / requiredSkills.size();

    // 5. Update UI if requested
    if (updateUI) {
        if (clientId != -1) {
            loadClientImage(clientId);
        }
        showCompatibilityResults(score, foundSkills, missingSkills);
        ui->tabWidget->setCurrentIndex(4);
    }

    return true;
}

void Client::loadClientImage(int clientId) {
    QSqlQuery query;
    query.prepare("SELECT cimage FROM clients WHERE id = :id");
    query.bindValue(":id", clientId);

    if (query.exec() && query.next()) {
        QByteArray imageData = query.value(0).toByteArray();
        if (!imageData.isEmpty()) {
            QPixmap pixmap;
            pixmap.loadFromData(imageData);

            if (!pixmap.isNull()) {
                ui->clientPhotoLabel->setPixmap(pixmap.scaled(ui->clientPhotoLabel->size(),
                                                              Qt::KeepAspectRatio, Qt::SmoothTransformation));
            }
        }
    }
}

void Client::updateCompatibilityResults(int clientId, const QString& projectName) {
    // Remove these lines:
    // ui->compatibilityTable->clearContents();
    // ui->compatibilityTable->setRowCount(0);

    // Keep the rest of the function as is:
    // Get client image from database
    QSqlQuery imgQuery;
    imgQuery.prepare("SELECT cimage FROM clients WHERE id = ?");
    imgQuery.addBindValue(clientId);

    if (imgQuery.exec() && imgQuery.next()) {
        QByteArray imgData = imgQuery.value(0).toByteArray();
        QPixmap photo;
        photo.loadFromData(imgData);
        ui->clientPhotoLabel->setPixmap(photo.scaled(100,100, Qt::KeepAspectRatio));
    }

    // Get compatibility data
    QSqlQuery compQuery;
    compQuery.prepare("SELECT * FROM client_compatibility "
                      "WHERE client_id = ? AND project_name = ? "
                      "ORDER BY analysis_date DESC LIMIT 1");
    compQuery.addBindValue(clientId);
    compQuery.addBindValue(projectName);

    if (compQuery.exec() && compQuery.next()) {
        int score = compQuery.value("compatibility_score").toInt();
        QString strengths = compQuery.value("strengths").toString();

        // Update UI elements
        ui->compatibilityProgress->setValue(score);
        ui->strengthsList->addItems(strengths.split(","));

        // Visual feedback
        QString style = QString("QProgressBar::chunk { background: %1; }")
                            .arg(score > 50 ? "#4CAF50" : "#F44336");
        ui->compatibilityProgress->setStyleSheet(style);
    }
}



void Client::showCompatibilityResults(int score, const QStringList& strengths, const QStringList& weaknesses) {
    // Update progress bar
    ui->compatibilityProgress->setValue(score);


    // Set progress bar color based on score
    QString color;
    if (score > 75) {
        color = "#4CAF50"; // Green
    } else if (score > 50) {
        color = "#FFC107"; // Yellow
    } else {
        color = "#F44336"; // Red
    }

    ui->compatibilityProgress->setStyleSheet(
        QString("QProgressBar { text-align: center; } "
                "QProgressBar::chunk { background: %1; }").arg(color));

    // Show strengths
    ui->strengthsList->clear();
    ui->strengthsList->addItems(strengths);

    // Show weaknesses (Point Faible)
    ui->weaknessesList->clear();
    ui->weaknessesList->addItems(weaknesses);

    // Update the compatibility percentage label

}

void Client::onPdfPageRendered(int pageNumber, QSize imageSize, const QImage &image)
{
    Q_UNUSED(pageNumber);
    Q_UNUSED(imageSize);

    if (!image.isNull()) {
        QPixmap pixmap = QPixmap::fromImage(image);
        ui->pdfLabel->setPixmap(pixmap.scaled(ui->pdfLabel->size(),
                                              Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

int Client::countClients() {
    QSqlQuery query;
    if (!query.exec("SELECT COUNT(*) FROM CLIENTS")) {
        qDebug() << "SQL Error:" << query.lastError().text();
        return -1;
    }

    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}


QMap<QDate, int> Client::getClientGrowthData() {
    QMap<QDate, int> growthData;
    QSqlQuery query;

    // Improved query with explicit date formatting
    query.prepare("SELECT TO_CHAR(DATE_CREATION, 'YYYY-MM-DD') as creation_date, "
                  "COUNT(*) as client_count "
                  "FROM CLIENTS "
                  "GROUP BY TO_CHAR(DATE_CREATION, 'YYYY-MM-DD') "
                  "ORDER BY creation_date");

    if (query.exec()) {
        while (query.next()) {
            QString dateStr = query.value(0).toString();
            QDate date = QDate::fromString(dateStr, "yyyy-MM-dd");
            int count = query.value(1).toInt();
            growthData[date] = count;
            qDebug() << "Date:" << date.toString("yyyy-MM-dd") << "Count:" << count;
        }
    } else {
        qDebug() << "Query error:" << query.lastError().text();
    }

    return growthData;
}

void Client::updateClientCount() {
    Client client;
    int count = client.countClients();

    if (count >= 0) {
        ui->clientCountLabel->setText("Total Clients: " + QString::number(count));
    } else {
        ui->clientCountLabel->setText("Error retrieving client count.");
    }



}


void Client::showClientGrowthChart() {
    Client client;
    QMap<QDate, int> growthData = client.getClientGrowthData();

    if (growthData.isEmpty()) {
        qDebug() << "No client growth data available.";
        return;
    }

    // Debug output to verify data
    qDebug() << "Growth data points:";
    for (auto it = growthData.begin(); it != growthData.end(); ++it) {
        qDebug() << it.key().toString("yyyy-MM-dd") << ":" << it.value();
    }

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
        qDebug() << "Adding point:" << it.key().toString("yyyy-MM-dd") << "->" << cumulativeCount;
    }

    // Customize bar appearance
    barSet->setColor(QColor(70, 130, 180)); // Steel blue color
    barSet->setBorderColor(Qt::black);
    series->append(barSet);

    // Create chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Client Growth Over Time");
    chart->legend()->setVisible(true);
    chart->setAnimationOptions(QChart::AllAnimations);

    // X-axis (categories - dates)
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText("Date");
    axisX->setLabelsAngle(-45); // Rotate labels for better readability
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Y-axis (count)
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Number of Clients");
    axisY->setLabelFormat("%i");
    axisY->setMin(0);
    axisY->setMax(cumulativeCount * 1.1); // Add 10% padding
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Styling
    chart->setTheme(QChart::ChartThemeBlueCerulean);
    series->setLabelsVisible(true);
    series->setLabelsFormat("@value");
    series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);

    // Create chart view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Calculate required width (100px per bar or minimum 800px)
    int requiredWidth = qMax(800, categories.size() * 100);
    chartView->setMinimumSize(requiredWidth, 400);

    // Create scroll area
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(chartView);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Find your chartContainer widget
    QWidget *chartContainer = ui->chartContainer;

    if (!chartContainer) {
        qDebug() << "Error: chartContainer not found!";
        return;
    }

    // Clear previous content
    QLayout *layout = chartContainer->layout();
    if (!layout) {
        layout = new QVBoxLayout(chartContainer);
        layout->setContentsMargins(0, 0, 0, 0);
    } else {
        QLayoutItem *item;
        while ((item = layout->takeAt(0))) {
            delete item->widget();
            delete item;
        }
    }

    // Add scroll area (containing the chart) to container
    layout->addWidget(scrollArea);
}

void Client::loadStatistics()
{
    // Show loading state
    ui->loadStatsButton->setEnabled(false);
    ui->loadStatsButton->setText("Loading...");
    qApp->processEvents(); // Allow UI to update

    try {
        // Update client count
        updateClientCount();

        // Show growth chart
        showClientGrowthChart();

        // Restore button state
        ui->loadStatsButton->setEnabled(true);
        ui->loadStatsButton->setText("Refresh Statistics");
    } catch (...) {
        // Handle any errors
        ui->loadStatsButton->setEnabled(true);
        ui->loadStatsButton->setText("Load Statistics");
        QMessageBox::critical(this, "Error", "Failed to load statistics");
    }
}



void Client::on_callButton_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a client first");
        return;
    }

    QString phoneNumber = ui->tableWidget->item(row, 5)->text();
    QString formattedNumber = formatPhoneNumber(phoneNumber);

    // Direct WhatsApp call without app selection
    startWhatsAppCall(formattedNumber);
}

void Client::startWhatsAppCall(const QString &phoneNumber)
{
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Error", "Please select a client first");
        return;
    }

    // Get client info
    currentCallClientId = ui->tableWidget->item(row, 0)->text(); // ID in column 0 as QString
    QString clientName = ui->tableWidget->item(row, 1)->text(); // Name in column 1

    // Start timer
    callStartTime = QTime::currentTime();
    callTimer->start(1000);

    // WhatsApp call URL
    QString url = "whatsapp://call?phone=" + formatPhoneNumber(phoneNumber);

#ifdef Q_OS_WINDOWS
    QProcess::startDetached("cmd", {"/C", "start", url});
#elif defined(Q_OS_LINUX)
    QProcess::startDetached("xdg-open", {url});
#elif defined(Q_OS_MACOS)
    QProcess::startDetached("open", {url});
#endif

    // Update UI
    ui->callStatusLabel->setText("Calling " + clientName + "...");
    ui->endCallButton->setEnabled(true);
}

void Client::on_endCallButton_clicked()
{
    if (!currentCallClientId.isEmpty()) {
        callTimer->stop();
        int durationSec = callStartTime.secsTo(QTime::currentTime());

        // Save to database
        saveCallToDatabase(currentCallClientId, durationSec);

        // Update UI
        ui->callStatusLabel->setText("Call completed (" +
                                     QTime(0,0).addSecs(durationSec).toString("hh:mm:ss") + ")");
        ui->endCallButton->setEnabled(false);
        currentCallClientId.clear();
    }
}

void Client::updateCallDuration()
{
    int durationSec = callStartTime.secsTo(QTime::currentTime());
    ui->callDurationLabel->setText(QTime(0,0).addSecs(durationSec).toString("hh:mm:ss"));
}

void Client::saveCallToDatabase(const QString &clientId, int durationSec)
{
    // Generate filename with timestamp
    QString filename = QString("call_%1_%2.wav")
                           .arg(clientId)
                           .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));

    // Create full path
    QString audioPath = QDir(callSavePath).filePath(filename);

    // Verify we can write to the file
    QFile testFile(audioPath);
    if (!testFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Cannot write to:" << audioPath;
        qDebug() << "Error:" << testFile.errorString();
        return;
    }
    testFile.close();
    testFile.remove();

    // Save to database
    QSqlQuery query;
    query.prepare("INSERT INTO CLIENT_CALLS "
                  "(CLIENT_ID, CALL_DATE, DURATION, AUDIO_PATH, FORMAT, STATUS) "
                  "VALUES (:client_id, SYSTIMESTAMP, :duration, :audio_path, :format, :status)");

    query.bindValue(":client_id", clientId.toInt());
    query.bindValue(":duration", durationSec);
    query.bindValue(":audio_path", audioPath);
    query.bindValue(":format", "WAV");
    query.bindValue(":status", "COMPLETED");

    if (!query.exec()) {
        qDebug() << "Database error:" << query.lastError().text();
    }
    else {
        qDebug() << "Call saved to database. Path:" << audioPath;

        // Verify file creation after 1 second
        QTimer::singleShot(1000, [audioPath]() {
            if (QFile::exists(audioPath)) {
                qDebug() << "File successfully created";
            } else {
                qDebug() << "FILE NOT CREATED! Check permissions or storage";
            }
        });
    }
}

QString Client::formatPhoneNumber(const QString &number)
{
    QString clean = number;
    clean.remove(QRegularExpression("[^0-9+]"));

    // Tunisian number formatting
    if (clean.startsWith("00216")) {
        return "+216" + clean.mid(5);
    }
    else if (clean.startsWith("0") && clean.length() == 8) {
        return "+216" + clean.mid(1);
    }
    return clean;
}
QString Client::generateCallFilename()
{
    return QString("%1/call_%2_%3.raw")
    .arg(callSavePath)
        .arg(currentCallClientId)
        .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));
}

void Client::setupAudioPlayback()
{
    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, [this](qint64 position) {
        ui->playbackProgress->setValue(position);
    });

    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [this](qint64 duration) {
        ui->playbackProgress->setMaximum(duration);
    });
}

void Client::playRecordedCall(const QString &filePath)
{
    QString cleanPath = QDir::cleanPath(filePath);

    // Check multiple possible locations
    if (!QFile::exists(cleanPath)) {
        // Try reconstructing path from filename only
        QFileInfo fi(cleanPath);
        QString altPath = QDir(callSavePath).filePath(fi.fileName());

        if (QFile::exists(altPath)) {
            cleanPath = altPath;
        } else {
            // Show comprehensive error
            QString errorMsg = QString(
                                   "Recording not found.\n\n"
                                   "Tried locations:\n"
                                   "1. %1\n"
                                   "2. %2\n\n"
                                   "Storage directory: %3\n"
                                   "Directory contents:\n%4")
                                   .arg(filePath)
                                   .arg(altPath)
                                   .arg(callSavePath)
                                   .arg(QDir(callSavePath).entryList().join("\n"));

            QMessageBox::critical(this, "Playback Error", errorMsg);
            return;
        }
    }

    // Setup media player
    mediaPlayer->setSource(QUrl::fromLocalFile(cleanPath));
    connect(mediaPlayer, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error error, const QString &errorString) {
        qDebug() << "Playback error:" << error << errorString;
    });

    mediaPlayer->play();
}

void Client::loadCallHistory(int clientId)
{
    qDebug() << "========== Loading call history for client:" << clientId << "==========";

    // Clear existing data but preserve column structure
    ui->callHistoryTable->setRowCount(0);

    QSqlQuery query;
    QString queryStr =
        "SELECT TO_CHAR(CALL_DATE, 'YYYY-MM-DD HH24:MI:SS') as call_date, "
        "DURATION, FORMAT, STATUS, AUDIO_PATH "
        "FROM CLIENT_CALLS "
        "WHERE CLIENT_ID = :client_id "
        "ORDER BY CALL_DATE DESC";

    qDebug() << "Preparing query:" << queryStr;
    qDebug() << "With client ID:" << clientId;

    query.prepare(queryStr);
    query.bindValue(":client_id", clientId);

    if (!query.exec()) {
        qDebug() << "Query execution failed!";
        qDebug() << "Error:" << query.lastError().text();
        QMessageBox::warning(this, "Database Error",
                             "Failed to load call history:\n" + query.lastError().text());
        return;
    }

    qDebug() << "Query executed successfully. Found" << query.size() << "records";

    while (query.next()) {
        int row = ui->callHistoryTable->rowCount();
        ui->callHistoryTable->insertRow(row);

        // Date
        QString callDate = query.value("call_date").toString();
        qDebug() << "Adding call date:" << callDate;
        QTableWidgetItem *dateItem = new QTableWidgetItem(callDate);
        ui->callHistoryTable->setItem(row, 0, dateItem);

        // Duration (handle negative values)
        int seconds = query.value("DURATION").toInt();
        qDebug() << "Duration in seconds:" << seconds;
        QString duration;
        if (seconds < 0) {
            duration = "N/A";
        } else {
            duration = QTime(0, 0).addSecs(seconds).toString("hh:mm:ss");
        }
        QTableWidgetItem *durationItem = new QTableWidgetItem(duration);
        ui->callHistoryTable->setItem(row, 1, durationItem);

        // Format
        QString format = query.value("FORMAT").toString();
        qDebug() << "Format:" << format;
        QTableWidgetItem *formatItem = new QTableWidgetItem(format);
        ui->callHistoryTable->setItem(row, 2, formatItem);

        // Status
        QString status = query.value("STATUS").toString();
        qDebug() << "Status:" << status;
        QTableWidgetItem *statusItem = new QTableWidgetItem(status);
        ui->callHistoryTable->setItem(row, 3, statusItem);

        // Store audio path in the item's data
        QString audioPath = query.value("AUDIO_PATH").toString();
        qDebug() << "Audio path:" << audioPath;
        statusItem->setData(Qt::UserRole, audioPath);
    }

    if (ui->callHistoryTable->rowCount() == 0) {
        qDebug() << "No call history found for client:" << clientId;
        QMessageBox::information(this, "Information", "No call history found for this client.");
    } else {
        ui->callHistoryTable->resizeColumnsToContents();
        qDebug() << "Successfully loaded" << ui->callHistoryTable->rowCount() << "call records";
    }
}
// When a client is selected in your main table:
void Client::on_tableWidget_itemSelectionChanged()
{
    // Get all selected items
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget->selectedItems();

    if (selectedItems.isEmpty()) {
        qDebug() << "No client selected";
        return;
    }

    // Get the row of the first selected item
    int row = selectedItems.first()->row();
    qDebug() << "Selected row:" << row;

    // Get the ID from column 0 of the selected row
    QTableWidgetItem* idItem = ui->tableWidget->item(row, 0);
    if (!idItem) {
        qDebug() << "No ID item found in row" << row;
        return;
    }

    bool ok;
    int clientId = idItem->text().toInt(&ok);
    if (!ok) {
        qDebug() << "Invalid client ID in row" << row << "text:" << idItem->text();
        return;
    }

    qDebug() << "Loading call history for client ID:" << clientId;
    loadCallHistory(clientId);
}

void Client::initializeCallHistoryTable()
{
    // Clear any existing content
    ui->callHistoryTable->clear();

    // Set column headers
    QStringList headers;
    headers << "Date" << "Duration" << "Format" << "Status";
    ui->callHistoryTable->setColumnCount(headers.size());
    ui->callHistoryTable->setHorizontalHeaderLabels(headers);

    // Visual settings
    ui->callHistoryTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->callHistoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->callHistoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->callHistoryTable->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    // Set column widths
    ui->callHistoryTable->setColumnWidth(0, 180); // Date column wider
    ui->callHistoryTable->setColumnWidth(1, 100); // Duration
    ui->callHistoryTable->setColumnWidth(2, 80);  // Format
    ui->callHistoryTable->setColumnWidth(3, 120); // Status

    ui->callHistoryTable->setVisible(true);
}
void Client::onClientSelectionChanged()
{
    if (ui->tableWidget->selectedItems().isEmpty()) return;

    int row = ui->tableWidget->currentRow();
    bool ok;
    int clientId = ui->tableWidget->item(row, 0)->text().toInt(&ok);

    if (ok) {
        loadCallHistory(clientId);
    }
}
