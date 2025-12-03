#ifndef MOVIMIENTOCIRCULAR_H
#define MOVIMIENTOCIRCULAR_H

#include "componentefisica.h"
#include "entidad.h"
#include <cmath>

// Movimiento Circular Uniforme (MCU)
// Usado para: Lamparas colgantes oscilando (Nivel 2)
// Ecuaciones:
//   x(t) = x_centro + r × cos(θ)
//   y(t) = y_centro + r × sin(θ)
//   θ(t) = θ₀ + ω × t
class MovimientoCircular : public ComponenteFisica {
private:
    float radio;                  // r - Radio del circulo (longitud de cuerda)
    float velocidadAngular;       // ω (omega) - Velocidad de rotacion
    Vector2D centroPivote;        // Centro del circulo (punto de anclaje)
    float anguloActual;           // θ (theta) - Angulo actual en radianes

    // Calcula posicion en la circunferencia dado un angulo
    Vector2D calcularPosicionCircular(float angulo) const;

public:
    // ========== CONSTRUCTORES ==========
    MovimientoCircular();
    MovimientoCircular(float r, float w, const Vector2D& centro);
    ~MovimientoCircular();

    // ========== METODOS HEREDADOS ==========
    Vector2D calcular(Entidad* e, float dt) override;
    void aplicar(Entidad* e) override;

    // ========== GETTERS Y SETTERS ==========
    float getRadio() const;
    float getVelocidadAngular() const;
    float getAnguloActual() const;
    Vector2D getCentroPivote() const;

    void setRadio(float r);
    void setVelocidadAngular(float w);
    void setCentroPivote(const Vector2D& centro);
    void setAnguloActual(float angulo);
};

#endif // MOVIMIENTOCIRCULAR_H
