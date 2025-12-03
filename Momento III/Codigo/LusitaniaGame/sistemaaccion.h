#ifndef SISTEMAACCION_H
#define SISTEMAACCION_H

#include "datosia.h"
#include "vector2d.h"
#include "entidad.h"

// Sistema de Accion
// Ejecuta las acciones decididas por el Sistema de Razonamiento
// Traduce el plan abstracto en comandos concretos
class SistemaAccion {
private:
    // Registro de ultima accion
    float tiempoUltimoDisparo;
    float cooldownDisparo;             // 3 segundos entre disparos

    Vector2D ultimoObjetivo;
    EstadoIA ultimaAccion;

public:
    // ========== CONSTRUCTOR ==========
    SistemaAccion();
    ~SistemaAccion();

    // ========== METODOS PRINCIPALES ==========

    // Ejecuta el plan de accion
    void ejecutar(const PlanAccion& plan, Entidad* agente, float dt);

    // Acciones especificas
    void moverHacia(Entidad* agente, const Vector2D& objetivo, float velocidad);
    bool disparar(Entidad* agente, float angulo);
    void patrullar(Entidad* agente, const Vector2D& puntoPatrulla, float dt);

    // ========== CONTROL DE DISPARO ==========
    bool puedeDisparar() const;
    void registrarDisparo();
    void actualizarCooldown(float dt);

    // ========== GETTERS ==========
    float getTiempoUltimoDisparo() const;
    EstadoIA getUltimaAccion() const;
};

#endif // SISTEMAACCION_H
