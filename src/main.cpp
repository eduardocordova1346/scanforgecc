#include <QApplication>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("ScanForge CC");
    app.setApplicationVersion("0.1.0");
    app.setOrganizationName("ScanForge");

    MainWindow window;
    window.show();

    return app.exec();
}
