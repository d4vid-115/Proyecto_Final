#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Crear ventana principal
    MainWindow ventana;
    ventana.show();

    return app.exec();
}
