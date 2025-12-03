#ifndef ENTIDAD_H
#define ENTIDAD_H

#include "vector2d.h"
#include "tipos.h"
#include <QPainter>

class ComponenteFisica;

// Clase base abstracta para TODAS las entidades del juego
// Usa polimorfismo: Jugador, Enemigo, Torpedo, etc. heredan de aquí
class Entidad {
protected:
    Vector2D posicion;           // Posicion en el mundo
    Vector2D velocidad;          // Velocidad actual
    CajaColision colision;       // Caja de colision AABB
    TipoEntidad tipo;            // Tipo de entidad (para identificar)
    bool activo;                 // Si esta viva/activa
    ComponenteFisica* fisica;    // Componente de fisica (puede ser nullptr)

    // Sprite (simplificado por ahora)
    int ancho, alto;             // Dimensiones visuales

public:
    // ========== CONSTRUCTORES Y DESTRUCTOR ==========
    Entidad();
    Entidad(const Vector2D& pos, TipoEntidad t);
    virtual ~Entidad();

    // ========== METODOS VIRTUALES PUROS ==========
    // Cada entidad especifica DEBE implementar estos
    virtual void actualizar(float dt) = 0;
    virtual void renderizar(QPainter& painter) = 0;

    // ========== METODOS VIRTUALES (con implementacion base) ==========
    virtual void destruir();
    virtual void onColision(Entidad* otra);

    // ========== FISICA ==========
    void setFisica(ComponenteFisica* f);
    ComponenteFisica* getFisica() const;
    void aplicarFisica(float dt);

    // ========== COLISIONES ==========
    bool colisionaCon(Entidad* otra) const;
    CajaColision getColision() const;
    void actualizarColision();  // Sincroniza caja con posicion

    // ========== GETTERS ==========
    Vector2D getPosicion() const;
    Vector2D getVelocidad() const;
    TipoEntidad getTipo() const;
    bool estaActivo() const;
    int getAncho() const;
    int getAlto() const;

    // ========== SETTERS ==========
    void setPosicion(const Vector2D& pos);
    void setVelocidad(const Vector2D& vel);
    void setActivo(bool estado);
    void setDimensiones(int w, int h);
};

#endif // ENTIDAD_H
