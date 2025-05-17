#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QImageCapture>
#include <QImage>
#include <QTimer>
#include <QPixmap>
#include <opencv2/opencv.hpp>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    static QString userRole;
    QString getUserRole() const;

    QString getUserName() const;
    QString getUserEmail() const;
    QPixmap getUserPhoto() const { return userPhoto; }
    void handleRecognitionResult(bool success, const QString &name, const QString &role, const QString &email);
    bool validateFacialRecognition(const cv::Mat &detectedFace);

signals:
    void startRecognition();

private slots:
    void on_btnFacialRecognition_clicked();
    void handleLogin(); // Slot pour gérer le clic sur le bouton "Sign In"
    void processCapturedImage(int id, const QImage &image); // Nouveau slot pour traiter l'image capturée

private:
    Ui::Login *ui;
    QThread *workerThread;

    bool isReadyToEnter;
    bool validateCredentials(const QString &email, const QString &password);
    bool performFacialRecognition(const cv::Mat &frame); // Vérifie les identifiants dans la base de données
    bool performFacialRecognition(const QImage &image);
    void displayMessage(const QString &message, bool isError = false);
    bool isCameraActive;

    QCamera *camera;
    QMediaCaptureSession *captureSession;
    QImageCapture *imageCapture; // Nouveau membre pour capturer des images
    QTimer *recognitionTimer;
    QString userName;  // Nom complet de l'utilisateur
    QString userEmail; // Email de l'utilisateur
    QPixmap userPhoto;
    bool isRecognized;
};

#endif // LOGIN_H
