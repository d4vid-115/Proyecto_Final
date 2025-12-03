#ifndef AGENTEIA_H
#define AGENTEIA_H

#include "datosia.h"
#include "sistemapercepcion.h"
#include "sistemarazonamiento.h"
#include "sistemaaccion.h"
#include "sistemaaprendizaje.h"
#include "entidad.h"

// Agente IA - Integrador de los 4 sistemas
// Arquitectura: Percepción → Razonamiento → Accion → Aprendizaje

class AgenteIA {
private:
    // Los 4 subsistemas
    SistemaPercepcion* percepcion;
    SistemaRazonamiento* razonamiento;
    SistemaAccion* accion;
    SistemaAprendizaje* aprendizaje;

    // Perfil de comportamiento
    PerfilIA perfil;

    // Referencias
    Entidad* agente;        // El submarino que controla
    Entidad* objetivo;      // El jugador

    // Estado interno
    DatosPercepcion datosPercepcion;
    PlanAccion planActual;

    // Aprendizaje
    float tiempoDesdeUltimoAprendizaje;
    float intervaloAprendizaje;      // Cada 5 segundos aprende

public:
    // ========== CONSTRUCTORES ==========
    AgenteIA();
    AgenteIA(Entidad* agenteSub, Entidad* obj);
    ~AgenteIA();

    // ========== CICLO PRINCIPAL ==========
    // Este metodo llama cada frame
    void actualizar(float dt);

    // ========== CONFIGURACION ==========
    void setAgente(Entidad* agenteSub);
    void setObjetivo(Entidad* obj);
    void setPerfil(const PerfilIA& p);

    // ========== REGISTRO DE DISPAROS ==========
    void registrarDisparo(const DatosDisparo& disparo);

    // ========== GETTERS ==========
    DatosPercepcion getDatosPercepcion() const;
    PlanAccion getPlanActual() const;
    PerfilIA getPerfil() const;

    // Estadisticas
    float getTasaAcierto() const;
    int getTotalDisparos() const;

    // Estados
    bool debeDisparar() const;
    float getAnguloDisparo() const;
    EstadoIA getEstadoActual() const;
};

#endif // AGENTEIA_H
