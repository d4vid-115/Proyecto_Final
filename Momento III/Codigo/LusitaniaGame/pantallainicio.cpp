#include "pantallainicio.h"
#include "gestorsprites.h"
#include <QLinearGradient>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialog>

PantallaInicio::PantallaInicio(QWidget *parent)
    : QWidget(parent), menuPrincipal(true) {

    setFixedSize(800, 600);
    setMouseTracking(true);

    // Cargar fondo
    GestorSprites* gestor = GestorSprites::obtenerInstancia();
    fondoMenu = gestor->getSprite("menu_fondo");

    if (fondoMenu.isNull()) {
        qDebug() << "[PantallaInicio] Fondo de menu no cargado, usando color solido";
    } else {
        fondoMenu = fondoMenu.scaled(800, 600, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        qDebug() << "[PantallaInicio] Fondo de menu cargado correctamente";
    }

    // Crear botones MENU PRINCIPAL (centrados)
    int centroX = 400;
    int anchoBoton = 300;
    int altoBoton = 60;

    botonJugar = Boton(centroX - anchoBoton/2, 300, anchoBoton, altoBoton, "NUEVO JUEGO");
    botonOpciones = Boton(centroX - anchoBoton/2, 380, anchoBoton, altoBoton, "¿COMO JUGAR?");
    botonSalir = Boton(centroX - anchoBoton/2, 460, anchoBoton, altoBoton, "SALIR");

    // Crear botones SELECCION DE NIVEL
    botonNivel1 = Boton(centroX - anchoBoton/2, 260, anchoBoton, altoBoton, "NIVEL 1: OCEANO");
    botonNivel2 = Boton(centroX - anchoBoton/2, 340, anchoBoton, altoBoton, "NIVEL 2: BARCO");
    botonNivel3 = Boton(centroX - anchoBoton/2, 420, anchoBoton, altoBoton, "NIVEL 3: SUBMARINO");
}

PantallaInicio::~PantallaInicio() {
}

void PantallaInicio::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // ===== FONDO =====
    if (!fondoMenu.isNull()) {
        painter.drawPixmap(0, 0, fondoMenu);
        painter.fillRect(rect(), QColor(0, 0, 0, 120));
    } else {
        QLinearGradient gradient(0, 0, 0, 600);
        gradient.setColorAt(0, QColor(10, 20, 40));
        gradient.setColorAt(1, QColor(0, 50, 100));
        painter.fillRect(rect(), gradient);
    }

    // ===== TITULO =====
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 180));
    painter.drawRoundedRect(75, 40, 650, 160, 15, 15);

    // Sombra del titulo
    painter.setFont(QFont("Arial", 38, QFont::Bold));
    painter.setPen(QColor(0, 0, 0, 220));
    painter.drawText(80, 85, 650, 100, Qt::AlignCenter,
                     "EL NAUFRAGIO DEL\nRMS LUSITANIA");

    // Titulo principal
    painter.setPen(QColor(255, 215, 0));
    painter.drawText(75, 80, 650, 100, Qt::AlignCenter,
                     "EL NAUFRAGIO DEL\nRMS LUSITANIA");

    // Subtitulo
    painter.setFont(QFont("Arial", 13, QFont::Bold));
    painter.setPen(QColor(255, 255, 255));
    painter.drawText(75, 175, 650, 25, Qt::AlignCenter,
                     "Un Viaje por la Historia y la Física");

    // ===== BOTONES =====
    if (menuPrincipal) {
        dibujarBoton(painter, botonJugar);
        dibujarBoton(painter, botonOpciones);
        dibujarBoton(painter, botonSalir);
    } else {
        // Menu de seleccion de nivel
        painter.setFont(QFont("Arial", 20, QFont::Bold));
        painter.setPen(Qt::white);
        painter.drawText(150, 215, 500, 30, Qt::AlignCenter, "SELECCIONA UN NIVEL");

        dibujarBoton(painter, botonNivel1);
        dibujarBoton(painter, botonNivel2);
        dibujarBoton(painter, botonNivel3);

        // Boton volver
        Boton botonVolver = Boton(250, 510, 300, 50, "VOLVER");
        botonVolver.hover = botonSalir.hover;
        dibujarBoton(painter, botonVolver);
    }

    // ===== FOOTER =====
    painter.setFont(QFont("Arial", 10));
    painter.setPen(QColor(200, 200, 200));
    painter.drawText(10, 575, 780, 20, Qt::AlignCenter,
                     "Universidad de Antioquia - Informatica II - 2025-2");
}

