#ifndef AGENDAWINDOW_H
#define AGENDAWINDOW_H

#include <QMainWindow>
#include <QCalendarWidget>
#include <QListWidget>
#include <QPushButton>
#include <QToolButton>    // ✅ Ajouté pour le bouton profil
#include <QJsonArray>
#include <QTableWidget> // ✅ Ajout pour la vue horaire
#include <QVBoxLayout>



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
    void deleteEvent();


private:
    QCalendarWidget *calendar;
    QListWidget *eventList;
    QPushButton *addButton;
    QTableWidget *hourView; // ✅ Ajout pour la vue horaire
    QToolButton *profileButton;  // ✅ Nouveau : bouton profil
    QJsonArray events;
    QPushButton *deleteButton;
    void updateHourView(const QDate &date); // ✅ Ajout pour vue horaire
    QVBoxLayout *leftLayout; // ✅ Permet d'y accéder depuis toute la classe



    QString email; // ✅ E-mail de l’utilisateur connecté

    void updateEventList(const QDate &date);
    void repositionAddButton();
    void highlightEventDays();

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // AGENDAWINDOW_H
