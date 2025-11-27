#ifndef SISTEMARAZONAMIENTO_H
#define SISTEMARAZONAMIENTO_H

#include "datosia.h"
#include "vector2d.h"
#include <cmath>

// Sistema de Razonamiento
// Decide que accion tomar basandose en los datos de percepcion
// Usa maquina de estados: PATRULLAR, ACERCARSE, ATACAR, EVADIR
class SistemaRazonamiento {
private:
    PerfilIA perfil;                   // Perfil de comportamiento

    // Umbrales de decision
    float distanciaEvadir;             // < 100 px → EVADIR
    float distanciaAtaqueMin;          // 150 px
    float distanciaAtaqueMax;          // 250 px
    float distanciaAcercarse;          // > 250 px → ACERCARSE

    // Prediccion de movimiento
    Vector2D predecirPosicion(const Vector2D& posActual,
                              const Vector2D& velocidad,
                              float tiempoFuturo);

    // Calculo de angulo de interceptacion
    float calcularAnguloInterceptacion(const Vector2D& posAgente,
                                       const Vector2D& posObjetivo,
                                       const Vector2D& velObjetivo,
                                       float velProyectil);

    // Calculo de tiempo de vuelo del torpedo
    float calcularTiempoVuelo(float distancia, float velocidadProyectil);

public:
    // ========== CONSTRUCTOR ==========
    SistemaRazonamiento();
    SistemaRazonamiento(const PerfilIA& perfil);
    ~SistemaRazonamiento();

    // ========== METODO PRINCIPAL ==========
    // Decide que hacer basandose en la percepcion
    PlanAccion decidir(const DatosPercepcion& percepcion,
                       const Vector2D& posicionAgente);

    // ========== GETTERS ==========
    PerfilIA getPerfil() const;

    // ========== SETTERS ==========
    void setPerfil(const PerfilIA& perfil);
    void actualizarPerfil(float nuevaPrecision, float nuevaAgresividad);
};

#endif // SISTEMARAZONAMIENTO_H
