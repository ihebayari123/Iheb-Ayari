#include "employeinterface.h"
#include "ui_employeinterface.h"
#include <QMessageBox>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QtCharts/QtCharts>
#include <QtWidgets/QVBoxLayout>
#include <QPainter>
#include <QBuffer>
#include <QMediaRecorder>
#include <QMediaFormat>
#include <QUrl>
#include <QFileInfo>
#include <QMediaCaptureSession>
#include <QAudioInput>
#include <QTimer>
#include <QStandardPaths>
#include <QCamera>
// Pour OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>


EmployeInterface::EmployeInterface(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::EmployeInterface)
    , currentEmployeeId(0)
    , camera(nullptr)
    , imageCapture(nullptr)
    , videoWidget(nullptr)
    , captureSession(nullptr)
    , isCameraActive(false)
    , audioRecorder(nullptr)
    , audioInput(nullptr)
{
    ui->setupUi(this);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(1, Qt::AscendingOrder);  // Trie par nom au début
    connect(ui->btnTriID, &QPushButton::clicked, this, &EmployeInterface::trierParID);

    // ----- Initialisation de la caméra -----
    camera = new QCamera(this);
    imageCapture = new QImageCapture(this);
    videoWidget = new QVideoWidget(ui->videoFrame);
    captureSession = new QMediaCaptureSession(this);

    QVBoxLayout *videoLayout = new QVBoxLayout();
    videoLayout->setContentsMargins(0, 0, 0, 0);
    videoLayout->addWidget(videoWidget);
    ui->videoFrame->setLayout(videoLayout);
    videoWidget->setMinimumSize(300, 300);

    captureSession->setCamera(camera);
    captureSession->setImageCapture(imageCapture);
    captureSession->setVideoOutput(videoWidget);

    connect(imageCapture, &QImageCapture::imageCaptured, this, [this](int /*id*/, const QImage &preview) {
        QByteArray imageData;
        QBuffer buffer(&imageData);
        buffer.open(QIODevice::WriteOnly);
        preview.save(&buffer, "JPG");
        photoData = imageData;

        camera->stop();
        isCameraActive = false;
        ui->videoFrame->hide();

        QMessageBox::information(this, "Capture", "Image capturée !");
    });

    // ----- Initialisation caméra pour reconnaissance faciale -----
    cameraReconnaissance = new QCamera(this);
    imageCaptureReconnaissance = new QImageCapture(this);
    videoWidgetReconnaissance = new QVideoWidget(ui->videoReconnaissanceFrame);
    captureSessionReconnaissance = new QMediaCaptureSession(this);

    QVBoxLayout *reconLayout = new QVBoxLayout();
    reconLayout->setContentsMargins(0, 0, 0, 0);
    reconLayout->addWidget(videoWidgetReconnaissance);
    ui->videoReconnaissanceFrame->setLayout(reconLayout);
    videoWidgetReconnaissance->setMinimumSize(300, 300);

    captureSessionReconnaissance->setCamera(cameraReconnaissance);
    captureSessionReconnaissance->setImageCapture(imageCaptureReconnaissance);
    captureSessionReconnaissance->setVideoOutput(videoWidgetReconnaissance);
    reconnaissanceState = 0;

    // ----- Initialisation de l'enregistreur audio -----
    audioRecorder = new QMediaRecorder(this);
    // On place le fichier audio dans le dossier temporaire du système pour être sûr qu'il est accessible
    QString tempFilePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/voice.wav";
    audioRecorder->setOutputLocation(QUrl::fromLocalFile(tempFilePath));

    // Configuration du format audio
    {
        QMediaFormat format;
        format.setFileFormat(QMediaFormat::Mpeg4Audio); // Format m4a
        format.setAudioCodec(QMediaFormat::AudioCodec::AAC); // Codec AAC
        audioRecorder->setMediaFormat(format);
    }

    // Initialisation de l'entrée audio et liaison avec la session de capture
    audioInput = new QAudioInput(this);
    captureSession->setAudioInput(audioInput);
    captureSession->setRecorder(audioRecorder);

    // Connecter le signal recorderStateChanged pour détecter la fin d'enregistrement
    connect(audioRecorder, &QMediaRecorder::recorderStateChanged, this, [this](QMediaRecorder::RecorderState state) {
        qDebug() << "Nouvel état de l'enregistreur:" << state;
        if (state == QMediaRecorder::StoppedState) {
            QTimer::singleShot(500, this, [this]() {
                QString audioFilePath = audioRecorder->actualLocation().toLocalFile();
                qDebug() << "Lecture du fichier audio depuis:" << audioFilePath;
                QFile audioFile(audioFilePath);
                int waitTime = 0;
                while (!audioFile.exists() && waitTime < 2000) {
                    QThread::msleep(100);
                    waitTime += 100;
                    qDebug() << "En attente de la création du fichier...";
                }
                if (audioFile.exists() && audioFile.open(QIODevice::ReadOnly)) {
                    voiceData = audioFile.readAll();
                    audioFile.close();
                    QMessageBox::information(this, "Enregistrement vocal", "Voix enregistrée !");
                    qDebug() << "Taille du fichier audio lu:" << voiceData.size();
                } else {
                    QMessageBox::warning(this, "Erreur", "Impossible de lire le fichier audio.");
                    qDebug() << "Échec ouverture du fichier audio.";
                }
            });
        }
    });

    // ----- Initialisation des éléments graphiques (chart, table, etc.) -----
    if (!ui->chartLayout->layout()) {
        ui->chartLayout->addLayout(new QVBoxLayout());
    }
    connect(ui->btnExporter, &QPushButton::clicked, this, &EmployeInterface::on_btnExporterCSV_clicked);

    qDebug() << "Layout initialisé ?" << (ui->chartLayout->layout() != nullptr);
    if (!ui) {
        qDebug() << "Erreur : UI non chargée !";
        return;
    }
    qDebug() << "EmployeInterface chargée correctement.";

    model = new QSqlQueryModel();
    ui->tableView->setModel(model);

    afficherEmployes();
    afficherStatistiques();
    connect(ui->btnReconnaissance, &QPushButton::clicked, this, &EmployeInterface::on_btnReconnaissance_clicked);
}
EmployeInterface::~EmployeInterface()
{
    if (camera && camera->isActive()) {
        camera->stop();
    }
    delete ui;
}
bool EmployeInterface::isValidEmail(const QString &email) {
    QRegularExpression regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    QRegularExpressionMatch match = regex.match(email);
    return match.hasMatch();
}
void EmployeInterface::on_btnAjouter_clicked()
{
    QString nom = ui->lineEditNom->text().trimmed();
    QString prenom = ui->lineEditPrenom->text().trimmed();
    QString email = ui->lineEditEmail->text().trimmed();
    QString motdepasse = ui->lineEditMotdepasse->text().trimmed();
    QString role = ui->comboBoxRole->currentText().trimmed();

    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() ||
        motdepasse.isEmpty() || role.isEmpty()) {
        QMessageBox::warning(this, "Champ vide", "Tous les champs doivent être remplis.");
        return;
    }
    if (!isValidEmail(email)) {
        QMessageBox::warning(this, "Email invalide", "L'email fourni n'est pas valide. Vérifiez qu'il contient un '@' et un domaine valide (ex. : .com).");
        return;
    }
    if (photoData.isEmpty()) {
        QMessageBox::warning(this, "Photo manquante", "Veuillez prendre une photo avant d'ajouter l'employé.");
        return;
    }
    if (voiceData.isEmpty()) {
        QMessageBox::warning(this, "Audio manquant", "Veuillez enregistrer la voix avant d'ajouter l'employé.");
        return;
    }

    Employe employe(nom, prenom, email, motdepasse, role);
    employe.setPhoto(photoData);
    employe.setAudioData(voiceData);

    if (employe.createEmploye()) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");
        afficherEmployes();
        afficherStatistiques();
        ui->lineEditNom->clear();
        ui->lineEditPrenom->clear();
        ui->lineEditEmail->clear();
        ui->lineEditMotdepasse->clear();
        ui->comboBoxRole->setCurrentIndex(0);
        photoData.clear();
        voiceData.clear();
    } else {
        QMessageBox::critical(this, "Erreur", "L'ajout de l'employé a échoué.");
    }
}
void EmployeInterface::afficherEmployes()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open()) {
        qDebug() << "Erreur lors de l'ouverture de la connexion :" << db.lastError().text();
        return;
    }

    Employe employe;
    QSqlQuery query = employe.afficher();
    if (query.lastError().isValid()) {
        qDebug() << "Erreur lors de l'exécution de la requête d'affichage :" << query.lastError().text();
    } else {
        model->setQuery(std::move(query));
        if (model->lastError().isValid()) {
            qDebug() << "Erreur lors de la mise à jour du modèle :" << model->lastError().text();
        }
        ui->tableView->setModel(model);
        qDebug() << "Nombre d'employés récupérés :" << model->rowCount();
    }
}
void EmployeInterface::on_btnModifier_clicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Sélection", "Veuillez sélectionner un employé à modifier.");
        return;
    }
    currentEmployeeId = ui->tableView->model()->data(ui->tableView->model()->index(currentIndex.row(), 0)).toInt();
    QString nom = ui->tableView->model()->data(ui->tableView->model()->index(currentIndex.row(), 1)).toString();
    QString prenom = ui->tableView->model()->data(ui->tableView->model()->index(currentIndex.row(), 2)).toString();
    QString email = ui->tableView->model()->data(ui->tableView->model()->index(currentIndex.row(), 3)).toString();
    QString motdepasse = ui->tableView->model()->data(ui->tableView->model()->index(currentIndex.row(), 4)).toString();
    QString role = ui->tableView->model()->data(ui->tableView->model()->index(currentIndex.row(), 5)).toString();
    ui->lineEditNom->setText(nom);
    ui->lineEditPrenom->setText(prenom);
    ui->lineEditEmail->setText(email);
    ui->lineEditMotdepasse->setText(motdepasse);
    ui->comboBoxRole->setCurrentText(role);
    QMessageBox::information(this, "Pré-modification", "Les données de l'employé ont été chargées dans le formulaire.\nModifiez-les puis cliquez sur Sauvegarder.");
}
void EmployeInterface::on_btnSauvegarder_clicked()
{
    if (currentEmployeeId <= 0) {
        QMessageBox::warning(this, "Erreur", "Aucun employé n'a été sélectionné pour modification.");
        return;
    }
    QString newNom = ui->lineEditNom->text().trimmed();
    QString newPrenom = ui->lineEditPrenom->text().trimmed();
    QString newEmail = ui->lineEditEmail->text().trimmed();
    QString newMotdepasse = ui->lineEditMotdepasse->text().trimmed();
    QString newRole = ui->comboBoxRole->currentText().trimmed();
    if (newNom.isEmpty() || newPrenom.isEmpty() || newEmail.isEmpty() ||
        newMotdepasse.isEmpty() || newRole.isEmpty()) {
        QMessageBox::warning(this, "Champ vide", "Tous les champs doivent être remplis.");
        return;
    }
    if (!isValidEmail(newEmail)) {
        QMessageBox::warning(this, "Email invalide", "L'email fourni n'est pas valide.");
        return;
    }
    Employe updatedEmploye(newNom, newPrenom, newEmail, newMotdepasse, newRole);
    if (updatedEmploye.updateEmploye(currentEmployeeId)) {
        QMessageBox::information(this, "Succès", "Employé modifié avec succès !");
        afficherEmployes();
        afficherStatistiques();
    } else {
        QMessageBox::critical(this, "Erreur", "La modification a échoué.");
    }
    currentEmployeeId = 0;
}
void EmployeInterface::on_btnSupprimer_clicked()
{
    QModelIndex currentIndex = ui->tableView->currentIndex();
    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Sélection", "Veuillez sélectionner un employé à supprimer.");
        return;
    }
    int id = ui->tableView->model()->data(ui->tableView->model()->index(currentIndex.row(), 0)).toInt();
    int ret = QMessageBox::question(this, "Confirmer", "Voulez-vous vraiment supprimer cet employé ?", QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        Employe employe;
        if (employe.deleteEmploye(id)) {
            QMessageBox::information(this, "Succès", "Employé supprimé avec succès !");
            afficherEmployes();
            afficherStatistiques();
        } else {
            QMessageBox::critical(this, "Erreur", "La suppression a échoué.");
        }
    }
}
void EmployeInterface::on_lineEditRecherche_textChanged(const QString &text)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM employes WHERE nom LIKE :search OR prenom LIKE :search OR email LIKE :search");
    query.bindValue(":search", "%" + text + "%");
    if (query.exec()) {
        model->setQuery(std::move(query));
        if (model->lastError().isValid()) {
            qDebug() << "Erreur lors de la mise à jour du modèle :" << model->lastError().text();
        }
    } else {
        qDebug() << "Erreur de requête de recherche :" << query.lastError().text();
    }
}
void EmployeInterface::on_btnExporterCSV_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le fichier CSV", "", "Fichiers CSV (*.csv)");
    if (fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier pour l'écriture");
        return;
    }
    QTextStream out(&file);
    out.setAutoDetectUnicode(true);
    QAbstractItemModel *model = ui->tableView->model();
    for (int col = 0; col < model->columnCount(); ++col) {
        out << model->headerData(col, Qt::Horizontal).toString();
        if (col < model->columnCount() - 1)
            out << ",";
    }
    out << "\n";
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < model->columnCount(); ++col) {
            out << model->data(model->index(row, col)).toString();
            if (col < model->columnCount() - 1)
                out << ",";
        }
        out << "\n";
    }
    file.close();
    QMessageBox::information(this, "Exportation réussie", "Le fichier CSV a été exporté avec succès.");
}
void EmployeInterface::afficherStatistiques()
{
    QSqlQuery query;
    query.prepare("SELECT role, COUNT(*) FROM employes GROUP BY role");
    QPieSeries *series = new QPieSeries();
    if (query.exec()) {
        while (query.next()) {
            QString role = query.value(0).toString();
            int count = query.value(1).toInt();
            series->append(role, count);
        }
    } else {
        qDebug() << "Erreur lors de la récupération des statistiques :" << query.lastError().text();
        return;
    }
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des employés par rôle");
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    if (ui->chartLayout->count() > 0) {
        QLayoutItem *oldItem = ui->chartLayout->takeAt(0);
        delete oldItem->widget();
        delete oldItem;
    }
    ui->chartLayout->addWidget(chartView);
}

