#ifndef ADD_EVENT_DIALOG_H
#define ADD_EVENT_DIALOG_H

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

class AddEventDialog : public QDialog {
    Q_OBJECT  // ✅ obligatoire

public:
    explicit AddEventDialog(QWidget *parent = nullptr);
    QString getEventTitle() const;

private:
    QLineEdit *titleEdit;
    QPushButton *okButton;
};

#endif // ADD_EVENT_DIALOG_H
