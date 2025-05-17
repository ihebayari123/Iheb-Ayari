#ifndef CLIENT_H
#define CLIENT_H


#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableWidgetItem>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDate>
#include <QMap>
#include <QPdfDocument>
#include <QPdfPageRenderer>
#include <QSqlError>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QBuffer>
#include <QPainter>
#include <QPainterPath>
#include "QXlsx/header/xlsxdocument.h"
#include <QtCharts>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QAction>
#include <QSqlRecord>
#include "modifyclient.h"
#include <QtConcurrent/QtConcurrent>
#include <QAudioInput>
#include <QMediaRecorder>
#include <QAudioDevice>
#include <QAudioSource>  // For Qt6 (or QAudioInput for Qt5)
#include <QMediaDevices>
#include <QUrl>
#include <QDateTime>
#include <QDesktopServices>
#include <QUrl>

#include <QProcess>

#include <QMediaPlayer>
#include <QAudioOutput>

namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();
    // Constructeurs
    explicit Client(int id, QString nom, QString prenom, QString email, QString adresse, int tel, QByteArray imageData, QWidget *parent = nullptr);

    // Getters
    QString getNom(){return nom;}
    QString getPrenom(){return prenom;}
    int getID(){return id;}
    QString getEmail(){return email;}
    QString getAdresse(){return adresse;}
    int getTel(){return tel;}
    QByteArray getImageData() { return imageData; }
    QByteArray getClientImage(int id);

    // Setters
    void SetNom(QString n){nom=n;}
    void SetPrenom(QString p){prenom=p;}
    void SetID(int id){this->id=id;}
    void SetEmail(QString e){email=e;}
    void SetAdresse(QString a){adresse=a;}
    void SetTel(int t){this->tel=t;}
    void setImageData(QByteArray data) { imageData = data; }

    // Fonctionnalités de base
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int);
    bool update(int id, const QString &nom, const QString &prenom, const QString &email, const QString &adresse, int tel);

    // Validation methods
    static bool validateNom(const QString &nom);
    static bool validatePrenom(const QString &prenom);
    static bool validateEmail(const QString &email);
    static bool validateAdresse(const QString &adresse);
    static bool validateTel(const QString &tel);

    QSqlQueryModel* rechercher(const QString& searchTerm);
    QSqlQueryModel* trierPar(const QString& column, bool ascending);
    QMap<QDate, int> getClientGrowthData();
    int countClients();
    static QString extractPdfText(const QString& filePath);
    bool analyzeCompatibility(const QString& cvPath, const QString& projectName,
                              int& score, QStringList& foundSkills, QStringList& missingSkills,
                              bool updateUI = false, int clientId = -1);
    QString generateCallFilename();
    void on_endCallButton_clicked();
    void on_callButton_clicked();
    void loadCallHistory(int clientId);
    void on_tableWidget_itemSelectionChanged();
    void initializeCallHistoryTable();
    void onClientSelectionChanged();

public slots:
    void on_addClientButton_clicked();
    void on_uploadImageButton_clicked();
    void onModifyButtonClicked();
    void onDeleteButtonClicked();
    void on_deleteImageButton_clicked();
    void on_searchButton_clicked();
    void on_searchLineEdit_returnPressed();
    void on_sortComboBox_currentIndexChanged(int index);
    void on_sortOrderButton_clicked();
    void on_pdfLabel_doubleClicked();
    void on_uploadPdfButton_clicked();
    void onPdfPageRendered(int pageNumber, QSize imageSize, const QImage &image);
    void loadStatistics();




private:
    Ui::Client *ui;
    QString nom,prenom,email,adresse;
    int id,tel;
    QByteArray imageData;
    QTableWidget *tableWidget;
    QTabWidget *tabWidget;
    bool currentSortAscending = true;
    bool chartNeedsUpdate = true;
    bool chartViewAdded = false;
    QChartView *cachedChartView = nullptr;
    bool dataLoaded = false;
    QFutureWatcher<void> loadDataWatcher;
    QString pdfFilePath;
    QPdfDocument *pdfDocument;
    QPdfPageRenderer *pageRenderer;
    bool chartLoaded = false;
    QMediaRecorder *mediaRecorder;
    QAudioInput *audioInput;
    QAudioDevice inputDevice;
    QString currentCallClientId;
    QString callSavePath = "calls/";
    QAudioSource *audioSource;  // Qt6 (use QAudioInput for Qt5)
    QIODevice *audioIODevice;
    QString currentCallClientPhone;
    QProcess *callProcess;  // Add this member
    QString formatPhoneNumber(const QString &number);
    void startWhatsAppCall(const QString &phoneNumber);
    struct CallRecord {
        QDateTime timestamp;
        QString clientName;
        QString phoneNumber;
        QString duration; // "00:00:00" format
        QString status; // "Completed", "Missed", etc.
    };

    QTimer *callTimer;
    QTime callStartTime;


    void saveCallToDatabase(const QString &clientId, int durationSec);
    void updateCallDuration();

    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;
    QString currentAudioPath;

    void setupAudioPlayback();
    void playRecordedCall(const QString &filePath);


    void setupTable();
    void refreshTable();
    void exporterVersExcel();
    void refreshTableWithSearch(const QString& searchTerm);
    void populateTableWithModel(QSqlQueryModel* model);
    void refreshTableWithSort(const QString& column, bool ascending);
    void setupDragAndDrop();
    void displayPdfInfo(const QString& filePath);
    void on_effacerCvButton_clicked();
    void loadClientImage(int clientId);
    void updateCompatibilityResults(int clientId, const QString& projectName);
    void showCompatibilityResults(int score, const QStringList& strengths, const QStringList& weaknesses);
    void updateClientCount();
    void showClientGrowthChart();

protected:
    //void showEvent(QShowEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;



};

#endif // CLIENT_H
