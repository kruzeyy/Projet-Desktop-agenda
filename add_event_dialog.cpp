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
    layout->addWidget(okButton);

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
}

QString AddEventDialog::getEventTitle() const {
    return titleEdit->text();
}
