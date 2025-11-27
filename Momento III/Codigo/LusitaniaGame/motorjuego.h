#ifndef MOTORJUEGO_H
#define MOTORJUEGO_H

#include "tipos.h"

// Forward declarations
class Nivel;

// Motor del Juego - Singleton Pattern
// Controla el flujo del juego: niveles, estados, transiciones
class MotorJuego {
private:
    // ===== SINGLETON =====
    static MotorJuego* instancia;

    // Constructor privado (solo accesible internamente)
    MotorJuego();

    // ===== ESTADO DEL JUEGO =====
    EstadoJuego estadoActual;
    EstadoJuego estadoAnterior;

    // ===== NIVELES =====
    Nivel* nivelActual;
    int numeroNivelActual;        // 1, 2, o 3

    // ===== CONTROL DE TIEMPO =====
    float tiempoTotal;            // Tiempo total jugado
    float tiempoPausado;          // Tiempo en pausa
    bool pausado;

    // ===== PUNTUACION GLOBAL =====
    int puntuacionTotal;
    int vidasRestantes;

public:
    // ===== SINGLETON - OBTENER INSTANCIA =====
    static MotorJuego* obtenerInstancia();
    static void destruirInstancia();

    // Prohibir copia y asignacion (singleton)
    MotorJuego(const MotorJuego&) = delete;
    MotorJuego& operator=(const MotorJuego&) = delete;

    // ===== DESTRUCTOR =====
    ~MotorJuego();

    // ===== CICLO PRINCIPAL =====
    void actualizar(float dt);
    void renderizar();  // Se implementará cuando tengamos GUI

    // ===== GESTION DE NIVELES =====
    void cargarNivel(int numeroNivel);
    void siguienteNivel();
    void reiniciarNivel();
    void volverAlMenu();

    // ===== GESTION DE ESTADOS =====
    void cambiarEstado(EstadoJuego nuevoEstado);
    void pausar();
    void reanudar();
    void gameOver();
    void victoria();

    // ===== GETTERS =====
    EstadoJuego getEstado() const;
    EstadoJuego getEstadoAnterior() const;
    Nivel* getNivelActual() const;
    int getNumeroNivelActual() const;
    float getTiempoTotal() const;
    int getPuntuacionTotal() const;
    int getVidasRestantes() const;
    bool estaPausado() const;

    // ===== SETTERS =====
    void setPuntuacionTotal(int puntos);
    void agregarPuntos(int puntos);
    void setVidasRestantes(int vidas);
};

#endif // MOTORJUEGO_H
