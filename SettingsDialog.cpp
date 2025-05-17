#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"
#include <QDesktopServices>
#include <QUrl>
#include <QJsonDocument>
#include <QTimer>
#include <QUrlQuery>
#include <QRandomGenerator>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QTimer>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
    , networkManager(new QNetworkAccessManager(this))
    , countdownTimer(new QTimer(this))
{
    ui->setupUi(this);

    // Use a simpler regular expression for phone number validation
    QRegularExpression phoneRegex("^\\+216[0-9]{8}$");


    QRegularExpressionValidator *phoneValidator = new QRegularExpressionValidator(phoneRegex, this);
    ui->phoneNumberEdit->setValidator(phoneValidator);

    // Connect signals
    connect(ui->radioButtonSombre, &QRadioButton::toggled, this, &SettingsDialog::onRadioButtonSombreToggled);
    connect(ui->radioButtonClair, &QRadioButton::toggled, this, &SettingsDialog::onRadioButtonClairToggled);
    ui->radioButtonClair->setChecked(true); // Default to light mode
    applyLightMode();


    connect(countdownTimer, &QTimer::timeout, this, &SettingsDialog::updateCountdown);
    connect(ui->sendCodeButton, &QPushButton::clicked, this, &SettingsDialog::on_sendCodeButton_clicked);
    disconnect(ui->verifyButton, &QPushButton::clicked, 0, 0);

    // Connect only once
    connect(ui->verifyButton, &QPushButton::clicked,
            this, &SettingsDialog::on_verifyButton_clicked);
    connect(ui->resendButton, &QPushButton::clicked, this, &SettingsDialog::on_resendButton_clicked);

    // Initialize UI state
    ui->verificationCodeEdit->setEnabled(false);
    ui->verifyButton->setEnabled(false);
    ui->resendButton->setEnabled(false);
    ui->timerLabel->setText("");

    // Set up the About Text
    QLabel *aboutText = new QLabel(this);
    aboutText->setText(
        "<h3>À propos de l'application</h3>"
        "<p><strong>Nom :</strong> StartWiser</p>"
        "<p><strong>Version :</strong> 1.0.0</p>"
        "<p><strong>Description :</strong><br>"
        "StartWiser est une application qui aide les startups à gérer leurs ressources.<br>"
        "Elle permet de se connecter avec des consultants et d'optimiser leur fonctionnement.<br>"
        "Cette application facilite la gestion des startups en leur offrant des outils puissants et une interface conviviale</p>"
        "<p><strong>Développeur :</strong>Group Innovators</p>"
        "<p><strong>Crédits :</strong> Utilise Qt pour l'interface graphique.</p>"
        "<p><strong>Site Web :</strong> <a href=\"https://handphone-blla.my.canva.site/startwise\">https://handphone-blla.my.canva.site/startwise</a></p>"
        "<p><strong>Contact :</strong> support@startWise.com</p>"
        );

    // Find the scroll area widget and add the about text
    QWidget *contentWidget = ui->WidgetContents;
    QVBoxLayout *layout = new QVBoxLayout(contentWidget);
    aboutText->setTextInteractionFlags(Qt::TextBrowserInteraction);
    aboutText->setOpenExternalLinks(true);

    // Add the QLabel to the layout
    layout->addWidget(aboutText);
    contentWidget->setLayout(layout);
}

SettingsDialog::~SettingsDialog() {
    delete ui;
    delete networkManager;
}
void SettingsDialog::on_verifyButton_clicked()
{
    // Prevent multiple verifications
    if (isVerifying) return;
    isVerifying = true;

    QString enteredCode = ui->verificationCodeEdit->text().trimmed();
    qDebug() << "Verification started - Entered:" << enteredCode << "Stored:" << generatedCode;

    if (enteredCode.isEmpty()) {
        showMessage("Veuillez entrer le code de vérification.", true);
        isVerifying = false;
        return;
    }

    if (enteredCode == generatedCode) {
        qDebug() << "Verification successful";
        showMessage("Vérification réussie! 2FA est maintenant activé.");
        ui->verificationCodeEdit->setStyleSheet("background-color: #e6f7e6;");

        // Clear code only after successful verification
        QTimer::singleShot(1000, this, [this]() {
            generatedCode.clear();
            isVerifying = false;
        });
    } else {
        qDebug() << "Verification failed";
        showMessage("Code incorrect. Veuillez réessayer.", true);
        ui->verificationCodeEdit->setStyleSheet("background-color: #ffebee;");
        isVerifying = false;
    }
}

