#include <QMessageBox>
#include "window.h"
#include "connection.h"
#include "modification_consultation.h"
#include <QCalendarWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLocale>
#include <QTextCharFormat>

//! [0]
// Constructeur principal de la fenêtre, initialise et organise les composants principaux de l'interface
Window::Window(QWidget *parent)
    : QWidget(parent)
{
    createPreviewGroupBox(); // Crée le groupe de prévisualisation du calendrier
    createGeneralOptionsGroupBox(); // Crée le groupe des options générales
    createDatesGroupBox(); // Crée le groupe pour les dates min/max et date sélectionnée
    createTextFormatsGroupBox(); // Crée le groupe des formats de texte

    QGridLayout *layout = new QGridLayout; // Mise en page générale de la fenêtre
    layout->addWidget(previewGroupBox, 0, 0);
    layout->addWidget(generalOptionsGroupBox, 0, 1);
    layout->addWidget(datesGroupBox, 1, 0);
    layout->addWidget(textFormatsGroupBox, 1, 1);
    layout->setSizeConstraint(QLayout::SetFixedSize); // Fixe la taille de la fenêtre en fonction du contenu
    setLayout(layout);

    previewLayout->setRowMinimumHeight(0, calendar->sizeHint().height()); // Fixe la hauteur minimale de la ligne de prévisualisation
    previewLayout->setColumnMinimumWidth(0, calendar->sizeHint().width()); // Fixe la largeur minimale de la colonne de prévisualisation

    setWindowTitle(tr("Calendar Widget")); // Titre de la fenêtre
}
//! [0]


void Window::localeChanged(int index)
{
    const QLocale newLocale(localeCombo->itemData(index).toLocale());
    calendar->setLocale(newLocale);
    int newLocaleFirstDayIndex = firstDayCombo->findData(newLocale.firstDayOfWeek());
    firstDayCombo->setCurrentIndex(newLocaleFirstDayIndex);
}

//! [1]
void Window::firstDayChanged(int index)
{
    calendar->setFirstDayOfWeek(Qt::DayOfWeek(
        firstDayCombo->itemData(index).toInt()));
}
//! [1]

void Window::selectionModeChanged(int index)
{
    calendar->setSelectionMode(QCalendarWidget::SelectionMode(
        selectionModeCombo->itemData(index).toInt()));
}

void Window::horizontalHeaderChanged(int index)
{
    calendar->setHorizontalHeaderFormat(QCalendarWidget::HorizontalHeaderFormat(
        horizontalHeaderCombo->itemData(index).toInt()));
}

void Window::verticalHeaderChanged(int index)
{
    calendar->setVerticalHeaderFormat(QCalendarWidget::VerticalHeaderFormat(
        verticalHeaderCombo->itemData(index).toInt()));
}

//! [2]
void Window::selectedDateChanged()
{
    currentDateEdit->setDate(calendar->selectedDate());

    QDate selectedDate = calendar->selectedDate(); // استخدم calendar من Window

    if (!selectedDate.isValid()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une date !");
        return;
    }


    QSqlQuery query;
    query.prepare("SELECT ID, SUJET, CONSULTATIONDATE, HEURECONSULTATION, STATUS FROM CONSULTATIONS WHERE CONSULTATIONDATE = :date");
    query.bindValue(":date", selectedDate);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Aucune consultation", "Aucune consultation trouvée pour cette date !");
        return;
    }

    CONSULTATIONS consultation;
    consultation.setID(query.value(0).toInt());
    consultation.setSujet(query.value(1).toString());
    consultation.setConsultationDate(query.value(2).toDateTime());
    consultation.setHeureConsultation(query.value(3).toString());
    consultation.setStatus(query.value(4).toString());

    ModifierConsultationDialog dialog(this);
    dialog.setData(consultation);

    if (dialog.exec() == QDialog::Accepted) {
        CONSULTATIONS modifiedConsultation = dialog.getModifiedConsultation();

        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE CONSULTATIONS SET SUJET = :sujet, CONSULTATIONDATE = TO_DATE(:date, 'YYYY-MM-DD'), HEURECONSULTATION = :heure, STATUS = :status WHERE ID = :id");
        updateQuery.bindValue(":sujet", modifiedConsultation.getSujet());
        updateQuery.bindValue(":date", modifiedConsultation.getConsultationDate().toString("yyyy-MM-dd"));
        updateQuery.bindValue(":heure", modifiedConsultation.getHeureConsultation());
        updateQuery.bindValue(":status", modifiedConsultation.getStatus());
        updateQuery.bindValue(":id", modifiedConsultation.getID());

        if (!updateQuery.exec()) {
            QMessageBox::critical(this, "Erreur SQL", updateQuery.lastError().text());
        } else {
            QMessageBox::information(this, "Succès", "Consultation modifiée avec succès !");


        }
    }
}



