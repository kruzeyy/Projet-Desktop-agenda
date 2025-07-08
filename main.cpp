#include "agenda_window.h"
#include <QApplication>
#include <QPalette>
#include <QColor>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 🎨 Appliquer un thème sombre moderne
    QPalette palette;
    palette.setColor(QPalette::Window, QColor("#2E3440"));           // Fond général
    palette.setColor(QPalette::WindowText, QColor("#ECEFF4"));       // Texte
    palette.setColor(QPalette::Base, QColor("#3B4252"));             // Fond des widgets
    palette.setColor(QPalette::AlternateBase, QColor("#434C5E"));    // Alternance
    palette.setColor(QPalette::ToolTipBase, QColor("#ECEFF4"));
    palette.setColor(QPalette::ToolTipText, QColor("#2E3440"));
    palette.setColor(QPalette::Text, QColor("#ECEFF4"));
    palette.setColor(QPalette::Button, QColor("#5E81AC"));           // Boutons
    palette.setColor(QPalette::ButtonText, QColor("#ECEFF4"));
    palette.setColor(QPalette::Highlight, QColor("#81A1C1"));        // Sélection
    palette.setColor(QPalette::HighlightedText, QColor("#2E3440"));  // Texte sélectionné
    palette.setColor(QPalette::BrightText, QColor("#BF616A"));       // Alertes

    app.setPalette(palette);

    // ✅ Lancer directement l'agenda
    AgendaWindow window;
    window.setMinimumSize(600, 400);  // Taille minimale agréable
    window.setWindowTitle("Agenda Partagé");
    window.show();

    return app.exec();
}
