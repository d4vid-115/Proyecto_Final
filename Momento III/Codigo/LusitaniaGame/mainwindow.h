#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include "gamewidget.h"
#include "pantallainicio.h"
#include "pantallavictoria.h"
#include "pantalladerrota.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    // ===== WIDGETS =====
    GameWidget* gameWidget;
    PantallaInicio* pantallaInicio;
    PantallaVictoria* pantallaVictoria;
    PantallaDerrota* pantallaDerrota;

    bool enJuego;

    // ===== MENUS =====
    QMenu* menuJuego;
    QMenu* menuNiveles;
    QMenu* menuAyuda;

    // ===== ACCIONES =====
    QAction* accionNuevoJuego;
    QAction* accionPausar;
    QAction* accionReiniciar;
    QAction* accionSalir;

    QAction* accionNivel1;
    QAction* accionNivel2;
    QAction* accionNivel3;

    QAction* accionInstrucciones;
    QAction* accionAcercaDe;

    // Metodos de creacion de interfaz
    void crearAcciones();
    void crearMenus();
    void conectarAcciones();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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

    // Slots de pantallas
    void onIniciarJuegoDesdeMenu(int nivel);
    void onVictoria(int puntos, int nivel);
    void onDerrota(int puntos, int nivel);
    void volverAlMenu();
};

#endif // MAINWINDOW_H