//! [2]

//! [3]
void Window::minimumDateChanged(QDate date)
{
    calendar->setMinimumDate(date);
    maximumDateEdit->setDate(calendar->maximumDate());
}
//! [3]

//! [4]
void Window::maximumDateChanged(QDate date)
{
    calendar->setMaximumDate(date);
    minimumDateEdit->setDate(calendar->minimumDate());
}
//! [4]

//! [5]
void Window::ConsultationDatesFormatChanged()
{
    // Création d'un objet de format de texte (pour le calendrier)
    QTextCharFormat format;

    // Définir la couleur du texte à partir de la couleur sélectionnée dans la ComboBox
    format.setForeground(qvariant_cast<QColor>(
        ConsultationDatesColorCombo->itemData(ConsultationDatesColorCombo->currentIndex())));

    // Exécution d'une requête SQL pour récupérer les dates uniques de consultations
    QSqlQuery query;
    if (query.exec("SELECT DISTINCT CONSULTATIONDATE FROM consultations")) {
        // Pour chaque date obtenue, appliquer le format (couleur) dans le calendrier
        while (query.next()) {
            QDate date = query.value(0).toDate();
            calendar->setDateTextFormat(date, format); // Appliquer le format à la date dans le calendrier
        }
    } else {
        // Affichage d'une erreur si la requête échoue
        qDebug() << "Erreur SQL:" << query.lastError().text();
    }
}

//! [5]

//! [6]
void Window::weekendFormatChanged()
{
    QTextCharFormat format;

    format.setForeground(qvariant_cast<QColor>(
        weekendColorCombo->itemData(weekendColorCombo->currentIndex())));
    calendar->setWeekdayTextFormat(Qt::Saturday, format);
    calendar->setWeekdayTextFormat(Qt::Sunday, format);
}
//! [6]

//! [7]
void Window::reformatHeaders()
{
    QString text = headerTextFormatCombo->currentText();
    QTextCharFormat format;

    if (text == tr("Bold"))
        format.setFontWeight(QFont::Bold);
    else if (text == tr("Italic"))
        format.setFontItalic(true);
    else if (text == tr("Green"))
        format.setForeground(Qt::green);
    calendar->setHeaderTextFormat(format);
}
//! [7]

//! [8]
void Window::reformatCalendarPage()
{
    QTextCharFormat mayFirstFormat;
    const QDate mayFirst(calendar->yearShown(), 5, 1);

    QTextCharFormat firstFridayFormat;
    QDate firstFriday(calendar->yearShown(), calendar->monthShown(), 1);
    while (firstFriday.dayOfWeek() != Qt::Friday)
        firstFriday = firstFriday.addDays(1);

    if (firstFridayCheckBox->isChecked()) {
        firstFridayFormat.setForeground(Qt::blue);
    } else { // Revert to regular colour for this day of the week.
        Qt::DayOfWeek dayOfWeek(static_cast<Qt::DayOfWeek>(firstFriday.dayOfWeek()));
        firstFridayFormat.setForeground(calendar->weekdayTextFormat(dayOfWeek).foreground());
    }

    calendar->setDateTextFormat(firstFriday, firstFridayFormat);

    // When it is checked, "May First in Red" always takes precedence over "First Friday in Blue".
    if (mayFirstCheckBox->isChecked()) {
        mayFirstFormat.setForeground(Qt::red);
    } else if (!firstFridayCheckBox->isChecked() || firstFriday != mayFirst) {
        // We can now be certain we won't be resetting "May First in Red" when we restore
        // may 1st's regular colour for this day of the week.
        Qt::DayOfWeek dayOfWeek(static_cast<Qt::DayOfWeek>(mayFirst.dayOfWeek()));
        calendar->setDateTextFormat(mayFirst, calendar->weekdayTextFormat(dayOfWeek));
    }

    calendar->setDateTextFormat(mayFirst, mayFirstFormat);
    ConsultationDatesFormatChanged();

}
//! [8]

//! [9]
void Window::createPreviewGroupBox()
{
    previewGroupBox = new QGroupBox(tr("Preview"));

    calendar = new QCalendarWidget;
    calendar->setMinimumDate(QDate(1900, 1, 1));
    calendar->setMaximumDate(QDate(3000, 1, 1));
    calendar->setGridVisible(true);
    calendar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    connect(calendar, &QCalendarWidget::currentPageChanged,
            this, &Window::reformatCalendarPage);




    previewLayout = new QGridLayout;
    previewLayout->setContentsMargins(0, 0, 0, 0);
    previewLayout->addWidget(calendar, 0, 0, Qt::AlignTop | Qt::AlignLeft);
    previewGroupBox->setLayout(previewLayout);
}
//! [9]

