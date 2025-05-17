#ifndef MODIFIERRESSOURCEDIALOG_H
#define MODIFIERRESSOURCEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QRadioButton>
#include <QPushButton>

class ModifierRessourceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModifierRessourceDialog(QWidget *parent = nullptr);
    void setData(const QString &nom, const QString &type, int quantite, const QDate &date, const QString &status, double cout);
    QString getNom() const;
    QString getType() const;
    int getQuantite() const;
    QDate getDate() const;
    QString getStatus() const;
    double getCout() const;
    void showEvaluationStats();

private:
    QLineEdit *lineEditNom;
    QComboBox *comboBoxType;
    QLineEdit *lineEditQuantite;
    QDateEdit *dateEditAcquisition;
    QRadioButton *radioButtonEnPanne;
    QRadioButton *radioButtonEnmarche;
    QLineEdit *lineEditCout;
    QPushButton *btnConfirmer;
    QPushButton *btnDecliner;
};

#endif // MODIFIERRESSOURCEDIALOG_H
