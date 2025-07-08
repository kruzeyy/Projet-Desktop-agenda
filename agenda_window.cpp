#include "agenda_window.h"
#include "add_event_dialog.h"
#include <QVBoxLayout>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QTableWidget> // ✅ Ajout pour la vue horaire
#include <QHeaderView> // ✅ Ajout pour utiliser horizontalHeader()
#include <QInputDialog> // ✅ Ajout pour demander l'heure




AgendaWindow::AgendaWindow(QWidget *parent, const QString &userEmail)
    : QMainWindow(parent),
    calendar(new QCalendarWidget),
    eventList(new QListWidget),
    addButton(new QPushButton("➕", this)), // Parent direct de la fenêtre
    inviteServer(new QTcpServer(this)),      // ✅ Ajout serveur invitations
    inviteButton(new QPushButton("📨 Inviter", this)), // ✅ Ajout bouton inviter
    // ✅ Ajout du bouton Modifier
    editButton (new QPushButton("✏️ Modifier", this))
{
    // Mise en page
    QWidget *central = new QWidget;
    QHBoxLayout *mainLayout = new QHBoxLayout(central);

    // ✅ Colonne gauche : Calendrier + Liste
    leftLayout = new QVBoxLayout;
    leftLayout->addWidget(calendar);
    leftLayout->addWidget(eventList);

    // ✅ Ajout du bouton Supprimer
    deleteButton = new QPushButton("🗑 Supprimer");
    deleteButton->setStyleSheet(R"(
    QPushButton {
        background-color: #BF616A;
        color: white;
        border-radius: 8px;
        padding: 6px;
    }
    QPushButton:hover {
        background-color: #D08770;
    }
    )");
    leftLayout->addWidget(deleteButton);

    connect(deleteButton, &QPushButton::clicked, this, &AgendaWindow::deleteEvent);

    // ✅ Ajout du bouton Inviter
    inviteButton->setStyleSheet(R"(
    QPushButton {
        background-color: #88C0D0;
        color: white;
        border-radius: 8px;
        padding: 6px;
        font-weight: bold;
    }
    QPushButton:hover {
        background-color: #81A1C1;
    }
    )");
    leftLayout->addWidget(inviteButton);
    connect(inviteButton, &QPushButton::clicked, this, &AgendaWindow::sendInvitation);

    editButton->setStyleSheet(R"(
QPushButton {
    background-color: #A3BE8C;
    color: white;
    border-radius: 8px;
    padding: 6px;
    font-weight: bold;
}
QPushButton:hover {
    background-color: #88C0D0;
}
)");
    leftLayout->addWidget(editButton);

    // Connecte le bouton à la fonction de modification
    connect(editButton, &QPushButton::clicked, this, &AgendaWindow::editEvent);

    // ✅ Colonne droite : Vue horaire
    hourView = new QTableWidget;
    hourView->setRowCount(24);
    hourView->setColumnCount(1);
    hourView->setHorizontalHeaderLabels(QStringList() << "Événements");
    for (int i = 0; i < 24; ++i) {
        hourView->setVerticalHeaderItem(i, new QTableWidgetItem(QString("%1:00").arg(i, 2, 10, QChar('0'))));
        hourView->setItem(i, 0, new QTableWidgetItem(""));
    }
    hourView->horizontalHeader()->setStretchLastSection(true);
    hourView->verticalHeader()->setDefaultSectionSize(30);
    hourView->setEditTriggers(QAbstractItemView::NoEditTriggers); // 🔒 Empêche l’édition
    hourView->setFocusPolicy(Qt::NoFocus);                        // 🔒 Empêche la sélection


    // ✅ Ajoute les deux colonnes
    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addWidget(hourView, 1);

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
        QDate selectedDate = calendar->selectedDate();
        updateEventList(selectedDate);
        updateHourView(selectedDate);
    });

    // ✅ Serveur d'invitations pour recevoir
    connect(inviteServer, &QTcpServer::newConnection, this, &AgendaWindow::handleIncomingInvitation);
    inviteServer->listen(QHostAddress::Any, 5555);

    // Charger les événements
    loadEvents();
    updateEventList(calendar->selectedDate());
}


