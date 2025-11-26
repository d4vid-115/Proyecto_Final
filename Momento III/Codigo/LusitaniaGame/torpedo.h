#ifndef TORPEDO_H
#define TORPEDO_H

#include "entidad.h"
#include "trayectoriaparabolica.h"
#include <QPainter>

// Torpedo - Proyectil con fisica parabolica
// Disparado por submarinos enemigos
class Torpedo : public Entidad {
private:
    float anguloDisparo;      // Angulo de lanzamiento (radianes)
    float velocidadInicial;   // Velocidad de disparo (200 px/s)
    float tiempoVida;         // Tiempo maximo de existencia (6s)
    float tiempoActual;       // Tiempo transcurrido
    TrayectoriaParabolica* trayectoria; // Fisica parabolica

public:
    // ========== CONSTRUCTORES ==========
    Torpedo();
    Torpedo(const Vector2D& pos, float angulo, float velocidad);
    ~Torpedo();

    // ========== METODOS HEREDADOS ==========
    void actualizar(float dt) override;
    void renderizar(QPainter& painter) override;
    void onColision(Entidad* otra) override;

    // ========== CONFIGURACION ==========
    void configurarTrayectoria(const Vector2D& posInicial, float angulo, float vel);

    // ========== GETTERS ==========
    float getAnguloDisparo() const;
    float getTiempoRestante() const;
};

#endif // TORPEDO_H