void EmployeInterface::on_btnPrendrePhoto_clicked()
{
    if (!isCameraActive) {
        camera->start();
        isCameraActive = true;
        ui->videoFrame->show();
    } else {
        imageCapture->capture();
    }
}
void EmployeInterface::on_btnEnregistrerVoix_clicked()
{
    if (audioRecorder->recorderState() == QMediaRecorder::RecordingState) {
        // On arrête simplement l'enregistreur.
        audioRecorder->stop();
        qDebug() << "Demande d'arrêt de l'enregistrement.";
        QString finalPath = audioRecorder->actualLocation().toLocalFile();
        qDebug() << "Fichier final enregistré à :" << finalPath;

    } else {
        // On prépare un nouveau fichier et démarre l'enregistrement
        voiceData.clear();
        QString tempFilePath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/voice.wav";
        audioRecorder->setOutputLocation(QUrl::fromLocalFile(tempFilePath));
        audioRecorder->record();
        QMessageBox::information(this, "Enregistrement vocal", "Enregistrement en cours... Cliquez de nouveau pour arrêter.");
        qDebug() << "Enregistrement démarré.";
    }
}
cv::Mat EmployeInterface::QtImageToMat(const QImage &image) {
    QImage converted = image.convertToFormat(QImage::Format_RGB888);
    return cv::Mat(converted.height(), converted.width(), CV_8UC3,
                   const_cast<uchar*>(converted.bits()), converted.bytesPerLine()).clone();
}
void EmployeInterface::on_btnReconnaissance_clicked()
{
    qDebug() << "État reconnaissance actuel:" << reconnaissanceState;

    if (reconnaissanceState == 0) {
        // Démarrer la caméra et activer l'affichage
        cameraReconnaissance->start();
        ui->videoReconnaissanceFrame->show();

        // Changer l'état et le texte du bouton
        reconnaissanceState = 1;
        ui->btnReconnaissance->setText("Capturer et comparer");
        return;
    }

    // Si l'état n'est pas à 1 (prêt à capturer), on ne fait rien
    if (reconnaissanceState != 1) {
        qDebug() << "Clic ignoré : mauvais état";
        return;
    }

    // Empêcher les connexions multiples au signal
    disconnect(imageCaptureReconnaissance, nullptr, nullptr, nullptr);

    // Connexion unique pour gérer la capture
    connect(imageCaptureReconnaissance, &QImageCapture::imageCaptured, this, [=](int id, const QImage &image) {
        capturedImage = image;

        if (capturedImage.isNull()) {
            QMessageBox::warning(this, "Erreur", "Image capturée vide.");
            return;
        }
        cv::CascadeClassifier faceCascade;
        if (!faceCascade.load("C:\\resources\\haarcascade_frontalface_default.xml")) {
            QMessageBox::critical(this, "Erreur", "Impossible de charger le fichier HaarCascade.");
            return;
        }

        // Comparaison avec la base
        QSqlQuery query("SELECT id, nom, prenom, IMAGE FROM employes");


        double seuilReconnaissance = 12300;
            // Ajuste ce seuil après quelques tests
        double meilleureDiff = std::numeric_limits<double>::max();
        QString meilleurNom, meilleurPrenom;
        bool matchTrouve = false;

        while (query.next()) {
            QByteArray photoData = query.value("IMAGE").toByteArray();
            QImage dbImage;
            dbImage.loadFromData(photoData, "JPG");

            if (dbImage.isNull())
                continue;

            cv::Mat capturedMat = QtImageToMat(capturedImage);
            cv::Mat dbMat = QtImageToMat(dbImage);

            if (capturedMat.empty() || dbMat.empty())
                continue;

            // Convertir en niveaux de gris
            cv::cvtColor(capturedMat, capturedMat, cv::COLOR_BGR2GRAY);
            cv::cvtColor(dbMat, dbMat, cv::COLOR_BGR2GRAY);

            // Détection de visage dans l'image capturée
            std::vector<cv::Rect> facesCaptured;
            faceCascade.detectMultiScale(capturedMat, facesCaptured, 1.1, 4);

            if (facesCaptured.empty())
                continue;

            cv::Mat capturedFace = capturedMat(facesCaptured[0]); // On prend le premier visage détecté

            // Détection de visage dans l'image de la base
            std::vector<cv::Rect> facesDB;
            faceCascade.detectMultiScale(dbMat, facesDB, 1.1, 4);

            if (facesDB.empty())
                continue;

            cv::Mat dbFace = dbMat(facesDB[0]);

            // Redimensionner
            cv::resize(capturedFace, capturedFace, cv::Size(200, 200));
            cv::resize(dbFace, dbFace, cv::Size(200, 200));

            // Comparaison
            double diff = cv::norm(capturedFace, dbFace);
            qDebug() << "Diff avec ID" << query.value("id").toInt() << ":" << diff;

            if (diff < meilleureDiff) {
                meilleureDiff = diff;
                meilleurNom = query.value("nom").toString();
                meilleurPrenom = query.value("prenom").toString();
            }

        }

        // Après la boucle : vérification finale
        qDebug() << "Meilleure différence trouvée :" << meilleureDiff;

        if (meilleureDiff < seuilReconnaissance) {
            QMessageBox::information(this, "Reconnaissance",
                                     "Employé reconnu : " + meilleurNom + " " + meilleurPrenom);
            matchTrouve = true;
        } else {
            QMessageBox::warning(this, "Échec", "Aucun employé reconnu.");
        }



        // Réinitialisation complète
        cameraReconnaissance->stop();
        ui->videoReconnaissanceFrame->hide();
        reconnaissanceState = 0;
        ui->btnReconnaissance->setText("Démarrer reconnaissance");
    });

    // Déclenchement de la capture
    imageCaptureReconnaissance->capture();
}
bool EmployeInterface::sauvegarderAudioDepuisBase(const QString& idEmploye, const QString& cheminFichier)
{
    QSqlQuery query;
    query.prepare("SELECT AUDIO FROM employes WHERE id = :id");
    query.bindValue(":id", idEmploye);

    if (!query.exec()) {
        qDebug() << "Erreur lors de l'exécution de la requête:" << query.lastError();
        return false;
    }

    if (query.next()) {
        QByteArray audioData = query.value(0).toByteArray();
        QFile fichierAudio(cheminFichier);

        if (fichierAudio.open(QIODevice::WriteOnly)) {
            fichierAudio.write(audioData);
            fichierAudio.close();
            qDebug() << "Audio sauvegardé dans :" << cheminFichier;
            return true;
        } else {
            qDebug() << "Impossible d'écrire le fichier audio.";
        }
    } else {
        qDebug() << "Aucun employé trouvé avec cet ID.";
    }

    return false;
}
void EmployeInterface::on_btnTestExtractionVoix_clicked()
{
    QString idEmploye = "29"; // Mets ici un ID EXISTANT dans ta base
    QString cheminFichier = "voice.wav";

    if (sauvegarderAudioDepuisBase(idEmploye, cheminFichier)) {
        QMessageBox::information(this, "Succès", "Audio extrait avec succès !");
        qDebug() << "Fichier audio enregistré à :" << cheminFichier;
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l'extraction de l'audio.");
    }
}
void EmployeInterface::trierParID()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen() && !db.open()) {
        qDebug() << "Erreur : base non ouverte pour tri";
        return;
    }

    QString ordre = triIDCroissant ? "ASC" : "DESC";
    QString requete = "SELECT * FROM employes ORDER BY ID " + ordre;

    QSqlQuery query;
    if (!query.exec(requete)) {
        qDebug() << "Erreur tri ID :" << query.lastError().text();
        return;
    }

    model->setQuery(std::move(query));
    ui->tableView->setModel(model);

    triIDCroissant = !triIDCroissant;  // On inverse l'état pour le prochain clic
}
