#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("VehicleRentalSystem");
    app.setApplicationVersion("2.0");
    app.setOrganizationName("COS2614");

    MainWindow window;
    window.show();

    return app.exec();
}
