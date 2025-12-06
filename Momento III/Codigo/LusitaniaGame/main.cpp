#include "mainwindow.h"
#include "gestorsprites.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // ========== CARGAR TODOS LOS SPRITES ==========
    GestorSprites* gestor = GestorSprites::obtenerInstancia();
    gestor->cargarTodosLosSprites();

    // Crear ventana principal
    MainWindow ventana;
    ventana.show();

    int resultado = app.exec();

    // Limpiar gestor al salir
    GestorSprites::destruirInstancia();

    return resultado;
}
