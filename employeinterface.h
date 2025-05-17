#ifndef EMPLOYEINTERFACE_H
#define EMPLOYEINTERFACE_H
#include <QMainWindow>
#include <QWidget>
#include <QCamera>
#include <QImageCapture>
#include <QMediaCaptureSession>
#include <QVideoWidget>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QMediaRecorder>
#include <QAudioInput>
#include <opencv2/opencv.hpp>
#include "employe.h"
#include <QFile>

namespace Ui {
class EmployeInterface;
}

class EmployeInterface : public QMainWindow

{
    Q_OBJECT

public:
    explicit EmployeInterface(QWidget *parent = nullptr);
    ~EmployeInterface();

private slots:
    void on_btnAjouter_clicked();
    void afficherEmployes();
    void on_btnModifier_clicked();
    void on_btnSauvegarder_clicked();
    void on_btnSupprimer_clicked();
    void on_lineEditRecherche_textChanged(const QString &text);
    void on_btnExporterCSV_clicked();
    void afficherStatistiques();
    void on_btnPrendrePhoto_clicked();
    void on_btnEnregistrerVoix_clicked();
    void on_btnReconnaissance_clicked();
    bool sauvegarderAudioDepuisBase(const QString& idEmploye, const QString& cheminFichier);
    void on_btnTestExtractionVoix_clicked();
    void trierParID();

private:
    Ui::EmployeInterface *ui;
    bool isValidEmail(const QString &email);
    QSqlQueryModel *model;
    int currentEmployeeId;
    QByteArray photoData;
    QByteArray voiceData;
    QImage capturedImage;
    bool triIDCroissant = true;
    cv::Mat QtImageToMat(const QImage &image);

    QCamera *camera;
    QImageCapture *imageCapture;
    QVideoWidget *videoWidget;
    QMediaCaptureSession *captureSession;
    bool isCameraActive;

    QCamera *cameraReconnaissance;
    QImageCapture *imageCaptureReconnaissance;
    QVideoWidget *videoWidgetReconnaissance;
    QMediaCaptureSession *captureSessionReconnaissance;
    bool isReconnaissanceActive = false;
    int reconnaissanceState = 0;

    QMediaRecorder *mediaRecorder;
    QMediaRecorder *audioRecorder;
    QAudioInput *audioInput;
    QFile *audioFile;
};

#endif // EMPLOYEINTERFACE_H
