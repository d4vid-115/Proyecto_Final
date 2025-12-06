#ifndef NPC_H
#define NPC_H

#include "entidad.h"
#include "tipos.h"
#include <QPainter>

// NPC - Non-Player Character (Pasajeros del Lusitania)
// Nivel 2: El jugador debe guiar NPCs a la salida
// Comportamiento: Panico, seguir al jugador, o caidos
class NPC : public Entidad {
private:
    TipoNPC tipoNPC;              // HOMBRE, MUJER, NIÑO
    EstadoNPC estadoNPC;          // PANICO, SIGUIENDO, RESCATADO, CAIDO

    // Movimiento
    float velocidadBase;          // Velocidad segun tipo
    Vector2D direccionPanico;     // Direccion aleatoria en panico
    float tiempoDireccion;        // Tiempo hasta cambiar direccion

    // Seguimiento
    Entidad* jugador;             // Referencia al jugador
    float rangoSeguimiento;       // Distancia para empezar a seguir (100 px)

    // Salud
    float resistencia;            // Resistencia a golpes (100)
    bool consciente;              // Si esta consciente o caido
    float tiempoInconsciente;     // Tiempo restante inconsciente

    // Animacion
    float frameTiempo;
    int frameActual;

    void configurarSegunTipo();
    void actualizarMovimiento(float dt);

public:
    // ========== CONSTRUCTORES ==========
    NPC();
    NPC(const Vector2D& pos, TipoNPC tipo);
    ~NPC();

    // ========== METODOS HEREDADOS ==========
    void actualizar(float dt) override;
    void renderizar(QPainter& painter) override;
    void onColision(Entidad* otra) override;

    // ========== COMPORTAMIENTO ==========
    void entrarEnPanico();
    void seguirJugador(Entidad* jugador);
    void rescatar();
    void caer(float danio);
    void despertar();

    // ========== GETTERS ==========
    TipoNPC getTipoNPC() const;
    EstadoNPC getEstadoNPC() const;
    EstadoNPC getEstado() const { return estadoNPC; }
    bool estaRescatado() const;
    bool estaConsciente() const;
    float getResistencia() const;

    // ========== SETTERS ==========
    void setEstadoNPC(EstadoNPC estado);
    void setJugador(Entidad* j);
};

#endif // NPC_H
