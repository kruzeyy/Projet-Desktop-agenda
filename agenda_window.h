#ifndef AGENDAWINDOW_H
#define AGENDAWINDOW_H

#include <QMainWindow>
#include <QCalendarWidget>
#include <QListWidget>
#include <QPushButton>
#include <QToolButton>    // ✅ Ajouté pour le bouton profil
#include <QJsonArray>

class AgendaWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit AgendaWindow(QWidget *parent = nullptr, const QString &userEmail = "");
    ~AgendaWindow();

private slots:
    void addEvent();
    void loadEvents();
    void saveEvents();
    void onDateSelected(const QDate &date);

    // ✅ Slots pour le menu profil
    void onProfileClicked();
    void onLogoutClicked();


private:
    QCalendarWidget *calendar;
    QListWidget *eventList;
    QPushButton *addButton;
    QToolButton *profileButton;  // ✅ Nouveau : bouton profil
    QJsonArray events;

    QString email; // ✅ E-mail de l’utilisateur connecté

    void updateEventList(const QDate &date);
    void repositionAddButton();
    void highlightEventDays();

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // AGENDAWINDOW_H