QString SettingsDialog::generateRandomCode(int length)
{
    QString code;
    for (int i = 0; i < length; ++i) {
        code.append(QString::number(QRandomGenerator::global()->bounded(10)));
    }
    return code;
}

void SettingsDialog::sendSms(const QString &phoneNumber, const QString &message)
{

    // 1. Verify SSL support
    if (!QSslSocket::supportsSsl()) {
        qDebug() << "SSL not supported!";
        showMessage("Erreur SSL: Votre système ne supporte pas le cryptage nécessaire", true);
        return;
    }

    // 2. Configuration
    const QString fromNumber = "+13265001666";
    const QString accountSid = "AC8b86aaacb00006e8f2eeb45c50dba02f";
    const QString authToken = "bab59739fa35ce39bad9832915d1a2a4";

    // 3. Create and verify URL
    QUrl url(QString("https://api.twilio.com/2010-04-01/Accounts/%1/Messages.json").arg(accountSid));
    if (!url.isValid()) {
        qDebug() << "Invalid URL:" << url.errorString();
        return;
    }
    if (isSendingSms) return;
    isSendingSms = true;

    // 4. Prepare request
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // 5. Set authentication
    QString credentials = QString("%1:%2").arg(accountSid, authToken);
    request.setRawHeader("Authorization", "Basic " + credentials.toUtf8().toBase64());

    // 6. Prepare POST data
    QUrlQuery postData;
    postData.addQueryItem("To", phoneNumber);
    postData.addQueryItem("From", fromNumber);
    postData.addQueryItem("Body", message.left(160)); // SMS length limit

    // 7. Send request
    QNetworkReply* reply = networkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());

    // Replace the errorOccurred connection with:
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        this->onSmsResponse(reply);
    });

    // 8. Connect error signal

}

void SettingsDialog::onSmsResponse(QNetworkReply* reply)
{
    isSendingSms = false;
    QByteArray response = reply->readAll();
    qDebug() << "Twilio Response:" << response;

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Error Details:" << reply->errorString();
        showMessage("Erreur réseau: " + reply->errorString(), true);
    }
    else {
        QJsonDocument doc = QJsonDocument::fromJson(response);
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            if (obj.contains("error_code")) {
                 qDebug() <<"Erreur Twilio: "<< reply->errorString();
            }
            else {
                showMessage("SMS envoyé avec succès!", false);
                qDebug() << "SMS SID:" << obj["sid"].toString();
            }
        }
    }
    reply->deleteLater();
}
void SettingsDialog::showMessage(const QString &message, bool isError)
{
    if (isError) {
        QMessageBox::warning(this, "Erreur", message);
    } else {
        QMessageBox::information(this, "Information", message);
    }
}

void SettingsDialog::on_sendCodeButton_clicked()
{
    // Prevent multiple clicks
    if (codeGenerated) return;

    userPhoneNumber = ui->phoneNumberEdit->text().trimmed();

    if (!isValidPhoneNumber(userPhoneNumber)) {
        showMessage("Veuillez entrer un numéro de téléphone valide", true);
        return;
    }

    // Generate and store the code ONCE
    generatedCode = generateRandomCode(6);
    codeGenerated = true;
    qDebug() << "Generated verification code:" << generatedCode;

    // Disable button while sending
    ui->sendCodeButton->setEnabled(false);
    ui->resendButton->setEnabled(false);

    // Send SMS
    QString smsMessage = QString("Votre code de vérification StartWiser est: %1").arg(generatedCode);
    sendSms(userPhoneNumber, smsMessage);

    // Enable verification UI
    ui->verificationCodeEdit->setEnabled(true);
    ui->verifyButton->setEnabled(true);
    ui->verificationCodeEdit->clear();

    // Start countdown
    remainingTime = 240;
    startCountdown();
}
void SettingsDialog::on_resendButton_clicked()
{
    if (countdownTimer->isActive() && remainingTime > 120) {
        showMessage("Veuillez patienter avant de renvoyer", true);
        return;
    }
    on_sendCodeButton_clicked();
}
void SettingsDialog::startCountdown()
{
    updateCountdown(); // Immediate update
    countdownTimer->start(1000); // Update every second
}

