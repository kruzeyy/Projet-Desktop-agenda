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

private:
    QLineEdit *titleEdit;
    QPushButton *okButton;
    QTimeEdit *timeEdit;
    QCheckBox *allDayCheckBox;
};

#endif // ADD_EVENT_DIALOG_H
