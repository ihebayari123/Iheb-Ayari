#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVideoSink>
#include "mainwindow.h"

#include <QPixmap>
QString Login::userRole = "";
Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
    , camera(new QCamera(this))
    , captureSession(new QMediaCaptureSession(this))
    , imageCapture(new QImageCapture(this))
{
    ui->setupUi(this);
    isReadyToEnter = false;
    isCameraActive = false;

    ui->mdp->setEchoMode(QLineEdit::Password);

    // Configurer la caméra et la capture d'image
    captureSession->setCamera(camera);
    captureSession->setImageCapture(imageCapture);

    // Connecter les boutons
    connect(ui->sign_in, &QPushButton::clicked, this, &Login::handleLogin);
    connect(ui->btnFacialRecognition, &QPushButton::clicked, this, &Login::on_btnFacialRecognition_clicked);

    // Connecter le signal de capture d'image
    connect(imageCapture, &QImageCapture::imageCaptured, this, &Login::processCapturedImage);

    // Configurer le flux vidéo pour labelCameraPreview
    QVideoSink *videoSink = new QVideoSink(this);
    captureSession->setVideoSink(videoSink);
    connect(videoSink, &QVideoSink::videoFrameChanged, this, [this](const QVideoFrame &frame) {
        if (!frame.isValid())
            return;

        QImage image = frame.toImage();
        if (!image.isNull()) {
            ui->labelCameraPreview->setPixmap(QPixmap::fromImage(image).scaled(ui->labelCameraPreview->size(), Qt::KeepAspectRatio));
        }
    });
}

Login::~Login()
{
    if (camera->isActive()) {
        camera->stop();
    }
    delete camera;
    delete captureSession;
    delete imageCapture;
    delete ui;
}

void Login::handleLogin() {
    QString email = ui->email->text();
    QString password = ui->mdp->text();

    if (validateCredentials(email, password)) {
        QMessageBox::information(this, "Connexion réussie", "Vous êtes connecté avec succès !");
        accept();  // Ferme la boîte de dialogue et retourne QDialog::Accepted
    } else {
        QMessageBox::warning(this, "Échec de connexion", "Utilisateur ou mot de passe incorrect.");
    }
}

bool Login::validateCredentials(const QString &username, const QString &password) {
    QSqlQuery query;
    query.prepare("SELECT ROLE, NOM, EMAIL, IMAGE FROM EMPLOYES WHERE EMAIL = :username AND MOTDEPASS = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "Database query error:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        userRole = query.value("ROLE").toString(); // Stocke le rôle de l'utilisateur
        userName = query.value("NOM").toString();        // Stocke le nom complet
        userEmail = query.value("EMAIL").toString();     // Stocke l'email
        QByteArray photoData = query.value("IMAGE").toByteArray();
        if (!photoData.isEmpty()) {
            userPhoto = QPixmap();
            if (!userPhoto.loadFromData(photoData)) {
                qDebug() << "Failed to load photo from database.";
            }
        } else {
            qDebug() << "Photo is empty in the database.";
        }
        return true;
    }

    return false;
}

QString Login::getUserRole() const {
    return userRole;
}
QString Login::getUserName() const {
    return userName;
}

QString Login::getUserEmail() const {
    return userEmail;
}
void Login::on_btnFacialRecognition_clicked()
{
    if (camera == nullptr) {
        QMessageBox::warning(this, "Erreur", "Aucune caméra disponible !");
        return;
    }

    // Si la reconnaissance a réussi et le bouton est "Entrer"
    if (isReadyToEnter) {
        accept(); // Accepter le dialogue et continuer avec MainWindow
        return;
    }

    if (!isCameraActive) {
        // Démarrer la caméra
        camera->start();
        ui->btnFacialRecognition->setText("Capturer l'image");
        isCameraActive = true;
        return;
    }

    // Si la caméra est active, capturer une image
    if (imageCapture->isReadyForCapture()) {
        imageCapture->capture();
    } else {
        QMessageBox::warning(this, "Erreur", "La capture d'image n'est pas prête !");
    }
}