void SettingsDialog::updateCountdown()
{
    remainingTime--;
    if (remainingTime <= 0) {
        countdownTimer->stop();
        resetVerificationUI();
        QMessageBox::warning(this, "Code expiré", "Le code de vérification a expiré");
        return;
    }

    int minutes = remainingTime / 60;
    int seconds = remainingTime % 60;
    ui->timerLabel->setText(
        QString("Code valable pendant: %1:%2")
            .arg(minutes, 2, 10, QLatin1Char('0'))
            .arg(seconds, 2, 10, QLatin1Char('0'))
        );
}

void SettingsDialog::resetVerificationUI()
{
    ui->verificationCodeEdit->clear();
    ui->verificationCodeEdit->setEnabled(false);
    ui->verifyButton->setEnabled(false);
    ui->sendCodeButton->setEnabled(true);
    ui->resendButton->setEnabled(false);
    ui->timerLabel->setText("");
    codeGenerated = false;
}

// Ensure number is in E.164 format for Tunisia
bool SettingsDialog::isValidPhoneNumber(const QString &phone)
{
    // Tunisian numbers: +216 followed by 8 digits
    QRegularExpression regex("^\\+216[0-9]{8}$");
    return regex.match(phone).hasMatch();
}
void SettingsDialog::onRadioButtonSombreToggled(bool checked)
{
    if (checked) {
        applyNightMode();
    }
}
void SettingsDialog::applyNightMode()
{
    qApp->setStyleSheet(
        "QMainWindow { background-color: #121212; color: #ffffff; }"
        "QWidget { background-color: #121212; color: #ffffff; }"
        "QPushButton { background-color: #1E1E1E; color: #ffffff; border: 1px solid #ffffff; border-radius: 5px; padding: 5px; }"
        "QPushButton:hover { background-color: #333333; }"
        "QPushButton:pressed { background-color: #555555; }"
        "QLineEdit { background-color: #1E1E1E; color: #ffffff; border: 1px solid #ffffff; border-radius: 5px; padding: 5px; }"
        "QLabel { color: #ffffff; }"
        "QRadioButton { color: #ffffff; }"
        "QCheckBox { color: #ffffff; }"
        "QComboBox { background-color: #1E1E1E; color: #ffffff; border: 1px solid #ffffff; border-radius: 5px; padding: 5px; }"
        "QComboBox QAbstractItemView { background-color: #1E1E1E; color: #ffffff; border: 1px solid #ffffff; }"
        "QTabWidget::pane { border: 1px solid #ffffff; }"
        "QTabBar::tab { background-color: #1E1E1E; color: #ffffff; padding: 5px; border: 1px solid #ffffff; border-radius: 5px; }"
        "QTabBar::tab:selected { background-color: #333333; }"
        "QMenuBar { background-color: #1E1E1E; color: #ffffff; }"
        "QMenuBar::item { background-color: #1E1E1E; color: #ffffff; padding: 5px; }"
        "QMenuBar::item:selected { background-color: #333333; }"
        "QMenu { background-color: #1E1E1E; color: #ffffff; border: 1px solid #ffffff; }"
        "QMenu::item { background-color: #1E1E1E; color: #ffffff; padding: 5px; }"
        "QMenu::item:selected { background-color: #333333; }"
        "QScrollBar:vertical { background-color: #1E1E1E; border: 1px solid #ffffff; }"
        "QScrollBar::handle:vertical { background-color: #333333; border-radius: 5px; }"
        "QScrollBar:horizontal { background-color: #1E1E1E; border: 1px solid #ffffff; }"
        "QScrollBar::handle:horizontal { background-color: #333333; border-radius: 5px; }"
        "QStatusBar { background-color: #1E1E1E; color: #ffffff; }"
        "QMessageBox { background-color: #121212; color: #ffffff; }"
        );
}

void SettingsDialog::applyLightMode()
{
    qApp->setStyleSheet(""); // Reset to default style
}
void SettingsDialog::onRadioButtonClairToggled(bool checked)
{
    if (checked) {
        applyLightMode();
    }
}
