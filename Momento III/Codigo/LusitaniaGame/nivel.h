#ifndef NIVEL_H
#define NIVEL_H

#include "motorfisica.h"
#include "camara.h"
#include "jugador.h"
#include "tipos.h"
#include <QPainter>
#include <vector>

// Clase abstracta base para todos los niveles
// Define la estructura comun: jugador, camara, fisica, condiciones
class Nivel {
protected:
    // ===== COMPONENTES PRINCIPALES =====
    MotorFisica* motorFisica;          // Motor de fisica del nivel
    Camara* camara;                     // Camara del nivel
    Jugador* jugador;                   // Jugador (personaje principal)

    // ===== ESTADO DEL NIVEL =====
    bool completado;                    // Si el nivel fue completado
    bool fallado;                       // Si el jugador perdio
    float tiempoTranscurrido;           // Tiempo jugado en el nivel
    float tiempoLimite;                 // Tiempo límite (0 = sin limite)

    // ===== PUNTUACION =====
    int puntuacion;                     // Puntos en este nivel
    int objetivosPrincipales;           // Objetivos completados
    int objetivosSecundarios;           // Objetivos opcionales

    // ===== CONFIGURACION =====
    int anchoNivel;                     // Ancho del mundo del nivel
    int altoNivel;                      // Alto del mundo del nivel

public:
    // ========== CONSTRUCTOR Y DESTRUCTOR ==========
    Nivel();
    virtual ~Nivel();

    // ========== METODOS VIRTUALES PUROS ==========
    // Cada nivel especifico DEBE implementar estos
    virtual void inicializar() = 0;
    virtual void actualizar(float dt) = 0;
    virtual void renderizar(QPainter& painter) = 0;
    virtual void reiniciar() = 0;

    // Condiciones de victoria/derrota especificas del nivel
    virtual bool verificarVictoria() = 0;
    virtual bool verificarDerrota() = 0;

    // ========== METODOS VIRTUALES (con implementacion base) ==========
    virtual void manejarInput(int tecla, bool presionada);
    virtual void limpiar();

    // ========== GETTERS =====
    MotorFisica* getMotorFisica() const;
    Camara* getCamara() const;
    Jugador* getJugador() const;

    bool estaCompletado() const;
    bool haFallado() const;
    float getTiempoTranscurrido() const;
    float getTiempoLimite() const;
    int getPuntuacion() const;

    int getAnchoNivel() const;
    int getAltoNivel() const;

    // ========== SETTERS =====
    void setCompletado(bool comp);
    void setFallado(bool fall);
    void setPuntuacion(int puntos);
    void agregarPuntos(int puntos);
};

#endif // NIVEL_H
