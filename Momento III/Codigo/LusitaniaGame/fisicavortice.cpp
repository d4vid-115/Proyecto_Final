#include "fisicavortice.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ========== CONSTRUCTORES ==========

FisicaVortice::FisicaVortice()
    : radioInicial(150.0f),              // Radio inicial de 150 pixeles
    radioActual(150.0f),
    constanteDecaimiento(0.15f),       // Decae 15% por segundo
    fuerzaMaxima(300.0f),              // Fuerza maxima de succion
    velocidadAngular(2.0f * M_PI / 3.0f), // 2π/3 rad/s (giro)
    tiempoVida(8.0f),                  // Dura 8 segundos
    tiempoActual(0.0f),
    centroPivote(0, 0),
    activo(true) {
    // Constructor por defecto con valores del documento
}

FisicaVortice::FisicaVortice(const Vector2D& centro, float R0, float k)
    : radioInicial(R0),
    radioActual(R0),
    constanteDecaimiento(k),
    fuerzaMaxima(300.0f),
    velocidadAngular(2.0f * M_PI / 3.0f),
    tiempoVida(8.0f),
    tiempoActual(0.0f),
    centroPivote(centro),
    activo(true) {
    // Constructor con parametros personalizados
}

FisicaVortice::~FisicaVortice() {
    // Destructor vacio
}

// ========== CALCULO DE LA FISICA ==========

float FisicaVortice::calcularRadio(float t) const {
    // Decaimiento exponencial del radio:
    // R(t) = R₀ × e^(-kt)
    //
    // - Al inicio (t=0): R(0) = R₀ (radio completo)
    // - Con el tiempo: R disminuye exponencialmente
    // - Eventualmente: R → 0 (el vortice desaparece)
    //
    // e^(-kt) es la funcion exponencial decreciente
    // k controla que tan rapido decae

    return radioInicial * std::exp(-constanteDecaimiento * t);
}

float FisicaVortice::calcularFuerza(float distancia) const {
    // Fuerza de atraccion segun distancia al centro:
    // F = F_max × (1 - d/R₀)²
    //
    // - En el centro (d=0): F = F_max (maxima fuerza)
    // - En el borde (d=R₀): F = 0 (sin fuerza)
    // - El cuadrado hace que la fuerza decaiga rapidamente
    //
    // Si la entidad esta fuera del vortice, no hay fuerza

    if (distancia >= radioActual) {
        return 0.0f; // Fuera del vortice
    }

    // Normalizar distancia: d/R₀ esta en [0, 1]
    float distanciaNormalizada = distancia / radioActual;

    // Calcular fuerza con decaimiento cuadratico
    float factor = 1.0f - distanciaNormalizada;
    return fuerzaMaxima * factor * factor;
}

Vector2D FisicaVortice::calcular(Entidad* e, float dt) {
    if (!e || !activo) return e->getPosicion();

    // Calcular vector desde entidad hacia centro del vortice
    Vector2D posicionEntidad = e->getPosicion();
    Vector2D direccionAlCentro = centroPivote - posicionEntidad;
    float distancia = direccionAlCentro.magnitud();

    // Si esta fuera del radio de influencia, no aplicar fuerza
    if (distancia >= radioActual) {
        return posicionEntidad;
    }

    // Calcular componentes de la fuerza
    float fuerzaAtraccion = calcularFuerza(distancia);

    // Componente radial (hacia el centro)
    Vector2D fuerzaRadial = direccionAlCentro.normalizado() * fuerzaAtraccion;

    // Componente tangencial (rotacion)
    // Perpendicular al radio, sentido de rotacion
    Vector2D fuerzaTangencial = Vector2D(-direccionAlCentro.y, direccionAlCentro.x).normalizado();
    fuerzaTangencial *= fuerzaAtraccion * 0.3f; // 30% de la fuerza en rotacion

    // Fuerza total
    Vector2D fuerzaTotal = fuerzaRadial + fuerzaTangencial;

    // Aplicar fuerza a la velocidad
    Vector2D velocidad = e->getVelocidad();
    velocidad += fuerzaTotal * dt;

    // Calcular nueva posicion
    Vector2D nuevaPosicion = posicionEntidad + velocidad * dt;

    return nuevaPosicion;
}

void FisicaVortice::aplicar(Entidad* e) {
    if (!e || !activo) return;

    // Similar a calcular(), pero aplica directamente
    Vector2D posicionEntidad = e->getPosicion();
    Vector2D direccionAlCentro = centroPivote - posicionEntidad;
    float distancia = direccionAlCentro.magnitud();

    if (distancia >= radioActual) return;

    float fuerzaAtraccion = calcularFuerza(distancia);

    // Fuerzas radial y tangencial
    Vector2D fuerzaRadial = direccionAlCentro.normalizado() * fuerzaAtraccion;
    Vector2D fuerzaTangencial = Vector2D(-direccionAlCentro.y, direccionAlCentro.x).normalizado() * fuerzaAtraccion * 0.3f;

    Vector2D fuerzaTotal = fuerzaRadial + fuerzaTangencial;

    // Aplicar a velocidad
    Vector2D velocidad = e->getVelocidad();
    velocidad += fuerzaTotal * 0.016f; // ~60 FPS
    e->setVelocidad(velocidad);

    // Aplicar a posicion
    Vector2D nuevaPosicion = posicionEntidad + velocidad * 0.016f;
    e->setPosicion(nuevaPosicion);
}

// ========== ACTUALIZACION ==========

void FisicaVortice::actualizar(float dt) {
    if (!activo) return;

    // Incrementar tiempo
    tiempoActual += dt;

    // Actualizar radio con decaimiento exponencial
    radioActual = calcularRadio(tiempoActual);

    // Desactivar si el tiempo de vida se acabó
    if (tiempoActual >= tiempoVida) {
        activo = false;
        radioActual = 0.0f;
    }
}

// ========== GETTERS Y SETTERS ==========

float FisicaVortice::getRadioActual() const {
    return radioActual;
}

float FisicaVortice::getRadioInicial() const {
    return radioInicial;
}

Vector2D FisicaVortice::getCentro() const {
    return centroPivote;
}

bool FisicaVortice::estaActivo() const {
    return activo;
}

float FisicaVortice::getTiempoRestante() const {
    return tiempoVida - tiempoActual;
}

void FisicaVortice::setCentro(const Vector2D& centro) {
    centroPivote = centro;
}

void FisicaVortice::setFuerzaMaxima(float fmax) {
    fuerzaMaxima = fmax;
}

void FisicaVortice::reiniciar() {
    tiempoActual = 0.0f;
    radioActual = radioInicial;
    activo = true;
}
