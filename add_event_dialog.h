#ifndef ADD_EVENT_DIALOG_H
#define ADD_EVENT_DIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QTimeEdit> // ✅ Ajoute cette ligne en haut
#include <QCheckBox>


class AddEventDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddEventDialog(QWidget *parent = nullptr);
    QString getEventTitle() const;
    QTime getEventTime() const;  // ✅ Récupère l’heure
    bool isAllDay() const;       // ✅ Déplacé ici dans la classe
    QTime getStartTime() const;
    QTime getEndTime() const;


private:
    QLineEdit *titleEdit;
    QPushButton *okButton;
    QTimeEdit *timeEdit;
    QCheckBox *allDayCheckBox;
    QTimeEdit *endTimeEdit; // ✅ champ pour l’heure de fin
    QTimeEdit *startTimeEdit; // ✅ champ pour l’heure de début

};

#endif // ADD_EVENT_DIALOG_H
