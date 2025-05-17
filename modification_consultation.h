#ifndef MODIFICATION_CONSULTATION_H
#define MODIFICATION_CONSULTATION_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QPushButton>
#include "consultations.h"

class ModifierConsultationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModifierConsultationDialog(QWidget *parent = nullptr);

    // CONSULTATIONS
    void setData(const CONSULTATIONS &consultation);


    CONSULTATIONS getModifiedConsultation() const;

private:
    QLineEdit *lineEditSujet;

    QDateTimeEdit *dateEditConsultation;
    QLineEdit *lineEditHeureConsultation;
    QComboBox *comboBoxStatus;

    QPushButton *btnConfirmer;
    QPushButton *btnDecliner;

    CONSULTATIONS consultation;
};

#endif // MODIFICATION_CONSULTATION_H
