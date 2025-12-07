#include "hud.h"
#include "motorjuego.h"
#include "nivel2barco.h"
#include "nivel3submarino.h"
#include <QFont>

// ========== CONSTRUCTOR ==========

HUD::HUD()
    : colorFondo(QColor(0, 0, 0, 180)),
    colorTexto(QColor(255, 255, 255)),
    colorVidaAlta(QColor(0, 255, 0)),
    colorVidaMedia(QColor(255, 165, 0)),
    colorVidaBaja(QColor(255, 0, 0)) {
}

HUD::~HUD() {
}

// ========== RENDERIZADO PRINCIPAL ==========

void HUD::renderizar(QPainter& painter, Nivel* nivel, int fps) {
    if (!nivel) return;

    painter.save();

    Jugador* jugador = nivel->getJugador();

    if (jugador) {
        renderizarBarraVida(painter, jugador, 10, 10);
    }

    if (nivel->getTiempoLimite() > 0.0f) {
        renderizarBarraTiempo(painter, nivel, 540, 10);
    }

    // ===== INFORMACION ESPECIFICA POR NIVEL =====
    Nivel2Barco* nivel2 = dynamic_cast<Nivel2Barco*>(nivel);
    Nivel3Submarino* nivel3 = dynamic_cast<Nivel3Submarino*>(nivel);

    if (nivel2) {
        renderizarInfoNivel2(painter, nivel2, fps);
    } else if (nivel3) {
        renderizarInfoNivel3(painter, nivel3, jugador, fps);
    } else {
        renderizarInformacion(painter, nivel, fps);
    }

    painter.restore();
}

// ========== METODOS DE RENDERIZADO ==========

void HUD::renderizarBarraVida(QPainter& painter, Jugador* jugador, int x, int y) {
    if (!jugador) return;

    painter.setBrush(colorFondo);
    painter.setPen(QPen(Qt::white, 2));
    painter.drawRoundedRect(x, y, 300, 80, 5, 5);

    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.setPen(Qt::white);
    painter.drawText(x + 10, y + 20, "VIDA");

    int anchoBarraMax = 280;
    int altoBarraVida = 25;

    float porcentajeVida = jugador->getSalud() / jugador->getSaludMaxima();
    int anchoBarraVida = (int)(anchoBarraMax * porcentajeVida);

    QColor colorVida;
    if (porcentajeVida > 0.6f) {
        colorVida = colorVidaAlta;
    } else if (porcentajeVida > 0.3f) {
        colorVida = colorVidaMedia;
    } else {
        colorVida = colorVidaBaja;
    }

    painter.setBrush(QColor(100, 0, 0));
    painter.setPen(Qt::black);
    painter.drawRect(x + 10, y + 30, anchoBarraMax, altoBarraVida);

    painter.setBrush(colorVida);
    painter.drawRect(x + 10, y + 30, anchoBarraVida, altoBarraVida);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(x + 10, y + 70,
                     QString("%1 / %2")
                         .arg((int)jugador->getSalud())
                         .arg((int)jugador->getSaludMaxima()));
}

void HUD::renderizarBarraTiempo(QPainter& painter, Nivel* nivel, int x, int y) {
    if (!nivel) return;

    painter.setBrush(colorFondo);
    painter.setPen(QPen(Qt::white, 2));
    painter.drawRoundedRect(x, y, 250, 80, 5, 5);

    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.setPen(Qt::white);
    painter.drawText(x + 10, y + 20, "TIEMPO");

    float tiempoRestante = nivel->getTiempoLimite() - nivel->getTiempoTranscurrido();
    if (tiempoRestante < 0.0f) tiempoRestante = 0.0f;

    float porcentajeTiempo = tiempoRestante / nivel->getTiempoLimite();

    int anchoBarraMax = 230;
    int altoBarraTiempo = 25;
    int anchoBarraTiempo = (int)(anchoBarraMax * porcentajeTiempo);

    QColor colorTiempo;
    if (porcentajeTiempo > 0.5f) {
        colorTiempo = QColor(0, 200, 255);
    } else if (porcentajeTiempo > 0.25f) {
        colorTiempo = colorVidaMedia;
    } else {
        colorTiempo = colorVidaBaja;
    }

    painter.setBrush(QColor(50, 50, 50));
    painter.setPen(Qt::black);
    painter.drawRect(x + 10, y + 30, anchoBarraMax, altoBarraTiempo);

    painter.setBrush(colorTiempo);
    painter.drawRect(x + 10, y + 30, anchoBarraTiempo, altoBarraTiempo);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    int minutos = (int)tiempoRestante / 60;
    int segundos = (int)tiempoRestante % 60;
    painter.drawText(x + 10, y + 70,
                     QString("%1:%2")
                         .arg(minutos, 2, 10, QChar('0'))
                         .arg(segundos, 2, 10, QChar('0')));
}

void HUD::renderizarInformacion(QPainter& painter, Nivel* nivel, int fps) {
    if (!nivel) return;

    painter.setBrush(colorFondo);
    painter.setPen(QPen(Qt::white, 2));
    painter.drawRoundedRect(10, 510, 230, 80, 5, 5);

    painter.setFont(QFont("Arial", 11, QFont::Bold));
    painter.setPen(Qt::white);
    painter.drawText(20, 530, QString("Puntuacion: %1").arg(nivel->getPuntuacion()));
    painter.drawText(20, 548, QString("Nivel: %1").arg(MotorJuego::obtenerInstancia()->getNumeroNivelActual()));
    painter.drawText(20, 566, QString("FPS: %1").arg(fps));

    int minutos = (int)nivel->getTiempoTranscurrido() / 60;
    int segundos = (int)nivel->getTiempoTranscurrido() % 60;
    painter.drawText(20, 584, QString("Tiempo: %1:%2")
                                  .arg(minutos, 2, 10, QChar('0'))
                                  .arg(segundos, 2, 10, QChar('0')));
}

