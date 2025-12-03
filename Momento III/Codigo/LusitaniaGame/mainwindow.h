#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include "gamewidget.h"

// MainWindow - Ventana principal de la aplicacion
// Contiene el GameWidget y menus
class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    // Widget del juego
    GameWidget* gameWidget;

    // Menus y acciones
    QMenu* menuJuego;
    QMenu* menuNiveles;
    QMenu* menuAyuda;

    QAction* accionNuevoJuego;
    QAction* accionPausar;
    QAction* accionReiniciar;
    QAction* accionSalir;

    QAction* accionNivel1;
    QAction* accionNivel2;
    QAction* accionNivel3;

    QAction* accionInstrucciones;
    QAction* accionAcercaDe;

    // Metodos privados
    void crearMenus();
    void crearAcciones();
    void conectarAcciones();

private slots:
    // Slots de menu
    void onNuevoJuego();
    void onPausar();
    void onReiniciar();
    void onSalir();

    void onCargarNivel1();
    void onCargarNivel2();
    void onCargarNivel3();

    void onInstrucciones();
    void onAcercaDe();

public:
    // ========== CONSTRUCTOR ==========
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};

#endif // MAINWINDOW_H
