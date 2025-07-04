#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCalendarWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDateChanged(const QDate &date);
    void onAddEvent();

private:
    void loadEvents();
    void saveEvents();
    void refreshEventList();

    Ui::MainWindow *ui;
    QCalendarWidget *calendar;
    QTableWidget *eventTable;
    QLineEdit *eventEdit;
    QPushButton *addButton;

    QMap<QDate, QStringList> events;
};
#endif // MAINWINDOW_H
