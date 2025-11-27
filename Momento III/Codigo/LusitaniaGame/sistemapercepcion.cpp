#include "sistemapercepcion.h"

// ========== CONSTRUCTOR ==========

SistemaPercepcion::SistemaPercepcion()
    : radioDeteccion(300.0f),
    intervaloActualizacion(0.5f),
    tiempoAcumulado(0.0f) {
    // Constructor por defecto
}

SistemaPercepcion::SistemaPercepcion(float radio)
    : radioDeteccion(radio),
    intervaloActualizacion(0.5f),
    tiempoAcumulado(0.0f) {
    // Constructor con radio personalizado
}

SistemaPercepcion::~SistemaPercepcion() {
    // Destructor vacio
}

// ========== METODOS PRIVADOS ==========

Vector2D SistemaPercepcion::calcularVelocidad(const Vector2D& posActual,
                                              const Vector2D& posAnterior,
                                              float dt) {
    // Velocidad por diferencias finitas:
    // v = (pos_actual - pos_anterior) / dt
    //
    // Esto da una aproximacion de la velocidad real
    // Cuanto menor el dt, más precisa la aproximacion

    if (dt <= 0.0f) return Vector2D(0, 0);

    Vector2D desplazamiento = posActual - posAnterior;
    return desplazamiento / dt;
}

// ========== METODO PRINCIPAL ==========

DatosPercepcion SistemaPercepcion::detectar(Entidad* agente, Entidad* objetivo, float dt) {
    if (!agente || !objetivo) {
        // Sin agente u objetivo, no hay deteccion
        datosActuales.objetivoDetectado = false;
        return datosActuales;
    }

    // Acumular tiempo
    tiempoAcumulado += dt;

    // Calcular distancia al objetivo
    Vector2D posAgente = agente->getPosicion();
    Vector2D posObjetivo = objetivo->getPosicion();
    float distancia = posAgente.distanciaA(posObjetivo);

    // Verificar si esta en rango de deteccion
    if (distancia > radioDeteccion) {
        // Fuera de rango
        datosActuales.objetivoDetectado = false;
        datosActuales.distanciaObjetivo = distancia;
        return datosActuales;
    }

    // ===== OBJETIVO DETECTADO =====
    datosActuales.objetivoDetectado = true;
    datosActuales.distanciaObjetivo = distancia;

    // Actualizar velocidad solo cada intervaloActualizacion
    // (evita calculos innecesarios y ruido)
    if (tiempoAcumulado >= intervaloActualizacion) {
        // Guardar posicion anterior para el proximo calculo
        datosActuales.posicionAnterior = datosActuales.posicionObjetivo;

        // Actualizar posicion actual
        datosActuales.posicionObjetivo = posObjetivo;

        // Calcular velocidad por diferencias finitas
        datosActuales.velocidadObjetivo = calcularVelocidad(
            datosActuales.posicionObjetivo,
            datosActuales.posicionAnterior,
            tiempoAcumulado
            );

        // Resetear acumulador
        datosActuales.tiempoDesdeUltimaActualizacion = tiempoAcumulado;
        tiempoAcumulado = 0.0f;
    } else {
        // Entre actualizaciones, solo actualizar posicion
        datosActuales.posicionObjetivo = posObjetivo;
    }

    return datosActuales;
}

// ========== GETTERS ==========

float SistemaPercepcion::getRadioDeteccion() const {
    return radioDeteccion;
}

DatosPercepcion SistemaPercepcion::getDatosActuales() const {
    return datosActuales;
}

// ========== SETTERS ==========

void SistemaPercepcion::setRadioDeteccion(float radio) {
    radioDeteccion = radio;
}

void SistemaPercepcion::reiniciar() {
    datosActuales = DatosPercepcion();
    tiempoAcumulado = 0.0f;
}
