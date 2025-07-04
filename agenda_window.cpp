#include "agenda_window.h"
#include "add_event_dialog.h"
#include <QVBoxLayout>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>


AgendaWindow::AgendaWindow(QWidget *parent, const QString &userEmail)
    : QMainWindow(parent),
    calendar(new QCalendarWidget),
    eventList(new QListWidget),
    addButton(new QPushButton("➕", this)) // Parent direct de la fenêtre
{
    // Mise en page
    QWidget *central = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(central);
    layout->addWidget(calendar);
    layout->addWidget(eventList);
    setCentralWidget(central);



    // 🎨 Couleur spéciale pour la date d’aujourd’hui
    QTextCharFormat todayFormat;
    todayFormat.setBackground(QBrush(QColor("#A3BE8C"))); // Fond vert clair
    todayFormat.setForeground(QBrush(Qt::white));         // Texte blanc
    todayFormat.setFontWeight(QFont::Bold);
    calendar->setDateTextFormat(QDate::currentDate(), todayFormat);

    // Appliquer la mise en surbrillance des jours avec tâches
    highlightEventDays();

    // Bouton flottant en bas à droite
    addButton->setFixedSize(60, 60);
    addButton->setStyleSheet(R"(
        QPushButton {
            background-color: #5E81AC;
            color: white;
            border-radius: 30px;
            font-size: 28px;
            font-weight: bold;
            box-shadow: 0px 4px 10px rgba(0,0,0,0.3);
        }
        QPushButton:hover {
            background-color: #81A1C1;
        }
    )");
    addButton->raise(); // Passe au-dessus des autres widgets
    repositionAddButton();

    // Style général (flat design, sombre)
    setStyleSheet(R"(
        QWidget {
            font-family: "Segoe UI", "Arial", sans-serif;
            font-size: 14px;
            color: #ECEFF4;
            background-color: #2E3440;
        }

        QCalendarWidget QWidget {
            background-color: #3B4252;
            color: #ECEFF4;
            border-radius: 10px;
        }

        QCalendarWidget QToolButton {
            background-color: #5E81AC;
            color: #ECEFF4;
            border-radius: 6px;
            padding: 4px;
            margin: 2px;
        }

        QCalendarWidget QToolButton:hover {
            background-color: #81A1C1;
        }

        QCalendarWidget QAbstractItemView {
            selection-background-color: #88C0D0;
            selection-color: #2E3440;
            background-color: #3B4252;
            border: none;
        }

        QListWidget {
            background-color: #3B4252;
            border: none;
            border-radius: 8px;
            padding: 8px;
        }

        QListWidget::item {
            background-color: #434C5E;
            margin: 6px;
            padding: 10px;
            border-radius: 8px;
            box-shadow: 0px 2px 6px rgba(0,0,0,0.2);
        }

        QListWidget::item:selected {
            background-color: #81A1C1;
            color: #2E3440;
        }
    )");

    // Connecter signaux et slots
    connect(addButton, &QPushButton::clicked, this, &AgendaWindow::addEvent);
    connect(calendar, &QCalendarWidget::selectionChanged, [=]() {
        updateEventList(calendar->selectedDate());
    });

    // Charger les événements
    loadEvents();
    updateEventList(calendar->selectedDate());
}

AgendaWindow::~AgendaWindow() {
    saveEvents();
}

void AgendaWindow::addEvent() {
    AddEventDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QJsonObject event;
        event["date"] = calendar->selectedDate().toString(Qt::ISODate);
        event["title"] = dialog.getEventTitle();
        events.append(event);
        updateEventList(calendar->selectedDate());

        // 🎨 Mettre à jour les jours avec événements
        highlightEventDays();
    }
}

void AgendaWindow::updateEventList(const QDate &date) {
    eventList->clear();
    for (const QJsonValue &value : events) {
        QJsonObject obj = value.toObject();
        if (obj["date"] == date.toString(Qt::ISODate))
            eventList->addItem(obj["title"].toString());
    }
}

void AgendaWindow::highlightEventDays() {
    QTextCharFormat eventDayFormat;
    eventDayFormat.setBackground(QBrush(QColor("#EBCB8B"))); // Fond jaune doux
    eventDayFormat.setForeground(QBrush(Qt::black));         // Texte noir
    eventDayFormat.setFontItalic(true);                      // Italique pour les jours avec tâches

    // Réinitialiser d'abord tous les formats
    calendar->setDateTextFormat(QDate(), QTextCharFormat());

    // Réappliquer le format pour aujourd'hui
    QTextCharFormat todayFormat;
    todayFormat.setBackground(QBrush(QColor("#A3BE8C"))); // Fond vert clair
    todayFormat.setForeground(QBrush(Qt::white));
    todayFormat.setFontWeight(QFont::Bold);
    calendar->setDateTextFormat(QDate::currentDate(), todayFormat);

    // Appliquer le format aux jours avec événements
    for (const QJsonValue &value : events) {
        QJsonObject obj = value.toObject();
        QDate date = QDate::fromString(obj["date"].toString(), Qt::ISODate);
        if (date.isValid() && date != QDate::currentDate()) {
            calendar->setDateTextFormat(date, eventDayFormat);
        }
    }
}

void AgendaWindow::loadEvents() {
    QFile file("events.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        events = doc.array();

        // 🎨 Mettre à jour les jours avec tâches après chargement
        highlightEventDays();
    }
}

void AgendaWindow::saveEvents() {
    QFile file("events.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(events);
        file.write(doc.toJson());
    }
}

void AgendaWindow::onDateSelected(const QDate &date) {
    updateEventList(date);
}

// ✅ Repositionner le bouton flottant lors d'un resize
void AgendaWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    repositionAddButton();
}

void AgendaWindow::repositionAddButton() {
    addButton->move(width() - addButton->width() - 20, height() - addButton->height() - 20);
}

void AgendaWindow::onProfileClicked() {
    // ✅ Action par défaut : ouvrir un petit menu ou afficher un message
    QMessageBox::information(this, "Profil", "Ici vous pourrez gérer votre profil.");
}

void AgendaWindow::onLogoutClicked() {
    // ✅ Déconnexion : ferme la fenêtre
    this->close();
}
