#ifndef HUD_H
#define HUD_H

#include <QPainter>
#include <QString>
#include "nivel.h"

// HUD - Heads-Up Display
// Muestra informacion del juego en pantalla
class HUD {
private:
    // Colores y estilos
    QColor colorFondo;
    QColor colorTexto;
    QColor colorVidaAlta;
    QColor colorVidaMedia;
    QColor colorVidaBaja;

    // Metodos de renderizado
    void renderizarBarraVida(QPainter& painter, Jugador* jugador, int x, int y);
    void renderizarBarraTiempo(QPainter& painter, Nivel* nivel, int x, int y);
    void renderizarInformacion(QPainter& painter, Nivel* nivel, int fps);
    void renderizarMiniMapa(QPainter& painter, Nivel* nivel, int x, int y);

public:
    // ========== CONSTRUCTOR ==========
    HUD();
    ~HUD();

    // ========== RENDERIZADO PRINCIPAL ==========
    void renderizar(QPainter& painter, Nivel* nivel, int fps);

    // ========== CONFIGURACION =====
    void setColorTexto(const QColor& color);
    void setColorFondo(const QColor& color);
};

#endif // HUD_H