// TODO: use loc.name() as label (but has underscore in place of slash)
// TODO: use locale() == loc instead of only comparing language and territory
// Needs someone familiar with this example to work out ramifications
//! [10]
void Window::createGeneralOptionsGroupBox()
{
    // Création d'un groupe contenant les options générales
    generalOptionsGroupBox = new QGroupBox(tr("General Options"));

    // Création de la liste déroulante pour le paramètre régional (locale)
    localeCombo = new QComboBox;
    int curLocaleIndex = -1;
    int index = 0;

    // Remplissage de la liste des langues et territoires disponibles
    for (int _lang = QLocale::C; _lang <= QLocale::LastLanguage; ++_lang) {
        QLocale::Language lang = static_cast<QLocale::Language>(_lang);
        const auto locales =
            QLocale::matchingLocales(lang, QLocale::AnyScript, QLocale::AnyTerritory);
        for (auto loc : locales) {
            QString label = QLocale::languageToString(lang);
            auto territory = loc.territory();
            label += QLatin1Char('/');
            label += QLocale::territoryToString(territory);

            // Sauvegarde de l’index si c’est la locale actuelle
            if (locale().language() == lang && locale().territory() == territory)
                curLocaleIndex = index;

            localeCombo->addItem(label, loc);
            ++index;
        }
    }

    // Sélection de la locale actuelle dans la liste
    if (curLocaleIndex != -1)
        localeCombo->setCurrentIndex(curLocaleIndex);

    // Création du label pour la locale
    localeLabel = new QLabel(tr("&Locale"));
    localeLabel->setBuddy(localeCombo);

    // Création de la liste déroulante pour choisir le premier jour de la semaine
    firstDayCombo = new QComboBox;
    firstDayCombo->addItem(tr("Sunday"), Qt::Sunday);
    firstDayCombo->addItem(tr("Monday"), Qt::Monday);
    firstDayCombo->addItem(tr("Tuesday"), Qt::Tuesday);
    firstDayCombo->addItem(tr("Wednesday"), Qt::Wednesday);
    firstDayCombo->addItem(tr("Thursday"), Qt::Thursday);
    firstDayCombo->addItem(tr("Friday"), Qt::Friday);
    firstDayCombo->addItem(tr("Saturday"), Qt::Saturday);

    // Label pour le choix du premier jour de la semaine
    firstDayLabel = new QLabel(tr("Wee&k starts on:"));
    firstDayLabel->setBuddy(firstDayCombo);

    // Création de la liste pour le mode de sélection dans le calendrier
    selectionModeCombo = new QComboBox;
    selectionModeCombo->addItem(tr("Single selection"),
                                QCalendarWidget::SingleSelection);
    selectionModeCombo->addItem(tr("None"), QCalendarWidget::NoSelection);

    // Label pour le mode de sélection
    selectionModeLabel = new QLabel(tr("&Selection mode:"));
    selectionModeLabel->setBuddy(selectionModeCombo);

    // Case à cocher pour afficher ou non la grille
    gridCheckBox = new QCheckBox(tr("&Grid"));
    gridCheckBox->setChecked(calendar->isGridVisible());

    // Case à cocher pour activer la barre de navigation
    navigationCheckBox = new QCheckBox(tr("&Navigation bar"));
    navigationCheckBox->setChecked(true);

    // Liste déroulante pour le type d'entête horizontal (jours de la semaine)
    horizontalHeaderCombo = new QComboBox;
    horizontalHeaderCombo->addItem(tr("Single letter day names"),
                                   QCalendarWidget::SingleLetterDayNames);
    horizontalHeaderCombo->addItem(tr("Short day names"),
                                   QCalendarWidget::ShortDayNames);
    horizontalHeaderCombo->addItem(tr("None"),
                                   QCalendarWidget::NoHorizontalHeader);
    horizontalHeaderCombo->setCurrentIndex(1);

    // Label de l'entête horizontal
    horizontalHeaderLabel = new QLabel(tr("&Horizontal header:"));
    horizontalHeaderLabel->setBuddy(horizontalHeaderCombo);

    // Liste déroulante pour l'entête vertical (n° de semaine)
    verticalHeaderCombo = new QComboBox;
    verticalHeaderCombo->addItem(tr("ISO week numbers"),
                                 QCalendarWidget::ISOWeekNumbers);
    verticalHeaderCombo->addItem(tr("None"), QCalendarWidget::NoVerticalHeader);

    // Label de l'entête vertical
    verticalHeaderLabel = new QLabel(tr("&Vertical header:"));
    verticalHeaderLabel->setBuddy(verticalHeaderCombo);

    // Connexion des signaux aux slots correspondants pour mettre à jour l'interface
    connect(localeCombo, &QComboBox::currentIndexChanged,
            this, &Window::localeChanged);
    connect(firstDayCombo, &QComboBox::currentIndexChanged,
            this, &Window::firstDayChanged);
    connect(selectionModeCombo, &QComboBox::currentIndexChanged,
            this, &Window::selectionModeChanged);
    connect(gridCheckBox, &QCheckBox::toggled,
            calendar, &QCalendarWidget::setGridVisible);
    connect(navigationCheckBox, &QCheckBox::toggled,
            calendar, &QCalendarWidget::setNavigationBarVisible);
    connect(horizontalHeaderCombo, &QComboBox::currentIndexChanged,
            this, &Window::horizontalHeaderChanged);
    connect(verticalHeaderCombo, &QComboBox::currentIndexChanged,
            this, &Window::verticalHeaderChanged);

    // Création d'une mise en page horizontale pour les cases à cocher
    QHBoxLayout *checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addWidget(gridCheckBox);
    checkBoxLayout->addStretch();
    checkBoxLayout->addWidget(navigationCheckBox);

    // Création de la mise en page principale du groupe
    QGridLayout *outerLayout = new QGridLayout;
    outerLayout->addWidget(localeLabel, 0, 0);
    outerLayout->addWidget(localeCombo, 0, 1);
    outerLayout->addWidget(firstDayLabel, 1, 0);
    outerLayout->addWidget(firstDayCombo, 1, 1);
    outerLayout->addWidget(selectionModeLabel, 2, 0);
    outerLayout->addWidget(selectionModeCombo, 2, 1);
    outerLayout->addLayout(checkBoxLayout, 3, 0, 1, 2);
    outerLayout->addWidget(horizontalHeaderLabel, 4, 0);
    outerLayout->addWidget(horizontalHeaderCombo, 4, 1);
    outerLayout->addWidget(verticalHeaderLabel, 5, 0);
    outerLayout->addWidget(verticalHeaderCombo, 5, 1);
    generalOptionsGroupBox->setLayout(outerLayout);

    // Initialisation des valeurs par défaut selon les options sélectionnées
    firstDayChanged(firstDayCombo->currentIndex());
    selectionModeChanged(selectionModeCombo->currentIndex());
    horizontalHeaderChanged(horizontalHeaderCombo->currentIndex());
    verticalHeaderChanged(verticalHeaderCombo->currentIndex());
}

