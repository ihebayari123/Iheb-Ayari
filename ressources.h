#ifndef RESSOURCES_H
#define RESSOURCES_H
#include <QDate>
#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QTextToSpeech>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <QDebug>
#include <QTimer>
#include <QImage>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMutex>
#include <QMainWindow>
#include <QProcess>
#include <QFuture>

namespace QXlsx { class Document; }
namespace Ui {
class Res;
}

class Res : public QDialog
{
    Q_OBJECT

private:
    int id;
    QString nom;
    QByteArray image;
    QString type;
    int quantite;
    QDate acquisitionDate;
    QString status;
    double cout;
    QPixmap currentPixmap;
    cv::Mat currentCVImage;
    QTextToSpeech *m_speech;
    cv::VideoCapture m_capture;
    QTimer* m_timer;
    cv::dnn::Net m_net;
    bool m_modelLoaded;
    void loadTemplates();  // Charger les templates au démarrage
    std::map<std::string, cv::Mat> m_templates;  // Stocke les templates (nom fichier -> image)
    std::map<std::string, cv::Mat> m_descriptors; // Stocke leurs descripteurs ORB
    std::map<std::string, std::vector<cv::KeyPoint>> m_keypoints; // Points clés des templates
    cv::Ptr<cv::ORB> m_orb;
    struct ColorRange {
        cv::Scalar lower;
        cv::Scalar upper;
        cv::Scalar display;
        std::string name;
    };

    std::vector<ColorRange> colorRanges = {
        // Rouge (optimisé)
        {{0, 150, 50}, {8, 255, 255}, cv::Scalar(0,0,255), "Rouge"},    // Rouge clair
        {{172, 150, 50}, {180, 255, 255}, cv::Scalar(0,0,255), "Rouge"},// Rouge foncé

        // Bleu (plus précis)
        {{100, 150, 50}, {130, 255, 255}, cv::Scalar(255,0,0), "Bleu"},

        // Vert (resserré)
        {{40, 70, 50}, {80, 255, 255}, cv::Scalar(0,255,0), "Vert"},
        };
    bool m_useCloudDetection = false;
      // New slot for API responses
    bool m_apiRequestPending = false;  // To prevent overlapping requests
    QNetworkAccessManager* m_networkManager;
    cv::Mat m_lastFrame;
    QMutex m_frameMutex;
    QMutex m_apiMutex;
    cv::dnn::Net net;
    std::vector<std::string> classNames;
    cv::dnn::Net m_yoloNet;
    std::vector<std::string> m_classNames;
    QMutex m_detectionMutex;
    bool m_yoloLoaded = false;
    QTextDocument m_statsDocument; // Pour stocker le contenu HTML
    void paintStatsWidget();
    QString m_statsTextToSpeak;
    enum class CameraSource { PC, PHONE };
    CameraSource m_currentCameraSource = CameraSource::PC;
     // Default IP Webcam URL
    cv::VideoCapture m_phoneCapture;
    QProcess m_adbProcess;
    void setupUSBConnection();
    QString m_phoneCameraUrl;
    QList<QFuture<void>> m_detectionFutures;








public:
    // Constructeurs
    Res() {}
    Res(int, QString, QByteArray, QString, int, QDate, QString, double);
    QXlsx::Document* doc;

    explicit Res(QWidget *parent = nullptr);

    // Getters
    int get_ID() { return id; }
    QString get_Nom() { return nom; }
    QByteArray get_Image() { return image; }
    QString get_Type() { return type; }
    int get_Quantite() { return quantite; }
    QDate get_AcquisitionDate() { return acquisitionDate; }
    QString get_Status() { return status; }
    double get_Cout() { return cout; }

    // Setters
    void set_ID(int id) { this->id = id; }
    void set_Nom(QString n) { nom = n; }
    void set_Image(QByteArray img) { image = img; }
    void set_Type(QString t) { type = t; }
    void set_Quantite(int q) { quantite = q; }
    void set_AcquisitionDate(QDate d) { acquisitionDate = d; }
    void set_Status(QString s) { status = s; }
    void set_Cout(double c) { cout = c; }

    // Fonctionnalités CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool modifier(int);
    void on_btnImage_clicked();

    void onBtnAjouterClicked();
    void updateTable();
    void onBtnModifierClicked();
    void onBtnSupprimerClicked();
    void exporterVersExcel();
    void exporterPDF();
    void updateChart();
    void clearChart();
    void filtrerParNom(const QString &text);
    void onBtnRechercherClicked();
    void onBtnResetRechercheClicked();
    bool isIdUnique(int id);
    void setupSpeechSystem();

    //void updateVoices();

    void onVoiceGenderSelected();
    void checkAvailableEngines();
    void Vider_champs();
    void clearErrorIndicators();
    void showError(QWidget* widget, const QString& message, QLabel* errorLabel);
    void validateID();
    void validateName();
    void validateQuantite();
    void validateCout();
    void validateType();
    void detectObjects(cv::Mat& frame);
    void on_btnWebcamOpen_clicked();
    void processFrame();
    void detectObjectsEnhanced(cv::Mat &frame);
    std::string getColorName(cv::Scalar hsvColor);
    cv::Scalar findDominantColor(cv::Mat &roi);
    std::string detectShape(const std::vector<cv::Point>& contour);
    void on_btnUpload_clicked();
    void loadYOLOModel();
    void detectObjectsYOLO(cv::Mat &frame);
    void cleanupYOLO();
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void processDroppedImage(const QString &filePath);
    void genererStatistiquesTextuelles();
    QString genererAnalyseStatistique();
    QMap<QString, int> calculerRessourcesParType();
    QPair<int, int>calculerEtatRessources();
    int calculerNombreTotalRessources();
    bool eventFilter(QObject *watched, QEvent *event);
    void updateStats();
    void setupSpeechSystem2();
    void lireStatistiques();
    void onSpeechStateChanged(QTextToSpeech::State state);
    void on_record_phone_clicked();
    void displayFrame(const cv::Mat& frame);
    void processFrameWithYOLO(cv::Mat& frame);
    void cleanupFinishedFutures();

    // Notez le 'const' ajouté pour cv::Mat







    ~Res();  // Déclaration du destructeur

private:
    Ui::Res *ui;
    QChartView *chartView;
    QBarSet *set0;
    QBarSet *set1;
    QBarSeries *series;
    QChart *chart;


    void createChart();
    void animateChart();
};

#endif // RESSOURCES_H