AgendaWindow::~AgendaWindow() {
    saveEvents();
    deleteButton = new QPushButton("🗑 Supprimer", this);
    inviteServer = new QTcpServer(this);
    connect(inviteServer, &QTcpServer::newConnection, this, &AgendaWindow::handleIncomingInvitation);
    inviteServer->listen(QHostAddress::Any, 5555); // Port 5555

    deleteButton->setFixedHeight(40);

    deleteButton->setStyleSheet(R"(
    QPushButton {
        background-color: #BF616A;
        color: white;
        border-radius: 8px;
        font-size: 14px;
        font-weight: bold;
        padding: 6px 12px;
    }
    QPushButton:hover {
        background-color: #D08770;
    }
)");
    leftLayout->addWidget(deleteButton); // Ajoute sous la liste des événements


}

void AgendaWindow::addEvent() {
    AddEventDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString title = dialog.getEventTitle().trimmed();

        // 🛑 Vérifie si le titre est vide
        if (title.isEmpty()) {
            QMessageBox::warning(this, "Titre manquant",
                                 "Veuillez entrer un titre pour l'événement.");
            return; // ❌ Annule l’ajout
        }

        QJsonObject event;
        event["date"] = calendar->selectedDate().toString(Qt::ISODate);
        event["title"] = title;
        event["allDay"] = dialog.isAllDay();

        if (!dialog.isAllDay()) {
            event["startTime"] = dialog.getEventTime().toString("HH:mm");
            event["endTime"] = dialog.getEndTime().toString("HH:mm");
        } else {
            event["startTime"] = "00:00"; // Valeur par défaut
            event["endTime"] = "23:59";
        }

        events.append(event);
        updateEventList(calendar->selectedDate());
        updateHourView(calendar->selectedDate());
        highlightEventDays();
    }
}

void AgendaWindow::handleIncomingInvitation() {
    QTcpSocket *clientSocket = inviteServer->nextPendingConnection();
    connect(clientSocket, &QTcpSocket::readyRead, [=]() {
        QByteArray data = clientSocket->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject event = doc.object();

        QString title = event["title"].toString();
        QString dateStr = event["date"].toString();
        QDate date = QDate::fromString(dateStr, Qt::ISODate);

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "📨 Invitation reçue",
                                      "Vous avez été invité à :\n\n📅 " + title +
                                          "\n📆 Le : " + date.toString("dd/MM/yyyy") +
                                          "\n\nSouhaitez-vous l’ajouter à votre calendrier ?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // ✅ Ajoute l’événement à la liste
            events.append(event);
            saveEvents();

            // ✅ Rafraîchir la vue si c’est le jour sélectionné
            if (calendar->selectedDate() == date) {
                updateEventList(date);
                updateHourView(date);
            }

            // ✅ Mettre en surbrillance le jour
            highlightEventDays();

            QMessageBox::information(this, "✅ Ajouté", "L’événement a été ajouté à votre calendrier.");
        } else {
            QMessageBox::information(this, "❌ Refusé", "Vous avez refusé l’invitation.");
        }

        clientSocket->close();
        clientSocket->deleteLater();
    });
}






void AgendaWindow::updateEventList(const QDate &date) {
    eventList->clear();
    for (const QJsonValue &value : events) {
        QJsonObject obj = value.toObject();
        if (obj["date"] == date.toString(Qt::ISODate))
            eventList->addItem(obj["title"].toString());
    }
}
void AgendaWindow::updateHourView(const QDate &date) {
    hourView->clearContents(); // ✅ Réinitialise la vue horaire

    int allDayRow = 0; // Ligne spéciale pour les événements toute la journée

    for (const QJsonValue &value : events) {
        QJsonObject obj = value.toObject();
        if (obj["date"].toString() == date.toString(Qt::ISODate)) {
            QString title = obj["title"].toString();

            if (obj["allDay"].toBool()) {
                // ✅ Afficher les événements toute la journée en haut
                hourView->setItem(allDayRow, 0, new QTableWidgetItem("[📌] " + title + " (Toute la journée)"));
                allDayRow++; // Si plusieurs événements, empile-les
            } else {
                QTime start = QTime::fromString(obj["startTime"].toString(), "HH:mm");
                QTime end = QTime::fromString(obj["endTime"].toString(), "HH:mm");

                if (start.isValid() && end.isValid()) {
                    for (int h = start.hour(); h <= end.hour(); ++h) {
                        QString timeSlotText;

                        if (h == start.hour()) {
                            // ✅ Sur la première ligne : afficher intervalle et titre
                            timeSlotText = QString("%1 - %2 %3")
                                               .arg(start.toString("HH:mm"))
                                               .arg(end.toString("HH:mm"))
                                               .arg(title);
                        } else {
                            // ✅ Sur les lignes suivantes : affichage simplifié
                            timeSlotText = "↳ " + title;
                        }

                        hourView->setItem(h, 0, new QTableWidgetItem(timeSlotText));
                    }
                }
            }
        }
    }
}

