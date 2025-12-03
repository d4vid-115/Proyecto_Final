#ifndef VORTICE_H
#define VORTICE_H

#include "entidad.h"
#include "fisicavortice.h"
#include <QPainter>

// Vortice - Remolino submarino peligroso (Nivel 3)
// Atrae entidades cercanas con fuerza de succión
// Decae exponencialmente hasta desaparecer
class Vortice : public Entidad {
private:
    FisicaVortice* fisicaVortice;    // Fisica del vortice
    float radioActual;                // Radio de influencia actual
    float radioInicial;               // Radio inicial (150 px)
    float tiempoVida;                 // Tiempo total de existencia (8s)
    float tiempoActual;               // Tiempo transcurrido
    float rotacionActual;             // Angulo de rotacion para animacion

    // Particulas visuales para el efecto espiral
    struct Particula {
        float angulo;
        float distancia;
        float velocidadAngular;
    };
    Particula particulas[20];         // 20 particulas en espiral

    void inicializarParticulas();

public:
    // ========== CONSTRUCTORES ==========
    Vortice();
    Vortice(const Vector2D& centro);
    Vortice(const Vector2D& centro, float radio, float vida);
    ~Vortice();

    // ========== METODOS HEREDADOS ==========
    void actualizar(float dt) override;
    void renderizar(QPainter& painter) override;
    void onColision(Entidad* otra) override;

    // ========== METODOS ESPECIFICOS ==========
    void aplicarFuerzaA(Entidad* e);
    float getRadioActual() const;
    float getTiempoRestante() const;
    bool haExpirado() const;

    // ========== GETTERS ==========
    Vector2D getCentro() const;
    float getFuerzaEn(const Vector2D& punto) const;
};

#endif // VORTICE_H
