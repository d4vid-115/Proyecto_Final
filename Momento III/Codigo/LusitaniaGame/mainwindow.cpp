#include "mainwindow.h"
#include <QMessageBox>
#include <QApplication>

// ========== CONSTRUCTOR ==========

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
    gameWidget(nullptr) {

    // Configurar ventana
    setWindowTitle("El Naufragio del RMS Lusitania");
    setMinimumSize(800, 650); // Extra 50px para menu

    // Crear widget del juego
    gameWidget = new GameWidget(this);
    setCentralWidget(gameWidget);

    // Crear interfaz
    crearAcciones();
    crearMenus();
    conectarAcciones();

    // Centrar ventana
    resize(800, 650);
}

MainWindow::~MainWindow() {
}

// ========== CREACION DE INTERFAZ ==========

void MainWindow::crearAcciones() {
    // ===== MENU JUEGO =====
    accionNuevoJuego = new QAction("&Nuevo Juego", this);
    accionNuevoJuego->setShortcut(QKeySequence("Ctrl+N"));

    accionPausar = new QAction("&Pausar", this);
    accionPausar->setShortcut(QKeySequence("P"));

    accionReiniciar = new QAction("&Reiniciar", this);
    accionReiniciar->setShortcut(QKeySequence("Ctrl+R"));

    accionSalir = new QAction("&Salir", this);
    accionSalir->setShortcut(QKeySequence("Ctrl+Q"));

    // ===== MENU NIVELES =====
    accionNivel1 = new QAction("Nivel 1: Oceano", this);
    accionNivel1->setShortcut(QKeySequence("1"));

    accionNivel2 = new QAction("Nivel 2: Barco", this);
    accionNivel2->setShortcut(QKeySequence("2"));

    accionNivel3 = new QAction("Nivel 3: Submarino", this);
    accionNivel3->setShortcut(QKeySequence("3"));

    // ===== MENU AYUDA =====
    accionInstrucciones = new QAction("&Instrucciones", this);
    accionInstrucciones->setShortcut(QKeySequence("F1"));

    accionAcercaDe = new QAction("&Acerca de", this);
}

void MainWindow::crearMenus() {
    // ===== MENU JUEGO =====
    menuJuego = menuBar()->addMenu("&Juego");
    menuJuego->addAction(accionNuevoJuego);
    menuJuego->addAction(accionPausar);
    menuJuego->addAction(accionReiniciar);
    menuJuego->addSeparator();
    menuJuego->addAction(accionSalir);

    // ===== MENU NIVELES =====
    menuNiveles = menuBar()->addMenu("&Niveles");
    menuNiveles->addAction(accionNivel1);
    menuNiveles->addAction(accionNivel2);
    menuNiveles->addAction(accionNivel3);

    // ===== MENU AYUDA =====
    menuAyuda = menuBar()->addMenu("&Ayuda");
    menuAyuda->addAction(accionInstrucciones);
    menuAyuda->addSeparator();
    menuAyuda->addAction(accionAcercaDe);
}

void MainWindow::conectarAcciones() {
    // Conectar acciones a slots
    connect(accionNuevoJuego, &QAction::triggered, this, &MainWindow::onNuevoJuego);
    connect(accionPausar, &QAction::triggered, this, &MainWindow::onPausar);
    connect(accionReiniciar, &QAction::triggered, this, &MainWindow::onReiniciar);
    connect(accionSalir, &QAction::triggered, this, &MainWindow::onSalir);

    connect(accionNivel1, &QAction::triggered, this, &MainWindow::onCargarNivel1);
    connect(accionNivel2, &QAction::triggered, this, &MainWindow::onCargarNivel2);
    connect(accionNivel3, &QAction::triggered, this, &MainWindow::onCargarNivel3);

    connect(accionInstrucciones, &QAction::triggered, this, &MainWindow::onInstrucciones);
    connect(accionAcercaDe, &QAction::triggered, this, &MainWindow::onAcercaDe);
}

// ========== SLOTS DE MENU ==========

void MainWindow::onNuevoJuego() {
    if (gameWidget) {
        gameWidget->iniciarJuego(1); // Comenzar desde nivel 1
    }
}

void MainWindow::onPausar() {
    if (gameWidget) {
        MotorJuego* motor = MotorJuego::obtenerInstancia();

        if (motor->getEstado() == EstadoJuego::JUGANDO) {
            gameWidget->pausarJuego();
        } else if (motor->getEstado() == EstadoJuego::PAUSADO) {
            gameWidget->reanudarJuego();
        }
    }
}

void MainWindow::onReiniciar() {
    if (gameWidget) {
        gameWidget->reiniciarJuego();
    }
}

void MainWindow::onSalir() {
    QApplication::quit();
}

void MainWindow::onCargarNivel1() {
    if (gameWidget) {
        gameWidget->iniciarJuego(1);
    }
}

void MainWindow::onCargarNivel2() {
    if (gameWidget) {
        gameWidget->iniciarJuego(2);
    }
}

void MainWindow::onCargarNivel3() {
    if (gameWidget) {
        gameWidget->iniciarJuego(3);
    }
}

void MainWindow::onInstrucciones() {
    QMessageBox::information(this, "Instrucciones",
                             "<h2>El Naufragio del RMS Lusitania</h2>"
                             "<p><b>Controles:</b></p>"
                             "<ul>"
                             "<li><b>WASD / Flechas:</b> Mover jugador</li>"
                             "<li><b>Shift:</b> Habilidad especial (aumenta velocidad 3s)</li>"
                             "<li><b>P:</b> Pausar/Reanudar</li>"
                             "</ul>"
                             "<p><b>Niveles:</b></p>"
                             "<ul>"
                             "<li><b>Nivel 1 - Oceano:</b> Sobrevive 3 minutos evitando submarinos</li>"
                             "<li><b>Nivel 2 - Barco:</b> Rescata 10 NPCs antes del hundimiento</li>"
                             "<li><b>Nivel 3 - Submarino:</b> Llega a la superficie evitando vortices</li>"
                             "</ul>");
}

void MainWindow::onAcercaDe() {
    QMessageBox::about(this, "Acerca de",
                       "<h2>El Naufragio del RMS Lusitania</h2>"
                       "<p><b>Version:</b> 1.0</p>"
                       "<p><b>Autor:</b> David Fernando Revelo Morales</p>"
                       "<p><b>Universidad:</b> Universidad de Antioquia</p>"
                       "<p><b>Curso:</b> Informatica II - 2025-2</p>"
                       "<br>"
                       "<p>Videojuego educativo que simula el hundimiento del RMS Lusitania "
                       "utilizando fisica realista y sistemas de IA.</p>"
                       "<p><b>Fisicas implementadas:</b></p>"
                       "<ul>"
                       "<li>Movimiento Armonico Simple (MAS)</li>"
                       "<li>Trayectoria Parabolica</li>"
                       "<li>Plano Inclinado</li>"
                       "<li>Movimiento Circular Uniforme (MCU)</li>"
                       "<li>Flotacion (Arquimedes)</li>"
                       "<li>Vortice</li>"
                       "</ul>");
}
