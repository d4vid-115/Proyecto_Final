#include "agenteia.h"

// ========== CONSTRUCTORES ==========

AgenteIA::AgenteIA()
    : percepcion(nullptr),
    razonamiento(nullptr),
    accion(nullptr),
    aprendizaje(nullptr),
    agente(nullptr),
    objetivo(nullptr),
    tiempoDesdeUltimoAprendizaje(0.0f),
    intervaloAprendizaje(5.0f) {

    // Crear los 4 subsistemas
    percepcion = new SistemaPercepcion(300.0f);
    razonamiento = new SistemaRazonamiento(perfil);
    accion = new SistemaAccion();
    aprendizaje = new SistemaAprendizaje(20);
}

AgenteIA::AgenteIA(Entidad* agenteSub, Entidad* obj)
    : percepcion(nullptr),
    razonamiento(nullptr),
    accion(nullptr),
    aprendizaje(nullptr),
    agente(agenteSub),
    objetivo(obj),
    tiempoDesdeUltimoAprendizaje(0.0f),
    intervaloAprendizaje(5.0f) {

    // Crear los 4 subsistemas
    percepcion = new SistemaPercepcion(300.0f);
    razonamiento = new SistemaRazonamiento(perfil);
    accion = new SistemaAccion();
    aprendizaje = new SistemaAprendizaje(20);
}

AgenteIA::~AgenteIA() {
    // Liberar memoria de los subsistemas
    if (percepcion) {
        delete percepcion;
        percepcion = nullptr;
    }

    if (razonamiento) {
        delete razonamiento;
        razonamiento = nullptr;
    }

    if (accion) {
        delete accion;
        accion = nullptr;
    }

    if (aprendizaje) {
        delete aprendizaje;
        aprendizaje = nullptr;
    }

    agente = nullptr;
    objetivo = nullptr;
}

// ========== CICLO PRINCIPAL ==========

void AgenteIA::actualizar(float dt) {
    if (!agente || !percepcion || !razonamiento || !accion) {
        return;
    }

    // ===== PASO 1: PERCEPCION =====
    // Detectar y rastrear al objetivo
    datosPercepcion = percepcion->detectar(agente, objetivo, dt);

    // ===== PASO 2: RAZONAMIENTO =====
    // Decidir que hacer basandose en la percepcion
    planActual = razonamiento->decidir(datosPercepcion, agente->getPosicion());

    // ===== PASO 3: ACCION =====
    // Ejecutar el plan decidido
    accion->ejecutar(planActual, agente, dt);

    // ===== PASO 4: APRENDIZAJE (periodico) =====
    // Cada intervaloAprendizaje segundos, mejorar el perfil
    tiempoDesdeUltimoAprendizaje += dt;

    if (tiempoDesdeUltimoAprendizaje >= intervaloAprendizaje) {
        if (aprendizaje) {
            aprendizaje->aprenderYMejorar(perfil);

            // Actualizar perfil en razonamiento
            razonamiento->setPerfil(perfil);
        }

        tiempoDesdeUltimoAprendizaje = 0.0f;
    }
}

// ========== CONFIGURACION ==========

void AgenteIA::setAgente(Entidad* agenteSub) {
    agente = agenteSub;
}

void AgenteIA::setObjetivo(Entidad* obj) {
    objetivo = obj;
}

void AgenteIA::setPerfil(const PerfilIA& p) {
    perfil = p;

    if (razonamiento) {
        razonamiento->setPerfil(perfil);
    }
}

// ========== REGISTRO DE DISPAROS ==========

void AgenteIA::registrarDisparo(const DatosDisparo& disparo) {
    if (aprendizaje) {
        aprendizaje->registrarDisparo(disparo);
    }
}

// ========== GETTERS ==========

DatosPercepcion AgenteIA::getDatosPercepcion() const {
    return datosPercepcion;
}

PlanAccion AgenteIA::getPlanActual() const {
    return planActual;
}

PerfilIA AgenteIA::getPerfil() const {
    return perfil;
}

float AgenteIA::getTasaAcierto() const {
    if (aprendizaje) {
        return aprendizaje->getTasaAcierto();
    }
    return 0.0f;
}

int AgenteIA::getTotalDisparos() const {
    if (aprendizaje) {
        return aprendizaje->getTotalDisparos();
    }
    return 0;
}

bool AgenteIA::debeDisparar() const {
    return planActual.debeDisparar && accion->puedeDisparar();
}

float AgenteIA::getAnguloDisparo() const {
    return planActual.anguloDisparo;
}

EstadoIA AgenteIA::getEstadoActual() const {
    return planActual.accion;
}
