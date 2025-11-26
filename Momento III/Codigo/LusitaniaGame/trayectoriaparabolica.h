#ifndef TRAYECTORIAPARABOLICA_H
#define TRAYECTORIAPARABOLICA_H

#include "componentefisica.h"
#include "entidad.h"

// Movimiento Parabolico
// Usado para: Torpedos submarinos (Nivel 1)
// Ecuaciones:
//   x(t) = x₀ + v₀ₓ × t
//   y(t) = y₀ + v₀ᵧ × t - ½ × g × t²
class TrayectoriaParabolica : public ComponenteFisica {
private:
    float v0x;                  // Velocidad inicial en X
    float v0y;                  // Velocidad inicial en Y
    float gravedad;             // g = 50 px/s² (resistencia del agua)
    float tiempoInicio;         // Tiempo de inicio del movimiento
    float tiempoActual;         // Tiempo actual desde el inicio
    Vector2D posicionInicial;   // Punto de lanzamiento (x₀, y₀)

    // Calcula la posicion en el tiempo t
    Vector2D calcularPosicion(float t) const;

public:
    // ========== CONSTRUCTORES ==========
    TrayectoriaParabolica();
    TrayectoriaParabolica(float vx, float vy, float g);
    ~TrayectoriaParabolica();

    // ========== METODOS HEREDADOS ==========
    Vector2D calcular(Entidad* e, float dt) override;
    void aplicar(Entidad* e) override;

    // ========== CONFIGURACION ==========
    void setVelocidadInicial(float vx, float vy);
    void setGravedad(float g);
    void setPosicionInicial(const Vector2D& pos);
    void reiniciarTiempo();
};

#endif // TRAYECTORIAPARABOLICA_H
