#include "sistemarazonamiento.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ========== CONSTRUCTOR ==========

SistemaRazonamiento::SistemaRazonamiento()
    : distanciaEvadir(100.0f),
    distanciaAtaqueMin(150.0f),
    distanciaAtaqueMax(250.0f),
    distanciaAcercarse(250.0f) {
    // Constructor con perfil por defecto
}

SistemaRazonamiento::SistemaRazonamiento(const PerfilIA& p)
    : perfil(p),
    distanciaEvadir(100.0f),
    distanciaAtaqueMin(150.0f),
    distanciaAtaqueMax(250.0f),
    distanciaAcercarse(250.0f) {
    // Constructor con perfil personalizado
    // Ajustar distancia optima segun perfil
    distanciaAtaqueMin = perfil.distanciaAtaqueOptima - 50.0f;
    distanciaAtaqueMax = perfil.distanciaAtaqueOptima + 50.0f;
}

SistemaRazonamiento::~SistemaRazonamiento() {
}

// ========== METODOS PRIVADOS ==========

Vector2D SistemaRazonamiento::predecirPosicion(const Vector2D& posActual,
                                               const Vector2D& velocidad,
                                               float tiempoFuturo) {
    // Prediccion lineal simple:
    // pos_futura = pos_actual + velocidad × tiempo
    //
    // Asume que el objetivo mantiene velocidad constante

    return posActual + velocidad * tiempoFuturo * perfil.factorPrediccion;
}

float SistemaRazonamiento::calcularAnguloInterceptacion(const Vector2D& posAgente,
                                                        const Vector2D& posObjetivo,
                                                        const Vector2D& velObjetivo,
                                                        float velProyectil) {
    // Calculo de angulo de interceptacion
    //
    // Problema: ¿A dónde disparar para que el torpedo y el objetivo
    //           se encuentren en el mismo punto?
    //
    // Solución simplificada:
    // 1. Calcular tiempo de vuelo aproximado
    // 2. Predecir posicion futura del objetivo
    // 3. Apuntar a esa posición

    Vector2D direccion = posObjetivo - posAgente;
    float distancia = direccion.magnitud();

    // Tiempo estimado de vuelo
    float tiempoVuelo = calcularTiempoVuelo(distancia, velProyectil);

    // Predecir posicion futura del objetivo
    Vector2D posicionFutura = predecirPosicion(posObjetivo, velObjetivo, tiempoVuelo);

    // Calcular angulo hacia esa posicion
    Vector2D direccionIntercepcion = posicionFutura - posAgente;

    return std::atan2(direccionIntercepcion.y, direccionIntercepcion.x);
}

float SistemaRazonamiento::calcularTiempoVuelo(float distancia, float velocidadProyectil) {
    // Tiempo de vuelo simplificado:
    // t = distancia / velocidad
    //
    // (ignora gravedad y resistencia del agua para simplificar)

    if (velocidadProyectil <= 0.0f) return 0.0f;

    return distancia / velocidadProyectil;
}

// ========== METODO PRINCIPAL ==========

PlanAccion SistemaRazonamiento::decidir(const DatosPercepcion& percepcion,
                                        const Vector2D& posicionAgente) {
    PlanAccion plan;

    // Si no hay objetivo detectado, patrullar
    if (!percepcion.objetivoDetectado) {
        plan.accion = EstadoIA::PATRULLANDO;
        plan.debeDisparar = false;
        plan.prioridad = 0.2f;
        return plan;
    }

    // ===== HAY OBJETIVO DETECTADO =====

    float distancia = percepcion.distanciaObjetivo;

    // MAQUINA DE ESTADOS basada en distancia

    if (distancia < distanciaEvadir) {
        // ===== ESTADO: EVADIR =====
        // Demasiado cerca, alejarse
        plan.accion = EstadoIA::EVADIENDO;
        plan.objetivoPosicion = posicionAgente + (posicionAgente - percepcion.posicionObjetivo).normalizado() * 100.0f;
        plan.debeDisparar = false;
        plan.prioridad = 0.9f; // Alta prioridad (supervivencia)

    } else if (distancia >= distanciaAtaqueMin && distancia <= distanciaAtaqueMax) {
        // ===== ESTADO: ATACAR =====
        // Distancia optima de ataque
        plan.accion = EstadoIA::ATACANDO;
        plan.objetivoPosicion = percepcion.posicionObjetivo;

        // Calcular angulo de disparo con interceptacion
        plan.anguloDisparo = calcularAnguloInterceptacion(
            posicionAgente,
            percepcion.posicionObjetivo,
            percepcion.velocidadObjetivo,
            200.0f // Velocidad del torpedo
            );

        // Decidir si disparar (segun agresividad)
        // Mas agresivo = dispara mas seguido
        float probabilidadDisparo = perfil.agresividad;
        plan.debeDisparar = (rand() % 100) < (probabilidadDisparo * 100);

        plan.prioridad = 0.8f;

    } else if (distancia > distanciaAtaqueMax) {
        // ===== ESTADO: ACERCARSE =====
        // Muy lejos, acercarse
        plan.accion = EstadoIA::ACERCANDOSE;

        // Predecir posicion futura para intercepcion
        float tiempoAcercamiento = distancia / 60.0f; // Velocidad base submarino
        plan.objetivoPosicion = predecirPosicion(
            percepcion.posicionObjetivo,
            percepcion.velocidadObjetivo,
            tiempoAcercamiento
            );

        plan.debeDisparar = false;
        plan.prioridad = 0.6f;

    } else {
        // Caso por defecto (entre evadir y atacar)
        plan.accion = EstadoIA::PATRULLANDO;
        plan.debeDisparar = false;
        plan.prioridad = 0.3f;
    }

    return plan;
}

// ========== GETTERS ==========

PerfilIA SistemaRazonamiento::getPerfil() const {
    return perfil;
}

// ========== SETTERS ==========

void SistemaRazonamiento::setPerfil(const PerfilIA& p) {
    perfil = p;

    // Actualizar umbrales segun nuevo perfil
    distanciaAtaqueMin = perfil.distanciaAtaqueOptima - 50.0f;
    distanciaAtaqueMax = perfil.distanciaAtaqueOptima + 50.0f;
}

void SistemaRazonamiento::actualizarPerfil(float nuevaPrecision, float nuevaAgresividad) {
    perfil.precision = nuevaPrecision;
    perfil.agresividad = nuevaAgresividad;
}
