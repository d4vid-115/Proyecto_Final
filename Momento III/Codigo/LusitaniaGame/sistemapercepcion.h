#ifndef SISTEMAPERCEPCION_H
#define SISTEMAPERCEPCION_H

#include "datosia.h"
#include "entidad.h"
#include "vector2d.h"

// Sistema de Percepcion
// Detecta y rastrea al jugador
// Calcula velocidad por diferencias finitas
class SistemaPercepcion {
private:
    float radioDeteccion;              // Radio de deteccion (300 px)
    float intervaloActualizacion;      // Cada cuanto actualiza (0.5s)
    float tiempoAcumulado;             // Tiempo desde ultima actualizacion

    DatosPercepcion datosActuales;     // Datos de percepcion actuales

    // Calcular velocidad del objetivo por diferencias finitas
    Vector2D calcularVelocidad(const Vector2D& posActual, const Vector2D& posAnterior, float dt);

public:
    // ========== CONSTRUCTOR ==========
    SistemaPercepcion();
    SistemaPercepcion(float radio);
    ~SistemaPercepcion();

    // ========== METODO PRINCIPAL ==========
    // Detecta y actualiza informacion del objetivo
    DatosPercepcion detectar(Entidad* agente, Entidad* objetivo, float dt);

    // ========== GETTERS ==========
    float getRadioDeteccion() const;
    DatosPercepcion getDatosActuales() const;

    // ========== SETTERS ==========
    void setRadioDeteccion(float radio);
    void reiniciar();
};

#endif // SISTEMAPERCEPCION_H
