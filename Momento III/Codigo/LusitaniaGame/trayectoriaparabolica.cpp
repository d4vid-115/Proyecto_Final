#include "trayectoriaparabolica.h"

// ========== CONSTRUCTORES ==========

TrayectoriaParabolica::TrayectoriaParabolica()
    : v0x(0.0f), v0y(0.0f), gravedad(50.0f),
    tiempoInicio(0.0f), tiempoActual(0.0f), posicionInicial(0, 0) {
    // Constructor por defecto
    // g = 50 px/s² simula resistencia del agua
}

TrayectoriaParabolica::TrayectoriaParabolica(float vx, float vy, float g)
    : v0x(vx), v0y(vy), gravedad(g),
    tiempoInicio(0.0f), tiempoActual(0.0f), posicionInicial(0, 0) {
    // Constructor con parametros
}

TrayectoriaParabolica::~TrayectoriaParabolica() {
    // Destructor vacio
}

// ========== CALCULO DE LA FISICA ==========

Vector2D TrayectoriaParabolica::calcularPosicion(float t) const {
    // Ecuaciones del movimiento parabolico:
    //
    // Horizontal (MRU - sin aceleracion):
    //   x(t) = x₀ + v₀ₓ × t
    //
    // Vertical (MRUA - con gravedad):
    //   y(t) = y₀ + v₀ᵧ × t - ½ × g × t²
    //
    // El termino "- 1/2 × g × t²" hace que caiga (o suba mas lento)

    float x = posicionInicial.x + v0x * t;
    float y = posicionInicial.y + v0y * t - 0.5f * gravedad * t * t;

    return Vector2D(x, y);
}

Vector2D TrayectoriaParabolica::calcular(Entidad* e, float dt) {
    // Incrementar tiempo
    tiempoActual += dt;

    // Calcular y retornar nueva posicion
    return calcularPosicion(tiempoActual);
}

void TrayectoriaParabolica::aplicar(Entidad* e) {
    if (!e) return;

    // Calcular nueva posicion
    Vector2D nuevaPosicion = calcularPosicion(tiempoActual);

    // Actualizar tambien la velocidad (para efectos visuales de rotacion)
    // vx permanece constante (MRU)
    // vy disminuye linealmente: vy(t) = v₀y - g×t
    float vx = v0x;
    float vy = v0y - gravedad * tiempoActual;
    e->setVelocidad(Vector2D(vx, vy));

    // Aplicar nueva posicion
    e->setPosicion(nuevaPosicion);
}

// ========== CONFIGURACION ==========

void TrayectoriaParabolica::setVelocidadInicial(float vx, float vy) {
    v0x = vx;
    v0y = vy;
}

void TrayectoriaParabolica::setGravedad(float g) {
    gravedad = g;
}

void TrayectoriaParabolica::setPosicionInicial(const Vector2D& pos) {
    posicionInicial = pos;
}

void TrayectoriaParabolica::reiniciarTiempo() {
    tiempoActual = 0.0f;
    tiempoInicio = 0.0f;
}
