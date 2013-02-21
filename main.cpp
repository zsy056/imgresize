#include <QtCore>
#include <QTranslator>

#include "main_window.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Imgresize");
    app.setOrganizationName("zsy");
    app.setOrganizationDomain("aztec.tk");

    auto locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString("imgresize_" + locale), ":/translations");
    qDebug() << locale;
    app.installTranslator(&translator);

    auto main_window = new MainWindow;
    main_window->show();
    return app.exec();
}