//! [12]

//! [13]
void Window::createDatesGroupBox()
{
    datesGroupBox = new QGroupBox(tr("Dates"));

    minimumDateEdit = new QDateEdit;
    minimumDateEdit->setDisplayFormat("MMM d yyyy");
    minimumDateEdit->setDateRange(calendar->minimumDate(),
                                  calendar->maximumDate());
    minimumDateEdit->setDate(calendar->minimumDate());

    minimumDateLabel = new QLabel(tr("&Minimum Date:"));
    minimumDateLabel->setBuddy(minimumDateEdit);

    currentDateEdit = new QDateEdit;
    currentDateEdit->setDisplayFormat("MMM d yyyy");
    currentDateEdit->setDate(calendar->selectedDate());
    currentDateEdit->setDateRange(calendar->minimumDate(),
                                  calendar->maximumDate());

    currentDateLabel = new QLabel(tr("&Current Date:"));
    currentDateLabel->setBuddy(currentDateEdit);

    maximumDateEdit = new QDateEdit;
    maximumDateEdit->setDisplayFormat("MMM d yyyy");
    maximumDateEdit->setDateRange(calendar->minimumDate(),
                                  calendar->maximumDate());
    maximumDateEdit->setDate(calendar->maximumDate());

    maximumDateLabel = new QLabel(tr("Ma&ximum Date:"));
    maximumDateLabel->setBuddy(maximumDateEdit);

    //! [13] //! [14]
    connect(currentDateEdit, &QDateEdit::dateChanged,
            calendar, &QCalendarWidget::setSelectedDate);
    connect(calendar, &QCalendarWidget::selectionChanged,
            this, &Window::selectedDateChanged);
    connect(minimumDateEdit, &QDateEdit::dateChanged,
            this, &Window::minimumDateChanged);
    connect(maximumDateEdit, &QDateEdit::dateChanged,
            this, &Window::maximumDateChanged);

    //! [14]
    QGridLayout *dateBoxLayout = new QGridLayout;
    dateBoxLayout->addWidget(currentDateLabel, 1, 0);
    dateBoxLayout->addWidget(currentDateEdit, 1, 1);
    dateBoxLayout->addWidget(minimumDateLabel, 0, 0);
    dateBoxLayout->addWidget(minimumDateEdit, 0, 1);
    dateBoxLayout->addWidget(maximumDateLabel, 2, 0);
    dateBoxLayout->addWidget(maximumDateEdit, 2, 1);
    dateBoxLayout->setRowStretch(3, 1);

    datesGroupBox->setLayout(dateBoxLayout);
    //! [15]
}
//! [15]

