#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
//#inlucde <functional>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Assignment6SimonGame_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }


    Model model;
    MainWindow w(model);
    w.show();
    return a.exec(); // Starts program
}
