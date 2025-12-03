#include "sistemaaprendizaje.h"
#include <algorithm>

// ========== CONSTRUCTOR ==========

SistemaAprendizaje::SistemaAprendizaje()
    : maxHistorial(20),
    totalDisparos(0),
    totalAciertos(0),
    tasaAcierto(0.0f),
    factorAprendizaje(0.1f) {
    // Constructor por defecto
    historialDisparos.reserve(maxHistorial);
}

SistemaAprendizaje::SistemaAprendizaje(int maxHist)
    : maxHistorial(maxHist),
    totalDisparos(0),
    totalAciertos(0),
    tasaAcierto(0.0f),
    factorAprendizaje(0.1f) {
    // Constructor con tamaño de historial personalizado
    historialDisparos.reserve(maxHistorial);
}

SistemaAprendizaje::~SistemaAprendizaje() {
    historialDisparos.clear();
}

// ========== METODOS PRIVADOS ==========

void SistemaAprendizaje::analizarDisparo(const DatosDisparo& disparo) {
    // Analizar el disparo para extraer informacion util

    // Actualizar estadisticas globales
    totalDisparos++;
    if (disparo.acerto) {
        totalAciertos++;
    }

    // Calcular tasa de acierto
    if (totalDisparos > 0) {
        tasaAcierto = (float)totalAciertos / (float)totalDisparos;
    }
}

float SistemaAprendizaje::calcularErrorPromedio() {
    if (historialDisparos.empty()) return 0.0f;

    // Calcular error promedio de los ultimos disparos
    // Error = distancia entre posicion objetivo y posicion impacto

    float errorTotal = 0.0f;
    int disparosContados = 0;

    for (const auto& disparo : historialDisparos) {
        // Calcular error de este disparo
        Vector2D errorVector = disparo.posicionImpacto - disparo.posicionObjetivo;
        float errorDistancia = errorVector.magnitud();

        errorTotal += errorDistancia;
        disparosContados++;
    }

    if (disparosContados > 0) {
        return errorTotal / disparosContados;
    }

    return 0.0f;
}

void SistemaAprendizaje::ajustarPerfil(PerfilIA& perfil) {
    // Ajustar perfil basandose en el desempeño

    if (historialDisparos.size() < 5) {
        // Muy pocos datos para aprender
        return;
    }

    // ===== AJUSTAR PRECISION =====

    // Si la tasa de acierto es alta, aumentar precision
    // Si es baja, mantener o reducir ligeramente

    float tasaObjetivo = 0.6f; // Objetivo: 60% de acierto
    float diferencia = tasaAcierto - tasaObjetivo;

    // Ajustar precision gradualmente
    float ajustePrecision = diferencia * factorAprendizaje;
    perfil.precision += ajustePrecision;

    // Limitar precision entre 0.3 y 0.9
    perfil.precision = std::max(0.3f, std::min(0.9f, perfil.precision));

    // ===== AJUSTAR FACTOR DE PREDICCION =====

    // Calcular error promedio
    float errorPromedio = calcularErrorPromedio();

    // Si el error es grande, ajustar factor de prediccion
    if (errorPromedio > 50.0f) {
        // El agente esta errando mucho
        // Ajustar factor de prediccion

        // Analizar si esta apuntando muy adelante o muy atras
        float errorDireccion = 0.0f;

        for (const auto& disparo : historialDisparos) {
            // Vector desde objetivo a impacto
            Vector2D errorVector = disparo.posicionImpacto - disparo.posicionObjetivo;

            // Proyectar sobre la direccion de velocidad
            if (disparo.velocidadObjetivo.magnitud() > 0.1f) {
                Vector2D dirVel = disparo.velocidadObjetivo.normalizado();
                float proyeccion = errorVector.productoEscalar(dirVel);
                errorDireccion += proyeccion;
            }
        }

        // Si errorDireccion > 0: esta apuntando muy adelante
        // Si errorDireccion < 0: esta apuntando muy atras

        float ajustePrediccion = -errorDireccion / 1000.0f; // Normalizar
        perfil.factorPrediccion += ajustePrediccion * factorAprendizaje;

        // Limitar factor de prediccion entre 0.5 y 1.5
        perfil.factorPrediccion = std::max(0.5f, std::min(1.5f, perfil.factorPrediccion));
    }

    // ===== AJUSTAR AGRESIVIDAD =====

    // Si esta acertando mucho, ser mas agresivo
    // Si esta errando mucho, ser mas cauteloso

    if (tasaAcierto > 0.7f) {
        // Muy buena punteria, ser mas agresivo
        perfil.agresividad += 0.05f * factorAprendizaje;
    } else if (tasaAcierto < 0.4f) {
        // Mala punteria, ser mas cauteloso
        perfil.agresividad -= 0.05f * factorAprendizaje;
    }

    // Limitar agresividad entre 0.3 y 1.0
    perfil.agresividad = std::max(0.3f, std::min(1.0f, perfil.agresividad));
}

// ========== METODOS PRINCIPALES ==========

void SistemaAprendizaje::registrarDisparo(const DatosDisparo& disparo) {
    // Agregar al historial
    historialDisparos.push_back(disparo);

    // Si excede el maximo, eliminar el mas antiguo
    if (historialDisparos.size() > (size_t)maxHistorial) {
        historialDisparos.erase(historialDisparos.begin());
    }

    // Analizar el disparo
    analizarDisparo(disparo);
}

void SistemaAprendizaje::aprenderYMejorar(PerfilIA& perfil) {
    // Solo aprender si hay suficientes datos
    if (historialDisparos.size() < 3) return;

    // Ajustar perfil basandose en el historial
    ajustarPerfil(perfil);
}

// ========== ESTADISTICAS ==========

float SistemaAprendizaje::getTasaAcierto() const {
    return tasaAcierto;
}

int SistemaAprendizaje::getTotalDisparos() const {
    return totalDisparos;
}

int SistemaAprendizaje::getTotalAciertos() const {
    return totalAciertos;
}

// ========== UTILIDADES ==========

void SistemaAprendizaje::reiniciar() {
    historialDisparos.clear();
    totalDisparos = 0;
    totalAciertos = 0;
    tasaAcierto = 0.0f;
}

void SistemaAprendizaje::limpiarHistorial() {
    historialDisparos.clear();
}
