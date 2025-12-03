#include "motorfisica.h"
#include <algorithm>

// ========== CONSTRUCTOR ==========

MotorFisica::MotorFisica()
    : gravedad(9.8f),
    gravedadActiva(false) {
    // Constructor por defecto
    // Gravedad desactivada por defecto (se activa por nivel)
}

MotorFisica::MotorFisica(float g)
    : gravedad(g),
    gravedadActiva(true) {
    // Constructor con gravedad personalizada
}

MotorFisica::~MotorFisica() {
    limpiarEntidades();
}

// ========== CICLO PRINCIPAL ==========

void MotorFisica::actualizar(float dt) {
    // 1. Aplicar fisica a todas las entidades
    aplicarFisica(dt);

    // 2. Aplicar gravedad (si esta activa)
    if (gravedadActiva) {
        aplicarGravedad(dt);
    }

    // 3. Actualizar todas las entidades
    for (auto* entidad : entidades) {
        if (entidad && entidad->estaActivo()) {
            entidad->actualizar(dt);
        }
    }

    // 4. Verificar colisiones
    verificarColisiones();

    // 5. Eliminar entidades inactivas
    eliminarEntidadesInactivas();
}

void MotorFisica::aplicarFisica(float dt) {
    // Aplicar componentes de fisica a las entidades que los tienen
    for (auto* entidad : entidades) {
        if (entidad && entidad->estaActivo()) {
            entidad->aplicarFisica(dt);
        }
    }
}

void MotorFisica::aplicarGravedad(float dt) {
    // Aplicar gravedad a entidades que no tienen fisica personalizada
    for (auto* entidad : entidades) {
        if (entidad && entidad->estaActivo()) {
            // Solo aplicar gravedad si no tiene componente de fisica
            if (!entidad->getFisica()) {
                Vector2D velocidad = entidad->getVelocidad();
                velocidad.y += gravedad * dt;
                entidad->setVelocidad(velocidad);
            }
        }
    }
}

// ========== COLISIONES ==========

void MotorFisica::verificarColisiones() {
    // Verificacion de colisiones: Para cada par de entidades, verificar si colisionan

    size_t numEntidades = entidades.size();

    for (size_t i = 0; i < numEntidades; i++) {
        Entidad* e1 = entidades[i];
        if (!e1 || !e1->estaActivo()) continue;

        for (size_t j = i + 1; j < numEntidades; j++) {
            Entidad* e2 = entidades[j];
            if (!e2 || !e2->estaActivo()) continue;

            // Verificar colision AABB
            if (e1->colisionaCon(e2)) {
                // Notificar a ambas entidades
                e1->onColision(e2);
                e2->onColision(e1);

                // Resolver colision fisica
                resolverColision(e1, e2);
            }
        }
    }
}

void MotorFisica::resolverColision(Entidad* e1, Entidad* e2) {
    if (!e1 || !e2) return;

    // Resolucion basica de colision (separacion)
    // Calcula el vector de penetracion y separa las entidades

    CajaColision c1 = e1->getColision();
    CajaColision c2 = e2->getColision();

    // Calcular centro de cada caja
    Vector2D centro1(c1.x + c1.ancho / 2, c1.y + c1.alto / 2);
    Vector2D centro2(c2.x + c2.ancho / 2, c2.y + c2.alto / 2);

    // Vector de separacion
    Vector2D separacion = centro1 - centro2;

    // Calcular penetracion en cada eje
    float penetracionX = (c1.ancho / 2 + c2.ancho / 2) - std::abs(separacion.x);
    float penetracionY = (c1.alto / 2 + c2.alto / 2) - std::abs(separacion.y);

    // Resolver por el eje de menor penetracion
    if (penetracionX < penetracionY) {
        // Separar en X
        float signo = (separacion.x > 0) ? 1.0f : -1.0f;

        Vector2D pos1 = e1->getPosicion();
        Vector2D pos2 = e2->getPosicion();

        pos1.x += signo * penetracionX / 2;
        pos2.x -= signo * penetracionX / 2;

        e1->setPosicion(pos1);
        e2->setPosicion(pos2);
    } else {
        // Separar en Y
        float signo = (separacion.y > 0) ? 1.0f : -1.0f;

        Vector2D pos1 = e1->getPosicion();
        Vector2D pos2 = e2->getPosicion();

        pos1.y += signo * penetracionY / 2;
        pos2.y -= signo * penetracionY / 2;

        e1->setPosicion(pos1);
        e2->setPosicion(pos2);
    }
}

// ========== LIMPIEZA ==========

void MotorFisica::eliminarEntidadesInactivas() {
    auto it = entidades.begin();

    while (it != entidades.end()) {
        if (*it && !(*it)->estaActivo()) {
            // Liberar memoria y eliminar del vector
            delete *it;
            it = entidades.erase(it);
        } else {
            ++it;
        }
    }
}

// ========== GESTION DE ENTIDADES ==========

void MotorFisica::agregarEntidad(Entidad* e) {
    if (e) {
        entidades.push_back(e);
    }
}

void MotorFisica::removerEntidad(Entidad* e) {
    if (!e) return;

    // Buscar y eliminar
    auto it = std::find(entidades.begin(), entidades.end(), e);

    if (it != entidades.end()) {
        delete *it;
        entidades.erase(it);
    }
}

void MotorFisica::limpiarEntidades() {
    // Eliminar todas las entidades
    for (auto* entidad : entidades) {
        if (entidad) {
            delete entidad;
        }
    }
    entidades.clear();
}

// ========== BUSQUEDA DE ENTIDADES ==========

std::vector<Entidad*> MotorFisica::obtenerEntidadesPorTipo(TipoEntidad tipo) {
    std::vector<Entidad*> resultado;

    for (auto* entidad : entidades) {
        if (entidad && entidad->estaActivo() && entidad->getTipo() == tipo) {
            resultado.push_back(entidad);
        }
    }

    return resultado;
}

Entidad* MotorFisica::obtenerPrimeraEntidadPorTipo(TipoEntidad tipo) {
    for (auto* entidad : entidades) {
        if (entidad && entidad->estaActivo() && entidad->getTipo() == tipo) {
            return entidad;
        }
    }

    return nullptr;
}

int MotorFisica::contarEntidadesPorTipo(TipoEntidad tipo) {
    int contador = 0;

    for (auto* entidad : entidades) {
        if (entidad && entidad->estaActivo() && entidad->getTipo() == tipo) {
            contador++;
        }
    }

    return contador;
}

// ========== GETTERS ==========

std::vector<Entidad*> MotorFisica::getEntidades() const {
    return entidades;
}

int MotorFisica::getNumeroEntidades() const {
    return entidades.size();
}

float MotorFisica::getGravedad() const {
    return gravedad;
}

bool MotorFisica::getGravedadActiva() const {
    return gravedadActiva;
}

// ========== SETTERS ==========

void MotorFisica::setGravedad(float g) {
    gravedad = g;
}

void MotorFisica::setGravedadActiva(bool activa) {
    gravedadActiva = activa;
}
