#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QTranslator>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();
private slots:
    void on_sendCodeButton_clicked();
    void on_verifyButton_clicked();
    void on_resendButton_clicked();
    void updateCountdown();
    void onSmsResponse(QNetworkReply* reply);
    void onRadioButtonSombreToggled(bool checked); // Slot for night mode
    void onRadioButtonClairToggled(bool checked);
private:
    Ui::SettingsDialog *ui;
    bool isSendingSms = false;
    bool isVerifying = false;
    bool codeGenerated = false;
    QNetworkAccessManager *networkManager;
    QTimer *countdownTimer;
    int remainingTime;
    QString generatedCode;
    QString userPhoneNumber;
    QString phoneNumber = "+21650711639";
    void startCountdown();
    void resetVerificationUI();
    bool isValidPhoneNumber(const QString &phone);

    QString generateRandomCode(int length);
    void sendSms(const QString &phoneNumber, const QString &message);
    void showMessage(const QString &message, bool isError = false);
    void applyNightMode(); // Method to apply night mode
    void applyLightMode();

};


#endif // SETTINGSDIALOG_H
