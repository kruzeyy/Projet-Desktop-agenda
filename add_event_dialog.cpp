#include "add_event_dialog.h"
#include <QVBoxLayout>
#include <QLabel>

AddEventDialog::AddEventDialog(QWidget *parent)
    : QDialog(parent), titleEdit(new QLineEdit), okButton(new QPushButton("✔ OK")) {

    // Mise en page
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *label = new QLabel("Titre de l'événement :");
    layout->addWidget(label);
    layout->addWidget(titleEdit);
    timeEdit = new QTimeEdit;
    endTimeEdit = new QTimeEdit(this); // ✅ Champ heure de fin
    endTimeEdit->setTime(QTime::currentTime().addSecs(3600)); // 1h après par défaut
    timeEdit->setDisplayFormat("HH:mm"); // ✅ Format 24h
    timeEdit->setTime(QTime::currentTime()); // ✅ Heure actuelle par défaut
    layout->addWidget(new QLabel("Heure de l'événement :"));
    layout->addWidget(timeEdit);

    layout->addWidget(new QLabel("Heure de fin :"));
    layout->addWidget(endTimeEdit);
    layout->addWidget(okButton);

    allDayCheckBox = new QCheckBox("Toute la journée");
    layout->addWidget(allDayCheckBox);


    // Style général de la pop-up
    setStyleSheet(R"(
        QDialog {
            background-color: #3B4252;
            border-radius: 10px;
        }

        QLabel {
            color: #ECEFF4;
            font-size: 14px;
        }

        QLineEdit {
            background-color: #434C5E;
            color: #ECEFF4;
            border: 1px solid #4C566A;
            border-radius: 5px;
            padding: 6px;
        }

        QLineEdit:focus {
            border: 1px solid #81A1C1;
        }

        QPushButton {
            background-color: #5E81AC;
            color: #ECEFF4;
            border-radius: 8px;
            font-weight: bold;
            padding: 8px 16px;
        }

        QPushButton:hover {
            background-color: #81A1C1;
        }
    )");

    // Bouton OK ferme la pop-up
    connect(okButton, &QPushButton::clicked, this, &AddEventDialog::accept);
    connect(allDayCheckBox, &QCheckBox::toggled, [=](bool checked) {
        timeEdit->setEnabled(!checked);
    });

}

QString AddEventDialog::getEventTitle() const {
    return titleEdit->text();
}

QTime AddEventDialog::getEventTime() const {
    return timeEdit->time();
}

bool AddEventDialog::isAllDay() const {
    return allDayCheckBox->isChecked();
}

QTime AddEventDialog::getStartTime() const {
    return startTimeEdit->time();
}

QTime AddEventDialog::getEndTime() const {
    return endTimeEdit->time();
}

void AddEventDialog::setEventTitle(const QString &title) {
    titleEdit->setText(title);
}

void AddEventDialog::setEventTime(const QTime &time) {
    timeEdit->setTime(time);
}

void AddEventDialog::setEndTime(const QTime &time) {
    endTimeEdit->setTime(time);
}

void AddEventDialog::setAllDay(bool allDay) {
    allDayCheckBox->setChecked(allDay);
}
