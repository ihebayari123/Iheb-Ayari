#ifndef GESTION_PROJET_H
#define GESTION_PROJET_H

#include <QDialog>

#include <QMainWindow>
#include <QModelIndex>
#include <QFileDialog>
#include <QDesktopServices>
#include <QQuickWidget>
#include <QUrl>
#include <QtCharts/QChartView> // Inclure QtCharts pour les graphiques
#include "connection.h"
#include "projet.h"
#include <QDialog>
#include <QPrinter>
#include <QPainter>
#include <QPixmap>
#include <QGeoCoordinate>
#include <QGeoPositionInfoSource>
//#include "mainwindow.h"
#include "mainmap.cpp"
#include "arduin.h"
#include <QSqlRecord>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QBuffer>


QT_BEGIN_NAMESPACE

namespace Ui {
class gestion_projet;
}
QT_END_NAMESPACE

class gestion_projet : public QDialog
{
    Q_OBJECT

public:
    explicit gestion_projet(QWidget *parent = nullptr);
    ~gestion_projet();
    void afficher(const QString& sortOrder) ;
    void loadProjectNames();
    QString getClientProjects(int clientId);
    QString getClientInfo(int clientId);
    void checkKeypadInput();
    void displayGeneratedImage(const QString &base64Image);
    void generateImage(const QString &prompt);
    void sendToOpenAI(const QString &message);

public slots:
    void refreshMap();

private slots:
    void on_ajouter_clicked();  // Slot pour ajouter un projet
    void on_supprimer_clicked();  // Slot pour supprimer un projet
    void on_tableView_activated(const QModelIndex &index);  // Slot pour interagir avec la tableView
    void on_modifier_clicked();  // Slot pour modifier un projet
    void on_UploadPDF_clicked();  // Slot pour uploader un PDF
    void on_OuvrirPDF_clicked();  // Slot pour ouvrir un PDF
    void on_etatComboBox_changed(int index);
    // Slot pour filtrer par état
    void on_rechercherButton_clicked(); // Slot pour la recherche par nom
    void on_rechercheLineEdit_textChanged(const QString& text); // Slot pour les changements de texte
    void on_stat_clicked() ;
    void genererPDFProjets();
    void on_searchLocationButton_clicked();
    void on_projectNameComboBox_currentIndexChanged(int index);
    QGeoCoordinate geocodeLocation(const QString &locationName);
    void on_sendButton_clicked();


    void on_send_clicked();

private:
    Ui::gestion_projet *ui;
    Connection cnx;  // Connexion à la base de données
    QString pdfFilePath;  // Chemin du fichier PDF sélectionné
    Projet projet;  // Objet Projet pour gérer les opérations sur les projets
    arduin arduino;
    QByteArray data;
    QQuickWidget *m_qmlMapWidget = nullptr; // Always initialize to nullptr
    void setupMap();         // Méthode pour afficher les statistiques sous forme de graphique
    void afficherStatistiques();
    QMap<QString, QStringList> responses;
    void initializeChatbotResponses();
    QString getChatbotResponse(const QString &message);
    void onUserMessageSent();
    QNetworkAccessManager *networkManager;
    QString openAIKey; // Store your OpenAI API key
    QString stabilityAIKey; // Store your Stability AI API key



};

#endif // MAINWINDOW_H


