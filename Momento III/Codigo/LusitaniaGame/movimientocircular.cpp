#include "movimientocircular.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ========== CONSTRUCTORES ==========

MovimientoCircular::MovimientoCircular()
    : radio(40.0f),                 // 40 pixeles de longitud de cuerda
    velocidadAngular(0.5f),       // Velocidad base
    centroPivote(0, 0),           // Se configurara despues
    anguloActual(0.0f) {
    // Constructor por defecto
}

MovimientoCircular::MovimientoCircular(float r, float w, const Vector2D& centro)
    : radio(r), velocidadAngular(w),
    centroPivote(centro), anguloActual(0.0f) {
    // Constructor con parametros
}

MovimientoCircular::~MovimientoCircular() {
    // Destructor
}

// ========== CALCULO DE LA FISICA ==========

Vector2D MovimientoCircular::calcularPosicionCircular(float angulo) const {
    // Ecuaciones del MCU:
    // x(t) = x_centro + r × cos(θ)
    // y(t) = y_centro + r × sin(θ)
    //
    // Esto describe un circulo centrado en (x_centro, y_centro)
    // con radio r

    float x = centroPivote.x + radio * std::cos(angulo);
    float y = centroPivote.y + radio * std::sin(angulo);

    return Vector2D(x, y);
}

Vector2D MovimientoCircular::calcular(Entidad* e, float dt) {
    // Actualizar angulo: θ(t) = θ₀ + ω × t
    // Cada frame incrementa el angulo segun la velocidad angular
    anguloActual += velocidadAngular * dt;

    // Mantener el angulo en el rango [0, 2π]
    // Esto evita que el valor crezca indefinidamente
    if (anguloActual > 2.0f * M_PI) {
        anguloActual -= 2.0f * M_PI;
    } else if (anguloActual < 0.0f) {
        anguloActual += 2.0f * M_PI;
    }

    // Calcular y retornar nueva posicion
    return calcularPosicionCircular(anguloActual);
}

void MovimientoCircular::aplicar(Entidad* e) {
    if (!e) return;

    // Calcular nueva posicion en el circulo
    Vector2D nuevaPosicion = calcularPosicionCircular(anguloActual);

    // Aplicar a la entidad
    e->setPosicion(nuevaPosicion);
}

// ========== GETTERS Y SETTERS ==========

float MovimientoCircular::getRadio() const {
    return radio;
}

float MovimientoCircular::getVelocidadAngular() const {
    return velocidadAngular;
}

float MovimientoCircular::getAnguloActual() const {
    return anguloActual;
}

Vector2D MovimientoCircular::getCentroPivote() const {
    return centroPivote;
}

void MovimientoCircular::setRadio(float r) {
    radio = r;
}

void MovimientoCircular::setVelocidadAngular(float w) {
    // La velocidad angular aumenta con la inclinacion del barco:
    // ω = 0.5 + angulo_inclinacion / 50
    velocidadAngular = w;
}

void MovimientoCircular::setCentroPivote(const Vector2D& centro) {
    // El centro es el punto de anclaje (techo donde cuelga la lampara)
    centroPivote = centro;
}

void MovimientoCircular::setAnguloActual(float angulo) {
    anguloActual = angulo;
}
