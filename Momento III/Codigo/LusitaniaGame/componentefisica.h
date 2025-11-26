#ifndef COMPONENTEFISICA_H
#define COMPONENTEFISICA_H

#include "vector2d.h"

// Forward declaration (evita includes circulares)
class Entidad;

// Clase abstracta base para todos los componentes fisicos
// Diferentes físicas intercambiables
class ComponenteFisica {
public:
    ComponenteFisica() {}
    virtual ~ComponenteFisica() {}

    // ========== METODOS VIRTUALES PUROS ==========
    // Cada fisica especifica debe implementar estos metodos

    // Calcula el nuevo valor (posicion, velocidad, etc.) basado en el tiempo
    virtual Vector2D calcular(Entidad* e, float dt) = 0;

    // Aplica la fisica a la entidad
    virtual void aplicar(Entidad* e) = 0;
};

#endif // COMPONENTEFISICA_H
