#ifndef ENEMIGO_H
#define ENEMIGO_H

#include "entidad.h"
#include <QPainter>

// Clase abstracta base para todos los enemigos
// Define comportamiento comun: salud, patrullaje, ataque
class Enemigo : public Entidad {
protected:
    float salud;              // Vida del enemigo
    float saludMaxima;        // Vida maxima
    float danio;              // Daño que causa al jugador
    float velocidadBase;      // Velocidad de movimiento
    bool agresivo;            // Si esta atacando activamente
    float rangoDeteccion;     // Distancia para detectar jugador
    float rangoAtaque;        // Distancia para atacar

    // Objetivo (usualmente el jugador)
    Entidad* objetivo;

public:
    // ========== CONSTRUCTORES ==========
    Enemigo();
    Enemigo(const Vector2D& pos, TipoEntidad tipo);
    virtual ~Enemigo();

    // ========== METODOS VIRTUALES PUROS ==========
    // Cada enemigo especifico debe implementar su comportamiento
    virtual void actualizar(float dt) override = 0;
    virtual void renderizar(QPainter& painter) override = 0;

    // Comportamientos especificos de enemigo
    virtual void patrullar(float dt) = 0;
    virtual void perseguir(Entidad* objetivo, float dt) = 0;
    virtual void atacar(Entidad* objetivo, float dt) = 0;

    // ========== METODOS VIRTUALES (con implementacion base) ==========
    virtual void recibirDanio(float cantidad);
    virtual void onColision(Entidad* otra) override;
    virtual void morir();

    // ========== DETECCION Y OBJETIVO ==========
    bool puedeDetectar(Entidad* e) const;
    bool puedeAtacar(Entidad* e) const;
    void setObjetivo(Entidad* obj);
    Entidad* getObjetivo() const;

    // ========== GETTERS ==========
    float getSalud() const;
    float getSaludMaxima() const;
    float getDanio() const;
    bool estaAgresivo() const;
    float getRangoDeteccion() const;
    float getRangoAtaque() const;

    // ========== SETTERS ==========
    void setSalud(float s);
    void setAgresivo(bool agr);
};

#endif // ENEMIGO_H
