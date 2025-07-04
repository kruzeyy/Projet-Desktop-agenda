#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    QString getEmail() const;
    QString getPassword() const;

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onCreateAccountClicked();
    void showLoginForm(); // ✅ Permet de revenir au formulaire de connexion

private:
    QLineEdit *emailEdit;
    QLineEdit *passwordEdit;
    QLineEdit *confirmPasswordEdit; // ✅ Champ confirmation mot de passe
    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *createAccountButton;
    QPushButton *backToLoginButton;

    bool isRegisterMode = false; // ✅ Mode inscription

    void setupLoginForm();
    void setupRegisterForm();
    void showMessage(const QString &message);
};

#endif // LOGINDIALOG_H
