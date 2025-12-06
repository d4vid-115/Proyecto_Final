#include "hud.h"
#include "motorjuego.h"
#include <QFont>

// ========== CONSTRUCTOR ==========

HUD::HUD()
    : colorFondo(QColor(0, 0, 0, 150)),
    colorTexto(QColor(255, 255, 255)),
    colorVidaAlta(QColor(0, 255, 0)),
    colorVidaMedia(QColor(255, 165, 0)),
    colorVidaBaja(QColor(255, 0, 0)) {
    // Constructor
}

HUD::~HUD() {
}

// ========== RENDERIZADO PRINCIPAL ==========

void HUD::renderizar(QPainter& painter, Nivel* nivel, int fps) {
    if (!nivel) return;

    painter.save();

    // Renderizar elementos del HUD
    Jugador* jugador = nivel->getJugador();

    if (jugador) {
        // Barra de vida (esquina superior izquierda)
        renderizarBarraVida(painter, jugador, 10, 10);
    }

    // Barra de tiempo (esquina superior derecha)
    if (nivel->getTiempoLimite() > 0.0f) {
        renderizarBarraTiempo(painter, nivel, 550, 10);
    }

    // Informacion del juego (esquina inferior izquierda)
    renderizarInformacion(painter, nivel, fps);

    // Mini-mapa (esquina inferior derecha)
    // renderizarMiniMapa(painter, nivel, 650, 450);

    painter.restore();
}

// ========== METODOS DE RENDERIZADO ==========

void HUD::renderizarBarraVida(QPainter& painter, Jugador* jugador, int x, int y) {
    if (!jugador) return;

    // Fondo de la barra
    painter.setBrush(colorFondo);
    painter.setPen(Qt::white);
    painter.drawRect(x, y, 240, 60);

    // Texto "VIDA"
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(x + 5, y + 15, "VIDA");

    // Barra de vida
    int anchoBarraMax = 220;
    int altoBarraVida = 20;

    float porcentajeVida = jugador->getSalud() / jugador->getSaludMaxima();
    int anchoBarraVida = (int)(anchoBarraMax * porcentajeVida);

    // Color segun cantidad de vida
    QColor colorVida;
    if (porcentajeVida > 0.6f) {
        colorVida = colorVidaAlta;
    } else if (porcentajeVida > 0.3f) {
        colorVida = colorVidaMedia;
    } else {
        colorVida = colorVidaBaja;
    }

    // Fondo de la barra (rojo oscuro)
    painter.setBrush(QColor(100, 0, 0));
    painter.drawRect(x + 10, y + 20, anchoBarraMax, altoBarraVida);

    // Barra de vida actual
    painter.setBrush(colorVida);
    painter.drawRect(x + 10, y + 20, anchoBarraVida, altoBarraVida);

    // Texto de vida numerica
    painter.setPen(Qt::white);
    painter.drawText(x + 10, y + 55,
                     QString("%1 / %2")
                         .arg((int)jugador->getSalud())
                         .arg((int)jugador->getSaludMaxima()));

    // Vidas (corazones)
    painter.drawText(x + 150, y + 55,
                     QString("Vidas: %1").arg(jugador->getVidas()));
}

void HUD::renderizarBarraTiempo(QPainter& painter, Nivel* nivel, int x, int y) {
    if (!nivel) return;

    // Fondo
    painter.setBrush(colorFondo);
    painter.setPen(Qt::white);
    painter.drawRect(x, y, 240, 60);

    // Texto "TIEMPO"
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(x + 5, y + 15, "TIEMPO");

    // Calcular tiempo restante
    float tiempoRestante = nivel->getTiempoLimite() - nivel->getTiempoTranscurrido();
    if (tiempoRestante < 0.0f) tiempoRestante = 0.0f;

    float porcentajeTiempo = tiempoRestante / nivel->getTiempoLimite();

    // Barra de tiempo
    int anchoBarraMax = 220;
    int altoBarraTiempo = 20;
    int anchoBarraTiempo = (int)(anchoBarraMax * porcentajeTiempo);

    // Color segun tiempo restante
    QColor colorTiempo;
    if (porcentajeTiempo > 0.5f) {
        colorTiempo = QColor(0, 200, 255);
    } else if (porcentajeTiempo > 0.25f) {
        colorTiempo = colorVidaMedia;
    } else {
        colorTiempo = colorVidaBaja;
    }

    // Fondo de la barra (gris oscuro)
    painter.setBrush(QColor(50, 50, 50));
    painter.drawRect(x + 10, y + 20, anchoBarraMax, altoBarraTiempo);

    // Barra de tiempo actual
    painter.setBrush(colorTiempo);
    painter.drawRect(x + 10, y + 20, anchoBarraTiempo, altoBarraTiempo);

    // Texto de tiempo numerico
    painter.setPen(Qt::white);
    int minutos = (int)tiempoRestante / 60;
    int segundos = (int)tiempoRestante % 60;
    painter.drawText(x + 10, y + 55,
                     QString("%1:%2")
                         .arg(minutos, 2, 10, QChar('0'))
                         .arg(segundos, 2, 10, QChar('0')));
}

void HUD::renderizarInformacion(QPainter& painter, Nivel* nivel, int fps) {
    if (!nivel) return;

    // Fondo
    painter.setBrush(colorFondo);
    painter.setPen(Qt::white);
    painter.drawRect(10, 520, 200, 70);

    // Informacion
    painter.setFont(QFont("Arial", 10));
    painter.drawText(15, 535, QString("Puntuacion: %1").arg(nivel->getPuntuacion()));
    painter.drawText(15, 550, QString("Nivel: %1").arg(MotorJuego::obtenerInstancia()->getNumeroNivelActual()));
    painter.drawText(15, 565, QString("FPS: %1").arg(fps));
    painter.drawText(15, 580, QString("Tiempo: %1s").arg((int)nivel->getTiempoTranscurrido()));
}

void HUD::renderizarMiniMapa(QPainter& painter, Nivel* nivel, int x, int y) {
    // Mini-mapa (opcional, placeholder)
    if (!nivel) return;

    painter.setBrush(QColor(0, 0, 0, 200));
    painter.setPen(Qt::white);
    painter.drawRect(x, y, 140, 140);

    painter.drawText(x + 40, y + 75, "MAPA");
}

// ========== CONFIGURACION ==========

void HUD::setColorTexto(const QColor& color) {
    colorTexto = color;
}

void HUD::setColorFondo(const QColor& color) {
    colorFondo = color;
}
