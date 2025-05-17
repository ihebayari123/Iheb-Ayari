#include "modifierressourcedialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>

ModifierRessourceDialog::ModifierRessourceDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Modifier la ressource");
    setStyleSheet(R"(
/* Style général de la fenêtre */
ModifierRessourceDialog {
    background-color: #f0f4f8;  /* Fond bleu très clair */
    font-family: "Segoe UI", sans-serif;
    font-size: 14px;
}

/* Style des labels */
QLabel {
    color: #2c3e50;  /* Texte bleu foncé */
    font-weight: bold;
    background-color: transparent;  /* Arrière-plan transparent */
}

/* Style des champs de saisie */
QLineEdit, QComboBox, QDateEdit {
    background-color: #ffffff;  /* Fond blanc */
    border: 1px solid #bdc3c7;  /* Bordure grise */
    border-radius: 5px;  /* Coins arrondis */
    padding: 8px;
    font-size: 14px;
    color: #34495e;  /* Texte bleu foncé */
}

QLineEdit:focus, QComboBox:focus, QDateEdit:focus {
    border: 1px solid #3498db;  /* Bordure bleue lorsqu'en focus */
    outline: none;
}

/* Style des boutons radio */
QRadioButton {
    color: #2c3e50;  /* Texte bleu foncé */
    font-weight: normal;
    background-color: transparent;  /* Arrière-plan transparent */
}

QRadioButton::indicator {
    width: 16px;
    height: 16px;
    border: 1px solid #bdc3c7;  /* Bordure grise */
    border-radius: 8px;  /* Cercle */
}

QRadioButton::indicator:checked {
    background-color: #3498db;  /* Fond bleu lorsqu'activé */
    border: 1px solid #3498db;
}

/* Style des boutons */
QPushButton {
    background-color: #3498db;  /* Fond bleu */
    color: #ffffff;  /* Texte blanc */
    border: none;
    border-radius: 5px;  /* Coins arrondis */
    padding: 10px 20px;
    font-size: 14px;
    font-weight: bold;
    transition: background-color 0.3s ease;  /* Transition fluide */
}

QPushButton:hover {
    background-color: #2980b9;  /* Fond bleu plus foncé au survol */
}

QPushButton:pressed {
    background-color: #1c5980;  /* Fond bleu encore plus foncé lors du clic */
}

/* Style spécifique pour le bouton "Décliner" */
QPushButton[text="Décliner"] {
    background-color: #e74c3c;  /* Fond rouge */
}

QPushButton[text="Décliner"]:hover {
    background-color: #c0392b;  /* Fond rouge plus foncé au survol */
}

QPushButton[text="Décliner"]:pressed {
    background-color: #96281b;  /* Fond rouge encore plus foncé lors du clic */
}

/* Style du calendrier dans QDateEdit */
QCalendarWidget {
    background-color: #ffffff;  /* Fond blanc */
    border: 1px solid #bdc3c7;  /* Bordure grise */
    border-radius: 5px;  /* Coins arrondis */
}

QCalendarWidget QToolButton {
    background-color: #3498db;  /* Fond bleu */
    color: #ffffff;  /* Texte blanc */
    border-radius: 5px;  /* Coins arrondis */
    padding: 5px;
}

QCalendarWidget QToolButton:hover {
    background-color: #2980b9;  /* Fond bleu plus foncé au survol */
}

QCalendarWidget QMenu {
    background-color: #ffffff;  /* Fond blanc */
    border: 1px solid #bdc3c7;  /* Bordure grise */
}

QCalendarWidget QSpinBox {
    background-color: #ffffff;  /* Fond blanc */
    border: 1px solid #bdc3c7;  /* Bordure grise */
    border-radius: 5px;  /* Coins arrondis */
    padding: 5px;
}

QCalendarWidget QAbstractItemView {
    background-color: #ffffff;  /* Fond blanc */
    color: #2c3e50;  /* Texte bleu foncé */
    selection-background-color: #3498db;  /* Fond bleu pour la sélection */
    selection-color: #ffffff;  /* Texte blanc pour la sélection */
}
    )");
    // Créer les champs de saisie
    lineEditNom = new QLineEdit(this);
    comboBoxType = new QComboBox(this);
    comboBoxType->addItems({"Équipements informatiques", "mobilier bureautique", "fornitures de bureau"});
    lineEditQuantite = new QLineEdit(this);
    dateEditAcquisition = new QDateEdit(this);
    dateEditAcquisition->setCalendarPopup(true);
    radioButtonEnPanne = new QRadioButton("En Panne", this);
    radioButtonEnmarche = new QRadioButton("En marche", this);
    lineEditCout = new QLineEdit(this);

    // Créer les boutons
    btnConfirmer = new QPushButton("Confirmer", this);
    btnDecliner = new QPushButton("Décliner", this);

    // Disposition des widgets
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Nom:", lineEditNom);
    formLayout->addRow("Type:", comboBoxType);
    formLayout->addRow("Quantité:", lineEditQuantite);
    formLayout->addRow("Date d'acquisition:", dateEditAcquisition);
    formLayout->addRow("Statut:", radioButtonEnPanne);
    formLayout->addRow("", radioButtonEnmarche);
    formLayout->addRow("Coût:", lineEditCout);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(btnConfirmer);
    buttonLayout->addWidget(btnDecliner);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);

    // Connexion des boutons
    connect(btnConfirmer, &QPushButton::clicked, this, &QDialog::accept);
    connect(btnDecliner, &QPushButton::clicked, this, &QDialog::reject);
}

void ModifierRessourceDialog::setData(const QString &nom, const QString &type, int quantite, const QDate &date, const QString &status, double cout)
{
    lineEditNom->setText(nom);//Remplir le champ "Nom" avec la valeur existante.
    comboBoxType->setCurrentText(type);
    lineEditQuantite->setText(QString::number(quantite));
    dateEditAcquisition->setDate(date);
    if (status == "En Panne") {
        radioButtonEnPanne->setChecked(true);
    } else {
        radioButtonEnmarche->setChecked(true);
    }
    lineEditCout->setText(QString::number(cout));
}

QString ModifierRessourceDialog::getNom() const
{
    return lineEditNom->text();// Retourne le texte contenu dans le champ "Nom".
}

QString ModifierRessourceDialog::getType() const
{
    return comboBoxType->currentText();// Retourne le texte de l'élément actuellement sélectionné dans la liste déroulante "Type".
}

int ModifierRessourceDialog::getQuantite() const
{
    return lineEditQuantite->text().toInt();// Convertit le texte du champ "Quantité" en entier et le retourne.
}

QDate ModifierRessourceDialog::getDate() const
{
    return dateEditAcquisition->date();
}

QString ModifierRessourceDialog::getStatus() const
{
    return radioButtonEnPanne->isChecked() ? "En Panne" : "En marche";
}

double ModifierRessourceDialog::getCout() const
{
    return lineEditCout->text().toDouble();
}