void AgendaWindow::deleteEvent() {
    QListWidgetItem *selectedItem = eventList->currentItem();
    if (!selectedItem) return;

    QString eventTitle = selectedItem->text();
    QDate selectedDate = calendar->selectedDate();

    // ✅ Confirmer la suppression
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Supprimer l'événement",
                                  "Voulez-vous vraiment supprimer l'événement : " + eventTitle + " ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Parcourir à l'envers pour éviter les erreurs d'index lors de la suppression
        for (int i = events.size() - 1; i >= 0; --i) {
            QJsonObject obj = events[i].toObject();
            if (obj["date"].toString() == selectedDate.toString(Qt::ISODate)
                && obj["title"].toString() == eventTitle) {
                events.removeAt(i); // ✅ Supprime du tableau
            }
        }

        saveEvents(); // ✅ Sauvegarde
        updateEventList(selectedDate);
        updateHourView(selectedDate);
        highlightEventDays();
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

void AgendaWindow::sendInvitation() {
    bool ok;
    QString ipAddress = QInputDialog::getText(this, "Inviter quelqu’un",
                                              "Entrez l’adresse IP du destinataire :", QLineEdit::Normal,
                                              "", &ok);
    if (!ok || ipAddress.isEmpty()) return;

    QListWidgetItem *selectedItem = eventList->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Aucun événement", "Sélectionnez un événement à inviter.");
        return;
    }

    // Trouve l’événement dans la liste
    QString title = selectedItem->text();
    QJsonObject eventToSend;
    for (const QJsonValue &value : events) {
        QJsonObject obj = value.toObject();
        if (obj["title"].toString() == title &&
            obj["date"].toString() == calendar->selectedDate().toString(Qt::ISODate)) {
            eventToSend = obj;
            break;
        }
    }

    if (eventToSend.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Impossible de trouver l’événement.");
        return;
    }

    // Envoie l’événement via TCP
    QTcpSocket socket;
    socket.connectToHost(ipAddress, 5555); // Port 5555 pour les invitations
    if (!socket.waitForConnected(3000)) {
        QMessageBox::warning(this, "Erreur", "Impossible de se connecter à " + ipAddress);
        return;
    }

    QJsonDocument doc(eventToSend);
    socket.write(doc.toJson());
    socket.flush();
    socket.waitForBytesWritten();
    socket.disconnectFromHost();

    QMessageBox::information(this, "Invitation envoyée", "L’événement a été envoyé à " + ipAddress);
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

void AgendaWindow::editEvent() {
    QListWidgetItem *selectedItem = eventList->currentItem();
    if (!selectedItem) return;

    QString eventTitle = selectedItem->text();
    QDate selectedDate = calendar->selectedDate();

    // 🔎 Trouver l'événement sélectionné
    for (int i = 0; i < events.size(); ++i) {
        QJsonObject obj = events[i].toObject();
        if (obj["date"].toString() == selectedDate.toString(Qt::ISODate) &&
            obj["title"].toString() == eventTitle) {

            // 📝 Ouvre le dialogue pré-rempli
            AddEventDialog dialog(this);
            dialog.setEventTitle(obj["title"].toString());
            dialog.setAllDay(obj["allDay"].toBool());

            if (!obj["allDay"].toBool()) {
                QTime start = QTime::fromString(obj["startTime"].toString(), "HH:mm");
                QTime end = QTime::fromString(obj["endTime"].toString(), "HH:mm");
                dialog.setEventTime(start);
                dialog.setEndTime(end);
            }

            if (dialog.exec() == QDialog::Accepted) {
                // 🔄 Mettre à jour l'événement
                obj["title"] = dialog.getEventTitle();
                obj["allDay"] = dialog.isAllDay();

                if (!dialog.isAllDay()) {
                    obj["startTime"] = dialog.getEventTime().toString("HH:mm");
                    obj["endTime"] = dialog.getEndTime().toString("HH:mm");
                } else {
                    obj["startTime"] = "00:00";
                    obj["endTime"] = "23:59";
                }

                events[i] = obj; // Remplacer l'ancien
                saveEvents();
                updateEventList(selectedDate);
                updateHourView(selectedDate);
                highlightEventDays();
            }
            break; // ✅ On a trouvé et modifié
        }
    }
}