void PantallaInicio::dibujarBoton(QPainter& painter, const Boton& boton) {
    painter.setPen(Qt::NoPen);

    if (boton.hover) {
        QLinearGradient gradient(boton.rect.topLeft(), boton.rect.bottomLeft());
        gradient.setColorAt(0, QColor(255, 215, 0, 240));
        gradient.setColorAt(1, QColor(255, 165, 0, 240));
        painter.setBrush(gradient);
        painter.setPen(QPen(QColor(255, 255, 150), 3));
    } else {
        QLinearGradient gradient(boton.rect.topLeft(), boton.rect.bottomLeft());
        gradient.setColorAt(0, QColor(40, 70, 120, 220));
        gradient.setColorAt(1, QColor(20, 40, 80, 220));
        painter.setBrush(gradient);
        painter.setPen(QPen(QColor(120, 160, 210), 2));
    }

    painter.drawRoundedRect(boton.rect, 12, 12);

    painter.setPen(boton.hover ? QColor(20, 20, 20) : Qt::white);
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(boton.rect, Qt::AlignCenter, boton.texto);
}

void PantallaInicio::verificarHover(const QPoint& pos) {
    if (menuPrincipal) {
        botonJugar.hover = botonJugar.rect.contains(pos);
        botonOpciones.hover = botonOpciones.rect.contains(pos);
        botonSalir.hover = botonSalir.rect.contains(pos);
    } else {
        botonNivel1.hover = botonNivel1.rect.contains(pos);
        botonNivel2.hover = botonNivel2.rect.contains(pos);
        botonNivel3.hover = botonNivel3.rect.contains(pos);
        botonSalir.hover = botonSalir.rect.contains(pos);
    }

    update();
}

void PantallaInicio::mouseMoveEvent(QMouseEvent* event) {
    verificarHover(event->pos());
}

void PantallaInicio::mousePressEvent(QMouseEvent* event) {
    if (event->button() != Qt::LeftButton) return;

    QPoint pos = event->pos();

    if (menuPrincipal) {
        if (botonJugar.rect.contains(pos)) {
            menuPrincipal = false;
            update();
        } else if (botonOpciones.rect.contains(pos)) {
            mostrarComoJugar();
        } else if (botonSalir.rect.contains(pos)) {
            emit salir();
        }
    } else {
        if (botonNivel1.rect.contains(pos)) {
            emit iniciarJuego(1);
        } else if (botonNivel2.rect.contains(pos)) {
            emit iniciarJuego(2);
        } else if (botonNivel3.rect.contains(pos)) {
            emit iniciarJuego(3);
        } else if (botonSalir.rect.contains(pos)) {
            menuPrincipal = true;
            update();
        }
    }
}

void PantallaInicio::mostrarComoJugar() {
    QDialog* dialogo = new QDialog(this);
    dialogo->setWindowTitle("¿Como Jugar?");
    dialogo->setFixedSize(500, 400);

    QVBoxLayout* layout = new QVBoxLayout(dialogo);

    QLabel* titulo = new QLabel("<h2>El Naufragio del RMS Lusitania</h2>", dialogo);
    titulo->setAlignment(Qt::AlignCenter);
    layout->addWidget(titulo);

    QLabel* instrucciones = new QLabel(
        "<p><b>CONTROLES:</b><br>"
        "• WASD o Flechas: Mover<br>"
        "• P: Pausar<br>"
        "• Shift: Habilidad especial<br><br>"

        "<b>NIVEL 1 - OCEANO:</b><br>"
        "Evita los torpedos de los submarinos enemigos durante 3 minutos.<br><br>"

        "<b>NIVEL 2 - BARCO:</b><br>"
        "Rescata 10 pasajeros llevandolos a la zona de rescate antes de que el barco se hunda.<br><br>"

        "<b>NIVEL 3 - SUBMARINO:</b><br>"
        "Escapa del naufragio nadando hasta la superficie. "
        "Usa salvavidas para recuperar oxigeno y evita los vortices.<br><br>"

        "<b>OBJETIVO:</b> ¡Completar los 3 niveles!</p>",
        dialogo
        );
    instrucciones->setWordWrap(true);
    instrucciones->setAlignment(Qt::AlignLeft);
    layout->addWidget(instrucciones);

    QPushButton* btnCerrar = new QPushButton("Cerrar", dialogo);
    connect(btnCerrar, &QPushButton::clicked, dialogo, &QDialog::accept);
    layout->addWidget(btnCerrar);

    dialogo->exec();
    delete dialogo;
}
