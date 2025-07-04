#include "mainwindow.h"
#include <QVBoxLayout>
#include <QFile>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(nullptr)
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    calendar = new QCalendarWidget(this);
    eventTable = new QTableWidget(this);
    eventTable->setColumnCount(1);
    eventTable->setHorizontalHeaderLabels(QStringList() << "Ev\xC3\xA9nements");

    eventEdit = new QLineEdit(this);
    addButton = new QPushButton(tr("Ajouter"), this);
    connect(addButton, &QPushButton::clicked, this, &MainWindow::onAddEvent);

    connect(calendar, &QCalendarWidget::selectionChanged, [this]() {
        onDateChanged(calendar->selectedDate());
    });

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(calendar);
    layout->addWidget(eventTable);

    QHBoxLayout *hl = new QHBoxLayout;
    hl->addWidget(eventEdit);
    hl->addWidget(addButton);
    layout->addLayout(hl);

    central->setLayout(layout);

    loadEvents();
    onDateChanged(calendar->selectedDate());
}

MainWindow::~MainWindow()
{
    saveEvents();
}

void MainWindow::onDateChanged(const QDate &date)
{
    Q_UNUSED(date);
    refreshEventList();
}

void MainWindow::onAddEvent()
{
    QString text = eventEdit->text().trimmed();
    if (!text.isEmpty()) {
        events[calendar->selectedDate()].append(text);
        eventEdit->clear();
        refreshEventList();
    }
}

void MainWindow::refreshEventList()
{
    eventTable->clearContents();
    QStringList list = events.value(calendar->selectedDate());
    eventTable->setRowCount(list.size());
    for (int i = 0; i < list.size(); ++i) {
        QTableWidgetItem *item = new QTableWidgetItem(list.at(i));
        eventTable->setItem(i, 0, item);
    }
}

void MainWindow::loadEvents()
{
    QFile file(QDir::homePath() + "/agenda_events.json");
    if (!file.open(QIODevice::ReadOnly))
        return;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject obj = doc.object();
    for (const QString &key : obj.keys()) {
        QDate date = QDate::fromString(key, Qt::ISODate);
        QJsonArray arr = obj.value(key).toArray();
        QStringList list;
        for (const QJsonValue &val : arr)
            list.append(val.toString());
        events[date] = list;
    }
}

void MainWindow::saveEvents()
{
    QFile file(QDir::homePath() + "/agenda_events.json");
    if (!file.open(QIODevice::WriteOnly))
        return;
    QJsonObject obj;
    for (auto it = events.begin(); it != events.end(); ++it) {
        QJsonArray arr;
        for (const QString &ev : it.value())
            arr.append(ev);
        obj.insert(it.key().toString(Qt::ISODate), arr);
    }
    QJsonDocument doc(obj);
    file.write(doc.toJson());
}
