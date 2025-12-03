#include "camara.h"

// ========== CONSTRUCTORES ==========

Camara::Camara()
    : posicion(0, 0), ancho(800), alto(600),
    limiteMin(0, 0), limiteMax(2000, 2000), tieneScroll(true) {
    // Constructor por defecto: camara 800x600 con scroll
}

Camara::Camara(float _ancho, float _alto)
    : posicion(0, 0), ancho(_ancho), alto(_alto),
    limiteMin(0, 0), limiteMax(2000, 2000), tieneScroll(true) {
    // Constructor con dimensiones especificas
}

// ========== METODOS DE POSICIONAMIENTO ==========

void Camara::setPosicion(const Vector2D& pos) {
    posicion = pos;

    // Aplicar limites para que la camara no salga del mundo
    if (tieneScroll) {
        // No puede ir mas a la izquierda de limiteMin.x
        if (posicion.x < limiteMin.x)
            posicion.x = limiteMin.x;

        // No puede ir mas arriba de limiteMin.y
        if (posicion.y < limiteMin.y)
            posicion.y = limiteMin.y;

        // No puede ir mas a la derecha (camara + ancho <= limite)
        if (posicion.x + ancho > limiteMax.x)
            posicion.x = limiteMax.x - ancho;

        // No puede ir mas abajo
        if (posicion.y + alto > limiteMax.y)
            posicion.y = limiteMax.y - alto;
    }
}

void Camara::mover(const Vector2D& desplazamiento) {
    // Mueve la camara relativamente
    setPosicion(posicion + desplazamiento);
}

void Camara::seguirObjetivo(const Vector2D& objetivo, float dt) {
    if (!tieneScroll) return;

    // Centrar la camara en el objetivo
    Vector2D centroDeseado = objetivo - Vector2D(ancho / 2, alto / 2);

    // Interpolacion suave (smooth follow)
    // En vez de saltar directamente, se mueve gradualmente
    Vector2D diferencia = centroDeseado - posicion;
    Vector2D movimiento = diferencia * (5.0f * dt); // Factor de suavizado

    setPosicion(posicion + movimiento);
}

void Camara::setLimites(const Vector2D& min, const Vector2D& max) {
    limiteMin = min;
    limiteMax = max;
}

void Camara::setTieneScroll(bool scroll) {
    tieneScroll = scroll;
}

// ========== GETTERS ==========

Vector2D Camara::getPosicion() const {
    return posicion;
}

float Camara::getAncho() const {
    return ancho;
}

float Camara::getAlto() const {
    return alto;
}

bool Camara::getTieneScroll() const {
    return tieneScroll;
}

// ========== CONVERSION DE COORDENADAS ==========

Vector2D Camara::mundoAPantalla(const Vector2D& posicionMundo) const {
    // Si el jugador esta en x=1000 del mundo
    // y la camara esta en x=600
    // entonces en pantalla se dibuja en x=400 (1000-600)
    return posicionMundo - posicion;
}

Vector2D Camara::pantallaAMundo(const Vector2D& posicionPantalla) const {
    // Operaciin inversa
    return posicionPantalla + posicion;
}

bool Camara::estaVisible(const Vector2D& punto) const {
    // Verifica si un punto esta dentro del viewport
    return punto.x >= posicion.x && punto.x <= posicion.x + ancho &&
           punto.y >= posicion.y && punto.y <= posicion.y + alto;
}
