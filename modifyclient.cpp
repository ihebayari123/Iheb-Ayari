#include "modifyclient.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

modifyclient::modifyclient(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Modifier le client");
    setStyleSheet(R"(

        QDialog {
            background-color: #f0f4f8;  /* Fond bleu très clair */
            font-family: "Segoe UI", sans-serif;
            font-size: 14px;
        }


        QLabel {
            color: #2c3e50;  /* Texte bleu foncé */
            font-weight: bold;
            background-color: transparent;  /* Arrière-plan transparent */
        }


        QLineEdit, QComboBox, QDateEdit {
            background-color: #ffffff;
            border: 1px solid #bdc3c7;
            border-radius: 5px;
            padding: 8px;
            font-size: 14px;
            color: #34495e;
        }

        QLineEdit:focus, QComboBox:focus, QDateEdit:focus {
            border: 1px solid #3498db;
            outline: none;
        }


        QPushButton {
            background-color: #3498db;
            color: #ffffff;
            border: none;
            border-radius: 5px;
            padding: 10px 20px;
            font-size: 14px;
            font-weight: bold;
            transition: background-color 0.3s ease;
        }

        QPushButton:hover {
            background-color: #2980b9;
        }

        QPushButton:pressed {
            background-color: #1c5980;
        }


        QPushButton[text="Décliner"] {
            background-color: #e74c3c;
        }

        QPushButton[text="Décliner"]:hover {
            background-color: #c0392b;
        }

        QPushButton[text="Décliner"]:pressed {
            background-color: #96281b;
        }
    )");

    // Create input fields
    lineEditNom = new QLineEdit(this);
    lineEditPrenom = new QLineEdit(this);
    lineEditEmail = new QLineEdit(this);
    lineEditAdresse = new QLineEdit(this);
    lineEditTel = new QLineEdit(this);

    // Create buttons
    btnConfirmer = new QPushButton("Confirmer", this);
    btnDecliner = new QPushButton("Décliner", this);

    // Create form layout
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Nom:", lineEditNom);
    formLayout->addRow("Prénom:", lineEditPrenom);
    formLayout->addRow("Email:", lineEditEmail);
    formLayout->addRow("Adresse:", lineEditAdresse);
    formLayout->addRow("Téléphone:", lineEditTel);

    // Create button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(btnConfirmer);
    buttonLayout->addWidget(btnDecliner);

    // Create main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);

    // Connect buttons
    connect(btnConfirmer, &QPushButton::clicked, this, &QDialog::accept);
    connect(btnDecliner, &QPushButton::clicked, this, &QDialog::reject);
}

void modifyclient::setData(const QString &nom, const QString &prenom, const QString &email, const QString &adresse, int Tel) {
    lineEditNom->setText(nom);
    lineEditPrenom->setText(prenom);
    lineEditEmail->setText(email);
    lineEditAdresse->setText(adresse);
    lineEditTel->setText(QString::number(Tel));
}

QString modifyclient::getNom() const {
    return lineEditNom->text();
}

QString modifyclient::getPrenom() const {
    return lineEditPrenom->text();
}

QString modifyclient::getEmail() const {
    return lineEditEmail->text();
}

QString modifyclient::getAdresse() const {
    return lineEditAdresse->text();
}

int modifyclient::getTel() const {
    return lineEditTel->text().toInt();
}
