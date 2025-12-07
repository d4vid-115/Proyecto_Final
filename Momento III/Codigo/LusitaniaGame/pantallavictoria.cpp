#include "pantallavictoria.h"
#include "gestorsprites.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>

PantallaVictoria::PantallaVictoria(QWidget *parent)
    : QWidget(parent), puntuacion(0), nivel(1) {

    setFixedSize(800, 600);
    configurarUI();
}

void PantallaVictoria::configurarUI() {
    QVBoxLayout* layoutPrincipal = new QVBoxLayout(this);
    layoutPrincipal->setAlignment(Qt::AlignCenter);
    layoutPrincipal->setSpacing(20);

    // Titulo
    lblTitulo = new QLabel("¡VICTORIA!", this);
    lblTitulo->setAlignment(Qt::AlignCenter);
    lblTitulo->setStyleSheet(
        "QLabel {"
        "   color: #FFD700;"
        "   font-size: 48px;"
        "   font-weight: bold;"
        "   font-family: Arial;"
        "   text-shadow: 3px 3px 5px black;"
        "}"
        );

    // Nivel completado
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
        "   color: #00FF00;"
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

    btnMenu = new QPushButton("MENU PRINCIPAL", this);
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

    btnReintentar = new QPushButton("JUGAR DE NUEVO", this);
    btnReintentar->setFixedSize(200, 50);
    btnReintentar->setStyleSheet(
        "QPushButton {"
        "   background-color: #27AE60;"
        "   color: white;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   border: 2px solid white;"
        "   border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #58D68D;"
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
    connect(btnMenu, &QPushButton::clicked, this, &PantallaVictoria::volverAlMenu);
    connect(btnReintentar, &QPushButton::clicked, this, &PantallaVictoria::jugarDeNuevo);
}

void PantallaVictoria::mostrarResultados(int puntuacionFinal, int nivelCompletado) {
    puntuacion = puntuacionFinal;
    nivel = nivelCompletado;

    lblNivel->setText(QString("Nivel %1 Completado").arg(nivel));
    lblPuntuacion->setText(QString("Puntuacion: %1").arg(puntuacion));

    QString mensaje;
    switch (nivel) {
    case 1:
        mensaje = "Has defendido al RMS Lusitania del ataque aleman.\n"
                  "Ahora debes evacuar a los pasajeros...";
        break;
    case 2:
        mensaje = "¡Evacuacion exitosa! Has rescatado a los pasajeros.\n"
                  "Pero el barco se esta hundiendo...";
        break;
    case 3:
        mensaje = "¡Has escapado del naufragio!\n"
                  "Completaste la misión historica del Lusitania.";
        break;
    }
    lblMensaje->setText(mensaje);

    update();
}

void PantallaVictoria::paintEvent(QPaintEvent* event) {
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
        // Fallback: degradado
        QLinearGradient gradiente(0, 0, 0, 600);
        gradiente.setColorAt(0, QColor(0, 100, 150));
        gradiente.setColorAt(1, QColor(0, 30, 60));
        painter.fillRect(rect(), gradiente);
    }

    // Overlay semitransparente
    painter.fillRect(rect(), QColor(0, 0, 0, 150));

    // Estrellas decorativas (efecto victoria)
    painter.setPen(Qt::NoPen);
    for (int i = 0; i < 50; ++i) {
        int x = (i * 137) % 800;
        int y = (i * 211) % 600;
        int size = 2 + (i % 4);

        painter.setBrush(QColor(255, 215, 0, 150 + (i % 105)));
        painter.drawEllipse(x, y, size, size);
    }
}
