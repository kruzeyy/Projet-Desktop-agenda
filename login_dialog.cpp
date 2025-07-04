#include "login_dialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Connexion");
    resize(400, 300);

    emailEdit = new QLineEdit;
    emailEdit->setPlaceholderText("Adresse e‑mail");

    passwordEdit = new QLineEdit;
    passwordEdit->setPlaceholderText("Mot de passe");
    passwordEdit->setEchoMode(QLineEdit::Password);

    confirmPasswordEdit = new QLineEdit;
    confirmPasswordEdit->setPlaceholderText("Confirmer le mot de passe");
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit->hide(); // ✅ Caché par défaut

    loginButton = new QPushButton("🔑 Se connecter");
    registerButton = new QPushButton("🆕 S'inscrire");
    createAccountButton = new QPushButton("✅ Créer un compte");
    createAccountButton->hide(); // ✅ Caché par défaut
    backToLoginButton = new QPushButton("← Retour");
    backToLoginButton->hide();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(emailEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(confirmPasswordEdit);
    layout->addWidget(loginButton);
    layout->addWidget(registerButton);
    layout->addWidget(createAccountButton);
    layout->addWidget(backToLoginButton);

    setStyleSheet(R"(
        QDialog {
            background-color: #2E3440;
            color: #ECEFF4;
            font-family: "Segoe UI", sans-serif;
            font-size: 14px;
            border-radius: 10px;
        }
        QLineEdit {
            background-color: #3B4252;
            border: 1px solid #4C566A;
            border-radius: 6px;
            padding: 8px;
            color: #ECEFF4;
        }
        QPushButton {
            background-color: #5E81AC;
            border-radius: 8px;
            color: white;
            padding: 10px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #81A1C1;
        }
    )");

    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginDialog::onRegisterClicked);
    connect(createAccountButton, &QPushButton::clicked, this, &LoginDialog::onCreateAccountClicked);
    connect(backToLoginButton, &QPushButton::clicked, this, &LoginDialog::showLoginForm);
}

QString LoginDialog::getEmail() const {
    return emailEdit->text();
}

QString LoginDialog::getPassword() const {
    return passwordEdit->text();
}

void LoginDialog::onLoginClicked() {
    if (emailEdit->text().isEmpty() || passwordEdit->text().isEmpty()) {
        showMessage("Veuillez entrer votre e‑mail et mot de passe.");
        return;
    }
    // TODO: Vérifier les identifiants
    accept(); // Connexion réussie
}

void LoginDialog::onRegisterClicked() {
    // ✅ Passe en mode inscription
    isRegisterMode = true;
    emailEdit->clear();
    passwordEdit->clear();
    confirmPasswordEdit->clear();

    loginButton->hide();
    registerButton->hide();
    createAccountButton->show();
    backToLoginButton->show();
    confirmPasswordEdit->show();
}

void LoginDialog::onCreateAccountClicked() {
    if (emailEdit->text().isEmpty() || passwordEdit->text().isEmpty() || confirmPasswordEdit->text().isEmpty()) {
        showMessage("Veuillez remplir tous les champs.");
        return;
    }
    if (passwordEdit->text() != confirmPasswordEdit->text()) {
        showMessage("Les mots de passe ne correspondent pas.");
        return;
    }

    // TODO: Enregistrer l'utilisateur dans un fichier JSON
    QMessageBox::information(this, "Compte créé", "Votre compte a été créé avec succès !");
    showLoginForm();
}

void LoginDialog::showLoginForm() {
    // ✅ Revient au mode connexion
    isRegisterMode = false;
    loginButton->show();
    registerButton->show();
    createAccountButton->hide();
    backToLoginButton->hide();
    confirmPasswordEdit->hide();
}

void LoginDialog::showMessage(const QString &message) {
    QMessageBox::warning(this, "Attention", message);
}