void Login::processCapturedImage(int id, const QImage &image)
{
    Q_UNUSED(id);

    if (image.isNull()) {
        ui->labelMessage->setText("Erreur : Image capturée vide.");
        return;
    }

    // Arrêter la caméra après la capture
    if (camera->isActive()) {
        camera->stop();
        isCameraActive = false;
    }

    if (performFacialRecognition(image)) {
        ui->labelMessage->setText("Reconnaissance réussie : Bienvenue " + userName);
        ui->btnFacialRecognition->setText("Entrer");
        isReadyToEnter = true;
    } else {
        ui->labelMessage->setText("Reconnaissance échouée : Aucun employé reconnu.");
        ui->btnFacialRecognition->setText("Essayer à nouveau");
        isReadyToEnter = false;
    }
}
bool Login::validateFacialRecognition(const cv::Mat &detectedFace)
{
    QSqlQuery query("SELECT NOM, PRENOM, IMAGE, ROLE, EMAIL FROM EMPLOYES");
    double bestMatch = std::numeric_limits<double>::max();
    QString bestName, bestSurname, bestRole, bestEmail;

    QByteArray bestPhotoData;
    int employeCount = 0;

    while (query.next()) {
        employeCount++;
        QByteArray photoData = query.value("IMAGE").toByteArray();
        QString nom = query.value("NOM").toString();
        QString prenom = query.value("PRENOM").toString();

        qDebug() << "Comparaison avec employé:" << nom << prenom;

        QImage dbImage;
        dbImage.loadFromData(photoData, "JPG");

        if (dbImage.isNull()) {
            qDebug() << "Image NULL pour:" << nom << prenom;
            continue;
        }

        cv::Mat dbMat = cv::Mat(dbImage.height(), dbImage.width(), CV_8UC3, const_cast<uchar*>(dbImage.bits()), dbImage.bytesPerLine()).clone();
        cv::cvtColor(dbMat, dbMat, cv::COLOR_BGR2GRAY);

        std::vector<cv::Rect> facesDB;
        cv::CascadeClassifier faceCascade;
        faceCascade.load("C:\\resources\\haarcascade_frontalface_default.xml");
        faceCascade.detectMultiScale(dbMat, facesDB, 1.1, 4);

        if (facesDB.empty()) {
            qDebug() << "Aucun visage détecté pour:" << nom << prenom;
            continue;
        }

        cv::Mat dbFace = dbMat(facesDB[0]);
        cv::resize(dbFace, dbFace, cv::Size(200, 200));

        double diff = cv::norm(detectedFace, dbFace);
        qDebug() << "Distance avec" << nom << prenom << ":" << diff;

        if (diff < bestMatch) {
            bestMatch = diff;
            bestName = query.value("NOM").toString();
            bestSurname = query.value("PRENOM").toString();
            bestRole = query.value("ROLE").toString();
            bestEmail = query.value("EMAIL").toString();
            bestPhotoData = photoData;
        }
    }

    qDebug() << "Nombre total d'employés:" << employeCount;
    qDebug() << "Meilleur match:" << bestName << bestSurname << "avec distance:" << bestMatch;

    double recognitionThreshold = 50000.0; // Ajustez ce seuil selon vos besoins
    if (bestMatch < recognitionThreshold) {
        userName = bestName + " " + bestSurname;
        userRole = bestRole;
        userEmail = bestEmail;

        if (!bestPhotoData.isEmpty()) {
            userPhoto = QPixmap();
            if (!userPhoto.loadFromData(bestPhotoData)) {
                qDebug() << "Failed to load photo from database.";
            }
        } else {
            qDebug() << "Photo is empty in the database.";
        }

        return true;
    }

    return false;
}
bool Login::performFacialRecognition(const QImage &image)
{
    if (image.isNull()) {
        qDebug() << "Erreur: L'image capturée est vide.";
        return false;
    }

    // Convertir l'image capturée en cv::Mat
    cv::Mat capturedMat = cv::Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
    if (capturedMat.empty()) {
        qDebug() << "Erreur: La conversion de QImage en cv::Mat a échoué.";
        return false;
    }

    cv::cvtColor(capturedMat, capturedMat, cv::COLOR_BGR2GRAY);

    // Charger le classificateur HaarCascade
    cv::CascadeClassifier faceCascade;
    if (!faceCascade.load("C:\\resources\\haarcascade_frontalface_default.xml")) {
        qDebug() << "Erreur: Impossible de charger le fichier HaarCascade.";
        return false;
    }

    // Détection de visage
    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(capturedMat, faces, 1.1, 4);

    if (faces.empty()) {
        qDebug() << "Aucun visage détecté.";
        return false;
    }

    cv::Mat detectedFace = capturedMat(faces[0]);
    cv::resize(detectedFace, detectedFace, cv::Size(200, 200));

    // Valider la reconnaissance faciale
    return validateFacialRecognition(detectedFace);
}
