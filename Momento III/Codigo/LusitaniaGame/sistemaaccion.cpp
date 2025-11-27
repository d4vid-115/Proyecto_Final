#include "sistemaaccion.h"
#include <cmath>

// ========== CONSTRUCTOR ==========

SistemaAccion::SistemaAccion()
    : tiempoUltimoDisparo(0.0f),
    cooldownDisparo(3.0f),
    ultimoObjetivo(0, 0),
    ultimaAccion(EstadoIA::PATRULLANDO) {
    // Constructor por defecto
}

SistemaAccion::~SistemaAccion() {
}

// ========== METODOS PRINCIPALES ==========

void SistemaAccion::ejecutar(const PlanAccion& plan, Entidad* agente, float dt) {
    if (!agente) return;

    // Actualizar cooldown de disparo
    actualizarCooldown(dt);

    // Registrar accion
    ultimaAccion = plan.accion;
    ultimoObjetivo = plan.objetivoPosicion;

    // Ejecutar segun el tipo de accion
    switch (plan.accion) {
    case EstadoIA::PATRULLANDO:
        // Moverse lentamente hacia punto de patrulla
        patrullar(agente, plan.objetivoPosicion, dt);
        break;

    case EstadoIA::ACERCANDOSE:
        // Moverse hacia el objetivo
        moverHacia(agente, plan.objetivoPosicion, 60.0f); // Velocidad normal
        break;

    case EstadoIA::ATACANDO:
        // Mantener distancia y disparar si es posible
        if (plan.debeDisparar && puedeDisparar()) {
            disparar(agente, plan.anguloDisparo);
        }
        // Movimiento minimo para mantener posicion
        moverHacia(agente, plan.objetivoPosicion, 20.0f); // Velocidad baja
        break;

    case EstadoIA::EVADIENDO:
        // Alejarse rapidamente
        moverHacia(agente, plan.objetivoPosicion, 80.0f); // Velocidad alta
        break;
    }
}

void SistemaAccion::moverHacia(Entidad* agente, const Vector2D& objetivo, float velocidad) {
    if (!agente) return;

    // Calcular direccion hacia el objetivo
    Vector2D posicionActual = agente->getPosicion();
    Vector2D direccion = objetivo - posicionActual;

    // Normalizar y aplicar velocidad
    if (direccion.magnitud() > 1.0f) {
        Vector2D velocidadVector = direccion.normalizado() * velocidad;
        agente->setVelocidad(velocidadVector);
    } else {
        // Ya llego al objetivo, detenerse
        agente->setVelocidad(Vector2D(0, 0));
    }
}

bool SistemaAccion::disparar(Entidad* agente, float angulo) {
    if (!agente || !puedeDisparar()) return false;

    // El disparo real se maneja en el nivel/submarino
    // Aqui solo registramos que se disparo
    registrarDisparo();

    return true;
}

void SistemaAccion::patrullar(Entidad* agente, const Vector2D& puntoPatrulla, float dt) {
    if (!agente) return;

    // Moverse lentamente hacia el punto de patrulla
    // Velocidad reducida (40% de la normal)
    moverHacia(agente, puntoPatrulla, 40.0f);
}

// ========== CONTROL DE DISPARO ==========

bool SistemaAccion::puedeDisparar() const {
    // Solo puede disparar si el cooldown ha terminado
    return tiempoUltimoDisparo <= 0.0f;
}

void SistemaAccion::registrarDisparo() {
    // Reiniciar cooldown
    tiempoUltimoDisparo = cooldownDisparo;
}

void SistemaAccion::actualizarCooldown(float dt) {
    if (tiempoUltimoDisparo > 0.0f) {
        tiempoUltimoDisparo -= dt;
        if (tiempoUltimoDisparo < 0.0f) {
            tiempoUltimoDisparo = 0.0f;
        }
    }
}

// ========== GETTERS ==========

float SistemaAccion::getTiempoUltimoDisparo() const {
    return tiempoUltimoDisparo;
}

EstadoIA SistemaAccion::getUltimaAccion() const {
    return ultimaAccion;
}
