#ifndef HUD_H
#define HUD_H

#include "nivel.h"
#include "jugador.h"
#include "nivel2barco.h"
#include "nivel3submarino.h"
#include <QPainter>

class HUD {
private:
    QColor colorFondo;
    QColor colorTexto;
    QColor colorVidaAlta;
    QColor colorVidaMedia;
    QColor colorVidaBaja;

    void renderizarBarraVida(QPainter& painter, Jugador* jugador, int x, int y);
    void renderizarBarraTiempo(QPainter& painter, Nivel* nivel, int x, int y);
    void renderizarInformacion(QPainter& painter, Nivel* nivel, int fps);
    void renderizarInfoNivel2(QPainter& painter, Nivel2Barco* nivel2, int fps);
    void renderizarInfoNivel3(QPainter& painter, Nivel3Submarino* nivel3, Jugador* jugador, int fps);

public:
    HUD();
    ~HUD();

    void renderizar(QPainter& painter, Nivel* nivel, int fps);

    void setColorTexto(const QColor& color);
    void setColorFondo(const QColor& color);
};

#endif // HUD_H
