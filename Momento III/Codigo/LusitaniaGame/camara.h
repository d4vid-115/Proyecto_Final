#ifndef CAMARA_H
#define CAMARA_H

#include "vector2d.h"

// Sistema de camara para seguir al jugador y manejar scroll
// Convierte coordenadas del mundo a coordenadas de pantalla
class Camara {
private:
    Vector2D posicion;      // Posicion de la camara en el mundo
    float ancho;            // Ancho del viewport
    float alto;             // Alto del viewport
    Vector2D limiteMin;     // Limite minimo del mundo (0, 0)
    Vector2D limiteMax;     // Limite maximo del mundo (tamanio del nivel)
    bool tieneScroll;       // Si false, camara fija

public:
    // ========== CONSTRUCTORES ==========
    Camara();
    Camara(float _ancho, float _alto);

    // ========== METODOS DE POSICIONAMIENTO ==========
    void setPosicion(const Vector2D& pos);           // Posicionar directamente
    void mover(const Vector2D& desplazamiento);      // Mover relativamente
    void seguirObjetivo(const Vector2D& objetivo, float dt); // Seguir suavemente
    void setLimites(const Vector2D& min, const Vector2D& max);
    void setTieneScroll(bool scroll);

    // ========== GETTERS ==========
    Vector2D getPosicion() const;
    float getAncho() const;
    float getAlto() const;
    bool getTieneScroll() const;

    // ========== CONVERSION DE COORDENADAS ==========
    // Convierte posicion del mundo (ej: jugador en x=1000)
    // a posicion de pantalla (ej: píxel x=400)
    Vector2D mundoAPantalla(const Vector2D& posicionMundo) const;
    Vector2D pantallaAMundo(const Vector2D& posicionPantalla) const;

    // Verifica si un punto esta visible en pantalla
    bool estaVisible(const Vector2D& punto) const;
};

#endif // CAMARA_H
