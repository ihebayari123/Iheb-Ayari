#include "modification_consultation.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

ModifierConsultationDialog::ModifierConsultationDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Modifier la consultation");
    setStyleSheet(R"(
    /* Style  */
    ModifierConsultationDialog {
        background-color: #f0f4f8;
        font-family: "Segoe UI", sans-serif;
        font-size: 14px;
    }

    QLabel {
        color: #2c3e50;
        font-weight: bold;
        background-color: transparent;
    }

    QLineEdit, QDateTimeEdit, QComboBox {
        background-color: #ffffff;
        border: 1px solid #bdc3c7;
        border-radius: 5px;
        padding: 8px;
        font-size: 14px;
        color: #34495e;
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


    lineEditSujet = new QLineEdit(this);
    dateEditConsultation = new QDateEdit(this);
    dateEditConsultation->setCalendarPopup(true);
    lineEditHeureConsultation = new QLineEdit(this);
    comboBoxStatus = new QComboBox(this);
    comboBoxStatus->addItems({"Planifie","En cours","Termine","Annule"});


    btnConfirmer = new QPushButton("Confirmer", this);
    btnDecliner = new QPushButton("Décliner", this);


    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Sujet:", lineEditSujet);
    formLayout->addRow("Date :", dateEditConsultation);
    formLayout->addRow("Heure Consultation:", lineEditHeureConsultation);
    formLayout->addRow("Statut:", comboBoxStatus);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(btnConfirmer);
    buttonLayout->addWidget(btnDecliner);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);


    connect(btnConfirmer, &QPushButton::clicked, this, &QDialog::accept);
    connect(btnDecliner, &QPushButton::clicked, this, &QDialog::reject);
}

void ModifierConsultationDialog::setData(const CONSULTATIONS &consultation)
{

    lineEditSujet->setText(consultation.getSujet());
    dateEditConsultation->setDateTime(consultation.getConsultationDate());
    lineEditHeureConsultation->setText(consultation.getHeureConsultation());
    comboBoxStatus->setCurrentText(consultation.getStatus());
}

CONSULTATIONS ModifierConsultationDialog::getModifiedConsultation() const
{

    CONSULTATIONS modifiedConsultation;


    modifiedConsultation.setSujet(lineEditSujet->text());
    modifiedConsultation.setConsultationDate(dateEditConsultation->dateTime());
    modifiedConsultation.setHeureConsultation(lineEditHeureConsultation->text());
    modifiedConsultation.setStatus(comboBoxStatus->currentText());

    return modifiedConsultation;
}