// ===== RENDERIZADO ESPECIFICO PARA NIVEL 2 =====
void HUD::renderizarInfoNivel2(QPainter& painter, Nivel2Barco* nivel2, int fps) {
    if (!nivel2) return;

    painter.setBrush(colorFondo);
    painter.setPen(QPen(Qt::white, 2));
    painter.drawRoundedRect(10, 465, 260, 125, 5, 5);

    painter.setFont(QFont("Arial", 11, QFont::Bold));
    painter.setPen(Qt::white);

    painter.drawText(20, 485, QString("Puntuacion: %1").arg(nivel2->getPuntuacion()));
    painter.drawText(20, 503, QString("Nivel: %1").arg(MotorJuego::obtenerInstancia()->getNumeroNivelActual()));
    painter.drawText(20, 521, QString("FPS: %1").arg(fps));

    // Angulo de inclinacion
    float angulo = nivel2->getAnguloBarco();
    QColor colorAngulo;
    if (angulo < 15.0f) colorAngulo = QColor(0, 255, 0);
    else if (angulo < 25.0f) colorAngulo = QColor(255, 165, 0);
    else colorAngulo = QColor(255, 0, 0);

    painter.setPen(colorAngulo);
    painter.drawText(20, 539, QString("Inclinacion: %1°").arg((int)angulo));

    // Rescatados
    painter.setPen(Qt::white);
    int rescatados = nivel2->getNPCsRescatados();
    int objetivo = nivel2->getObjetivoNPCs();
    painter.drawText(20, 557, QString("Rescatados: %1/%2").arg(rescatados).arg(objetivo));

    // ===== MUERTOS =====
    int muertos = nivel2->getNPCsMuertos();
    painter.setPen(QColor(255, 100, 100));
    painter.drawText(20, 575, QString("Muertos: %1").arg(muertos));

    painter.setPen(Qt::white);
    int minutos = (int)nivel2->getTiempoTranscurrido() / 60;
    int segundos = (int)nivel2->getTiempoTranscurrido() % 60;
    painter.drawText(20, 586, QString("Tiempo: %1:%2")
                                  .arg(minutos, 2, 10, QChar('0'))
                                  .arg(segundos, 2, 10, QChar('0')));
}

// ===== RENDERIZADO ESPECIFICO PARA NIVEL 3 =====
void HUD::renderizarInfoNivel3(QPainter& painter, Nivel3Submarino* nivel3, Jugador* jugador, int fps) {
    if (!nivel3 || !jugador) return;

    // ===== BARRA DE OXIGENO (ARRIBA A LA DERECHA) =====
    painter.setBrush(colorFondo);
    painter.setPen(QPen(Qt::white, 2));
    painter.drawRoundedRect(540, 100, 250, 80, 5, 5);

    painter.setFont(QFont("Arial", 12, QFont::Bold));
    painter.setPen(Qt::white);
    painter.drawText(550, 120, "OXIGENO");

    int anchoBarraMax = 230;
    int altoBarraOxigeno = 25;

    float porcentajeOxigeno = jugador->getOxigeno() / jugador->getOxigenoMaximo();
    int anchoBarraOxigeno = (int)(anchoBarraMax * porcentajeOxigeno);

    QColor colorOxigeno;
    if (porcentajeOxigeno > 0.5f) {
        colorOxigeno = QColor(0, 255, 255);  // Cyan
    } else if (porcentajeOxigeno > 0.25f) {
        colorOxigeno = colorVidaMedia;  // Naranja
    } else {
        colorOxigeno = colorVidaBaja;  // Rojo
    }

    painter.setBrush(QColor(50, 50, 80));
    painter.setPen(Qt::black);
    painter.drawRect(550, 130, anchoBarraMax, altoBarraOxigeno);

    painter.setBrush(colorOxigeno);
    painter.drawRect(550, 130, anchoBarraOxigeno, altoBarraOxigeno);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(550, 170,
                     QString("%1 / %2")
                         .arg((int)jugador->getOxigeno())
                         .arg((int)jugador->getOxigenoMaximo()));

    // ===== INFORMACION GENERAL =====
    painter.setBrush(colorFondo);
    painter.setPen(QPen(Qt::white, 2));
    painter.drawRoundedRect(10, 510, 230, 80, 5, 5);

    painter.setFont(QFont("Arial", 11, QFont::Bold));
    painter.setPen(Qt::white);
    painter.drawText(20, 530, QString("Puntuacion: %1").arg(nivel3->getPuntuacion()));
    painter.drawText(20, 548, QString("Profundidad: %1m").arg((int)(3000 - jugador->getPosicion().y)));
    painter.drawText(20, 566, QString("FPS: %1").arg(fps));

    int minutos = (int)nivel3->getTiempoTranscurrido() / 60;
    int segundos = (int)nivel3->getTiempoTranscurrido() % 60;
    painter.drawText(20, 584, QString("Tiempo: %1:%2")
                                  .arg(minutos, 2, 10, QChar('0'))
                                  .arg(segundos, 2, 10, QChar('0')));
}

// ========== CONFIGURACION =====

void HUD::setColorTexto(const QColor& color) {
    colorTexto = color;
}

void HUD::setColorFondo(const QColor& color) {
    colorFondo = color;
}