//! [16]
void Window::createTextFormatsGroupBox()
{
    textFormatsGroupBox = new QGroupBox(tr("Text Formats"));

    ConsultationDatesColorCombo = createColorComboBox();
    ConsultationDatesColorCombo->setCurrentIndex(
        ConsultationDatesColorCombo->findText(tr("Blue")));

    ConsultationDatesColorLabel = new QLabel(tr("&ConsultationDates color:"));
    ConsultationDatesColorLabel->setBuddy(ConsultationDatesColorCombo);

    weekendColorCombo = createColorComboBox();
    weekendColorCombo->setCurrentIndex(
        weekendColorCombo->findText(tr("Red")));

    weekendColorLabel = new QLabel(tr("Week&end color:"));
    weekendColorLabel->setBuddy(weekendColorCombo);

    //! [16] //! [17]
    headerTextFormatCombo = new QComboBox;
    headerTextFormatCombo->addItem(tr("Bold"));
    headerTextFormatCombo->addItem(tr("Italic"));
    headerTextFormatCombo->addItem(tr("Plain"));

    headerTextFormatLabel = new QLabel(tr("&Header text:"));
    headerTextFormatLabel->setBuddy(headerTextFormatCombo);

    firstFridayCheckBox = new QCheckBox(tr("&First Friday in blue"));

    mayFirstCheckBox = new QCheckBox(tr("May &1 in red"));

    //! [17] //! [18]
    connect(ConsultationDatesColorCombo, &QComboBox::currentIndexChanged,
            this, &Window::ConsultationDatesFormatChanged);
    connect(ConsultationDatesColorCombo, &QComboBox::currentIndexChanged,
            this, &Window::reformatCalendarPage);
    connect(weekendColorCombo, &QComboBox::currentIndexChanged,
            this, &Window::weekendFormatChanged);
    connect(weekendColorCombo, &QComboBox::currentIndexChanged,
            this, &Window::reformatCalendarPage);
    connect(headerTextFormatCombo, &QComboBox::currentIndexChanged,
            this, &Window::reformatHeaders);
    connect(firstFridayCheckBox, &QCheckBox::toggled,
            this, &Window::reformatCalendarPage);
    connect(mayFirstCheckBox, &QCheckBox::toggled,
            this, &Window::reformatCalendarPage);

    //! [18]
    QHBoxLayout *checkBoxLayout = new QHBoxLayout;
    checkBoxLayout->addWidget(firstFridayCheckBox);
    checkBoxLayout->addStretch();
    checkBoxLayout->addWidget(mayFirstCheckBox);

    QGridLayout *outerLayout = new QGridLayout;
    outerLayout->addWidget(ConsultationDatesColorLabel, 0, 0);
    outerLayout->addWidget(ConsultationDatesColorCombo, 0, 1);
    outerLayout->addWidget(weekendColorLabel, 1, 0);
    outerLayout->addWidget(weekendColorCombo, 1, 1);
    outerLayout->addWidget(headerTextFormatLabel, 2, 0);
    outerLayout->addWidget(headerTextFormatCombo, 2, 1);
    outerLayout->addLayout(checkBoxLayout, 3, 0, 1, 2);
    textFormatsGroupBox->setLayout(outerLayout);

    ConsultationDatesFormatChanged();
    weekendFormatChanged();
    //! [19]
    reformatHeaders();
    reformatCalendarPage();
}
//! [19]

//! [20]
QComboBox *Window::createColorComboBox()
{
    QComboBox *comboBox = new QComboBox;
    comboBox->addItem(tr("Red"), QColor(Qt::red));
    comboBox->addItem(tr("Blue"), QColor(Qt::blue));
    comboBox->addItem(tr("Black"), QColor(Qt::black));
    comboBox->addItem(tr("Magenta"), QColor(Qt::magenta));
    return comboBox;
}
//! [20]
