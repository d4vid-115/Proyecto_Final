#ifndef OSCILADORARMONICO_H
#define OSCILADORARMONICO_H

#include "componentefisica.h"
#include "entidad.h"
#include <cmath>

// Movimiento Armonico Simple (MAS)
// Usado para: Oscilación del Lusitania en las olas (Nivel 1)
// Ecuación: y(t) = A × sin(ωt + φ)
class OsciladorArmonico : public ComponenteFisica {
private:
    float amplitud;           // A - Altura de oscilacion (15 pixeles)
    float frecuencia;         // ω (omega) - Velocidad de oscilación (2π/3)
    float fase;               // φ (phi) - Desplazamiento inicial (0)
    float posicionBase;       // y₀ - Posicion central
    float tiempoTotal;        // Tiempo acumulado

public:
    // ========== CONSTRUCTORES ==========
    OsciladorArmonico();
    OsciladorArmonico(float A, float freq, float phi);
    ~OsciladorArmonico();

    // ========== METODOS HEREDADOS ==========
    Vector2D calcular(Entidad* e, float dt) override;
    void aplicar(Entidad* e) override;

    // Calcula el desplazamiento en un momento dado
    float calcularDesplazamiento(float t) const;

    // ========== GETTERS Y SETTERS ==========
    float getAmplitud() const;
    float getFrecuencia() const;
    void setAmplitud(float A);
    void setFrecuencia(float freq);
    void setPosicionBase(float yBase);
};

#endif // OSCILADORARMONICO_H
