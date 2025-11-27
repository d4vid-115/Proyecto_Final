#ifndef SUBMARINO_H
#define SUBMARINO_H

#include "enemigo.h"
#include "torpedo.h"
#include "agenteia.h"
#include <QPainter>
#include <vector>

// Forward declaration para evitar dependencia circular
// (cuando implementemos el AgenteIA completo)
class AgenteIA;

// Submarino U-20 - Enemigo principal del Nivel 1
// Patrulla, detecta, persigue y dispara torpedos
class Submarino : public Enemigo {
private:
    // Estado del submarino
    EstadoSubmarino estado;

    // IA (por ahora basica, luego sera AgenteIA completo)
    AgenteIA* agenteIA;

    // Patrullaje
    Vector2D puntoPatrulla;
    float tiempoEnPunto;
    float tiempoEsperaPunto;

    // Ataque
    float tiempoUltimoDisparo;
    float cooldownDisparo;       // 3 segundos entre disparos
    int torpedosDisparados;
    int maxTorpedos;             // Maximo 5 torpedos simultaneos

    // Movimiento
    float profundidad;           // Profundidad bajo el agua (50-150 px)
    bool moviendose;

    // IA basica (temporal, hasta implementar AgenteIA)
    void actualizarIABasica(float dt);
    void elegirNuevoPuntoPatrulla();
    float calcularAnguloInterceptacion(const Vector2D& objetivo);

public:
    // ========== CONSTRUCTORES ==========
    Submarino();
    Submarino(const Vector2D& pos);
    ~Submarino();

    // ========== METODOS HEREDADOS ==========
    void actualizar(float dt) override;
    void renderizar(QPainter& painter) override;

    void patrullar(float dt) override;
    void perseguir(Entidad* objetivo, float dt) override;
    void atacar(Entidad* objetivo, float dt) override;

    // ========== METODOS ESPECIFICOS ==========
    Torpedo* dispararTorpedo();
    void cambiarEstado(EstadoSubmarino nuevoEstado);
    bool puedeDisparar() const;

    // ========== GETTERS ==========
    EstadoSubmarino getEstado() const;
    float getProfundidad() const;
    int getTorpedosDisparados() const;

    // ========== SETTERS ==========
    void setEstado(EstadoSubmarino est);
    void setProfundidad(float prof);
};

#endif // SUBMARINO_H
