#ifndef FISICAVORTICE_H
#define FISICAVORTICE_H

#include "componentefisica.h"
#include "entidad.h"
#include <cmath>

// Fisica de Vortice (Remolinos submarinos)
// Usado para: Vortices peligrosos (Nivel 3)
// Ecuaciones:
//   R(t) = R₀ × e^(-kt)         - Radio decae exponencialmente
//   F = F_max × (1 - d/R₀)²     - Fuerza segun distancia
//   ω = 2π/3 rad/s              - Velocidad angular de giro
class FisicaVortice : public ComponenteFisica {
private:
    float radioInicial;         // R₀ - Radio inicial (150 pixeles)
    float radioActual;          // R(t) - Radio en el tiempo actual
    float constanteDecaimiento; // k - Tasa de decaimiento (0.15)
    float fuerzaMaxima;         // F_max - Fuerza en el centro
    float velocidadAngular;     // ω - Velocidad de rotacion (2π/3)
    float tiempoVida;           // Tiempo total de existencia
    float tiempoActual;         // Tiempo transcurrido
    Vector2D centroPivote;      // Centro del vortice
    bool activo;                // Si el vortice sigue vivo

    // Calcula el radio en el tiempo t
    float calcularRadio(float t) const;

    // Calcula la fuerza de atraccion segun distancia
    float calcularFuerza(float distancia) const;

public:
    // ========== CONSTRUCTORES ==========
    FisicaVortice();
    FisicaVortice(const Vector2D& centro, float R0, float k);
    ~FisicaVortice();

    // ========== METODOS HEREDADOS ==========
    Vector2D calcular(Entidad* e, float dt) override;
    void aplicar(Entidad* e) override;

    // ========== ACTUALIZACION ==========
    void actualizar(float dt);

    // ========== GETTERS Y SETTERS ==========
    float getRadioActual() const;
    float getRadioInicial() const;
    Vector2D getCentro() const;
    bool estaActivo() const;
    float getTiempoRestante() const;

    void setCentro(const Vector2D& centro);
    void setFuerzaMaxima(float fmax);
    void reiniciar();
};

#endif // FISICAVORTICE_H
