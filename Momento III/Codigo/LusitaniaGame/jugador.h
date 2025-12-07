#ifndef JUGADOR_H
#define JUGADOR_H

#include "entidad.h"
#include <QPainter>

// Clase Jugador - El personaje controlado por el usuario
// Hereda de Entidad y añade mecanicas especificas
class Jugador : public Entidad {
private:
    float salud;              // Vida actual (0-100)
    float saludMaxima;        // Vida maxima (100)
    int vidas;                // Vidas restantes (3)
    int puntuacion;           // Puntos acumulados
    float velocidadBase;      // Velocidad de movimiento (150 px/s)
    float oxigeno;

    // Habilidad especial
    bool habilidadActiva;     // Si esta usando habilidad
    float tiempoHabilidad;    // Tiempo restante de habilidad
    float cooldownHabilidad;  // Tiempo hasta poder usar otra vez (5s)
    float tiempoCooldown;     // Contador de cooldown

    // Invencibilidad temporal (al recibir daño)
    bool invencible;
    float tiempoInvencibilidad;

public:
    // ========== CONSTRUCTORES ==========
    Jugador();
    Jugador(const Vector2D& pos);
    ~Jugador();

    // ========== METODOS HEREDADOS ==========
    void actualizar(float dt) override;
    void renderizar(QPainter& painter) override;
    void renderizarNadador(QPainter& painter);
    void onColision(Entidad* otra) override;

    // ========== MOVIMIENTO ==========
    void mover(const Vector2D& direccion, float dt);
    void detener();

    // ========== SALUD Y DAÑO ==========
    void recibirDanio(float cantidad);
    void curar(float cantidad);
    bool estaMuerto() const;

    // ========== HABILIDAD ESPECIAL ==========
    void usarHabilidad();
    bool puedeUsarHabilidad() const;

    // ========== VIDAS Y PUNTUACION ==========
    void perderVida();
    void ganarVida();
    void agregarPuntos(int puntos);

    // ========== GETTERS ==========
    float getSalud() const;
    float getSaludMaxima() const;
    int getVidas() const;
    int getPuntuacion() const;
    float getVelocidadBase() const;
    float getOxigeno() const { return oxigeno; }
    bool estaInvencible() const;

    // ========== SETTERS ==========
    void setSalud(float s);
    void setVidas(int v);
    void setOxigeno(float o) { oxigeno = o; }
};

#endif // JUGADOR_H
