#include "mainwindow.h"
#include "gestorsprites.h"
#include "gestorsonidos.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    qDebug() << "\n╔══════════════════════════════════════════════╗";
    qDebug() << "║   EL NAUFRAGIO DEL RMS LUSITANIA            ║";
    qDebug() << "║   Universidad de Antioquia - 2025           ║";
    qDebug() << "╚══════════════════════════════════════════════╝\n";

    // ========== CARGAR SPRITES ==========
    GestorSprites* gestorSprites = GestorSprites::obtenerInstancia();
    gestorSprites->cargarTodosLosSprites();

    // ========== CARGAR SONIDOS ==========
    GestorSonidos* gestorSonidos = GestorSonidos::obtenerInstancia();
    gestorSonidos->cargarTodosLosSonidos();

    // Crear ventana principal
    MainWindow ventana;
    ventana.show();

    int resultado = app.exec();

    // Limpiar al salir
    GestorSprites::destruirInstancia();
    GestorSonidos::destruirInstancia();

    return resultado;
}
