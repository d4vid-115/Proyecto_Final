#ifndef DATOSIA_H
#define DATOSIA_H

#include "vector2d.h"
#include "tipos.h"

// ============================================
// ESTRUCTURAS DE DATOS PARA EL SISTEMA DE IA
// ============================================

// Datos de percepcion del entorno
struct DatosPercepcion {
    // Informacion del objetivo detectado
    bool objetivoDetectado;
    Vector2D posicionObjetivo;
    Vector2D velocidadObjetivo;
    float distanciaObjetivo;

    // Historial de posiciones (para predecir movimiento)
    Vector2D posicionAnterior;
    float tiempoDesdeUltimaActualizacion;

    DatosPercepcion()
        : objetivoDetectado(false),
        posicionObjetivo(0, 0),
        velocidadObjetivo(0, 0),
        distanciaObjetivo(0.0f),
        posicionAnterior(0, 0),
        tiempoDesdeUltimaActualizacion(0.0f) {}
};

// Plan de accion decidido por el razonamiento
struct PlanAccion {
    // Accion a ejecutar
    EstadoIA accion;

    // Objetivo de la accion
    Vector2D objetivoPosicion;

    // Parametros de disparo
    float anguloDisparo;
    bool debeDisparar;

    // Prioridad de la accion (0.0 = baja, 1.0 = alta)
    float prioridad;

    PlanAccion()
        : accion(EstadoIA::PATRULLANDO),
        objetivoPosicion(0, 0),
        anguloDisparo(0.0f),
        debeDisparar(false),
        prioridad(0.5f) {}
};

// Datos de aprendizaje (estadisticas de disparos)
struct DatosDisparo {
    // Informacion del disparo
    float distanciaDisparo;
    float anguloDisparo;
    Vector2D velocidadObjetivo;

    // Resultado
    bool acerto;

    // Posiciones al momento del disparo
    Vector2D posicionSubmarino;
    Vector2D posicionObjetivo;
    Vector2D posicionImpacto;

    float tiempoDisparo;

    DatosDisparo()
        : distanciaDisparo(0.0f),
        anguloDisparo(0.0f),
        velocidadObjetivo(0, 0),
        acerto(false),
        posicionSubmarino(0, 0),
        posicionObjetivo(0, 0),
        posicionImpacto(0, 0),
        tiempoDisparo(0.0f) {}
};

// Perfil de comportamiento del agente
struct PerfilIA {
    // Agresividad (0.0 = pasivo, 1.0 = muy agresivo)
    float agresividad;

    // Precision (0.0 = malo, 1.0 = perfecto)
    float precision;

    // Tiempo de reaccion (segundos)
    float tiempoReaccion;

    // Distancia preferida de ataque
    float distanciaAtaqueOptima;

    // Factor de prediccion (que tan adelante apunta)
    float factorPrediccion;

    PerfilIA()
        : agresividad(0.7f),
        precision(0.6f),
        tiempoReaccion(0.5f),
        distanciaAtaqueOptima(200.0f),
        factorPrediccion(1.0f) {}
};

#endif // DATOSIA_H
