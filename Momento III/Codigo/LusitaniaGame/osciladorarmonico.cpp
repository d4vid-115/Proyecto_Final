#include "osciladorarmonico.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ========== CONSTRUCTORES ==========

OsciladorArmonico::OsciladorArmonico()
    : amplitud(15.0f),                    // 15 pixeles de altura
    frecuencia(2.0f * M_PI / 3.0f),     // Periodo de 3 segundos
    fase(0.0f),                          // Sin desplazamiento inicial
    posicionBase(0.0f),                  // Se configurara despues
    tiempoTotal(0.0f) {
    // Constructor por defecto con valores del documento
}

OsciladorArmonico::OsciladorArmonico(float A, float freq, float phi)
    : amplitud(A), frecuencia(freq), fase(phi),
    posicionBase(0.0f), tiempoTotal(0.0f) {
    // Constructor con parametros personalizados
}

OsciladorArmonico::~OsciladorArmonico() {
    // Destructor vacio
}

// ========== CALCULO DE LA FISICA ==========

float OsciladorArmonico::calcularDesplazamiento(float t) const {
    // Ecuacion MAS: y(t) = A × sin(ωt + φ)
    //
    // A (amplitud): Determina que tan alto/bajo oscila
    // ω (frecuencia): Determina que tan rapido oscila
    // φ (fase): Determina donde empieza en el ciclo
    // t (tiempo): Tiempo transcurrido

    return amplitud * std::sin(frecuencia * t + fase);
}

Vector2D OsciladorArmonico::calcular(Entidad* e, float dt) {
    // Acumular tiempo
    tiempoTotal += dt;

    // Calcular desplazamiento vertical
    float offsetY = calcularDesplazamiento(tiempoTotal);

    // Obtener posicion actual de la entidad
    Vector2D posicionActual = e->getPosicion();

    // Retornar nueva posicion con oscilacion solo en Y
    // X se mantiene, Y oscila alrededor de posicionBase
    return Vector2D(posicionActual.x, posicionBase + offsetY);
}

void OsciladorArmonico::aplicar(Entidad* e) {
    // Incrementar tiempo (asumiendo ~60 FPS)
    tiempoTotal += 0.016f;

    // Calcular desplazamiento
    float offsetY = calcularDesplazamiento(tiempoTotal);

    // Obtener posicion actual
    Vector2D posicionActual = e->getPosicion();

    // Aplicar solo en el eje Y (oscilacion vertical)
    e->setPosicion(Vector2D(posicionActual.x, posicionBase + offsetY));
}

// ========== GETTERS Y SETTERS ==========

float OsciladorArmonico::getAmplitud() const {
    return amplitud;
}

float OsciladorArmonico::getFrecuencia() const {
    return frecuencia;
}

void OsciladorArmonico::setAmplitud(float A) {
    amplitud = A;
}

void OsciladorArmonico::setFrecuencia(float freq) {
    frecuencia = freq;
}

void OsciladorArmonico::setPosicionBase(float yBase) {
    // La posicion base es el centro de oscilacion
    // Si el barco esta en y=300, oscilara entre 285 y 315 (con A=15)
    posicionBase = yBase;
}
