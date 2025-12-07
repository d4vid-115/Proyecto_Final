#include "pantalladerrota.h"
#include "gestorsprites.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>

PantallaDerrota::PantallaDerrota(QWidget *parent)
    : QWidget(parent), puntuacion(0), nivel(1) {

    setFixedSize(800, 600);
    configurarUI();
}

void PantallaDerrota::configurarUI() {
    QVBoxLayout* layoutPrincipal = new QVBoxLayout(this);
    layoutPrincipal->setAlignment(Qt::AlignCenter);
    layoutPrincipal->setSpacing(20);

    // Titulo
    lblTitulo = new QLabel("MISION FALLIDA", this);
    lblTitulo->setAlignment(Qt::AlignCenter);
    lblTitulo->setStyleSheet(
        "QLabel {"
        "   color: #FF4444;"
        "   font-size: 48px;"
        "   font-weight: bold;"
        "   font-family: Arial;"
        "   text-shadow: 3px 3px 5px black;"
        "}"
        );

    // Nivel perdido
    lblNivel = new QLabel(this);
    lblNivel->setAlignment(Qt::AlignCenter);
    lblNivel->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   font-size: 24px;"
        "   font-weight: bold;"
        "}"
        );

    // Puntuacion
    lblPuntuacion = new QLabel(this);
    lblPuntuacion->setAlignment(Qt::AlignCenter);
    lblPuntuacion->setStyleSheet(
        "QLabel {"
        "   color: #FFAA00;"
        "   font-size: 32px;"
        "   font-weight: bold;"
        "}"
        );

    // Mensaje
    lblMensaje = new QLabel(this);
    lblMensaje->setAlignment(Qt::AlignCenter);
    lblMensaje->setWordWrap(true);
    lblMensaje->setStyleSheet(
        "QLabel {"
        "   color: white;"
        "   font-size: 16px;"
        "   padding: 20px;"
        "}"
        );

    // Botones
    QHBoxLayout* layoutBotones = new QHBoxLayout();
    layoutBotones->setSpacing(20);

    btnMenu = new QPushButton("MENÚ PRINCIPAL", this);
    btnMenu->setFixedSize(200, 50);
    btnMenu->setStyleSheet(
        "QPushButton {"
        "   background-color: #2E86C1;"
        "   color: white;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   border: 2px solid white;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #5DADE2;"
        "}"
        );

    btnReintentar = new QPushButton("REINTENTAR", this);
    btnReintentar->setFixedSize(200, 50);
    btnReintentar->setStyleSheet(
        "QPushButton {"
        "   background-color: #E74C3C;"
        "   color: white;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   border: 2px solid white;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #EC7063;"
        "}"
        );

    layoutBotones->addWidget(btnMenu);
    layoutBotones->addWidget(btnReintentar);

    // Agregar todo al layout principal
    layoutPrincipal->addWidget(lblTitulo);
    layoutPrincipal->addWidget(lblNivel);
    layoutPrincipal->addWidget(lblPuntuacion);
    layoutPrincipal->addWidget(lblMensaje);
    layoutPrincipal->addLayout(layoutBotones);

    // Conectar señales
    connect(btnMenu, &QPushButton::clicked, this, &PantallaDerrota::volverAlMenu);
    connect(btnReintentar, &QPushButton::clicked, this, &PantallaDerrota::reintentar);
}

void PantallaDerrota::mostrarResultados(int puntuacionFinal, int nivelPerdido) {
    puntuacion = puntuacionFinal;
    nivel = nivelPerdido;

    lblNivel->setText(QString("Nivel %1").arg(nivel));
    lblPuntuacion->setText(QString("Puntuacion: %1").arg(puntuacion));

    QString mensaje;
    switch (nivel) {
    case 1:
        mensaje = "El RMS Lusitania ha sido hundido por los torpedos alemanes.\n"
                  "1,198 vidas perdidas en el mar...";
        break;
    case 2:
        mensaje = "No lograste evacuar a todos los pasajeros a tiempo.\n"
                  "El barco se hundio con personas a bordo...";
        break;
    case 3:
        mensaje = "Te quedaste sin oxigeno en las profundidades.\n"
                  "El oceano reclamo otra vida...";
        break;
    }
    lblMensaje->setText(mensaje);

    update();
}

void PantallaDerrota::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Fondo con imagen
    GestorSprites* gestor = GestorSprites::obtenerInstancia();
    QPixmap fondoMenu = gestor->getSprite("menu_fondo");

    if (!fondoMenu.isNull()) {
        QPixmap fondoEscalado = fondoMenu.scaled(800, 600,
                                                 Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap(0, 0, fondoEscalado);
    } else {
        // Fallback: degradado oscuro
        QLinearGradient gradiente(0, 0, 0, 600);
        gradiente.setColorAt(0, QColor(50, 0, 0));
        gradiente.setColorAt(1, QColor(10, 0, 0));
        painter.fillRect(rect(), gradiente);
    }

    // Overlay rojo semitransparente
    painter.fillRect(rect(), QColor(100, 0, 0, 100));
}
