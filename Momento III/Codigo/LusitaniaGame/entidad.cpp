#include "entidad.h"
#include "componentefisica.h"

// ========== CONSTRUCTORES Y DESTRUCTOR ==========

Entidad::Entidad()
    : posicion(0, 0), velocidad(0, 0),
    colision(0, 0, 32, 32),
    tipo(TipoEntidad::JUGADOR),
    activo(true),
    fisica(nullptr),
    ancho(32), alto(32) {
    // Constructor por defecto
}

Entidad::Entidad(const Vector2D& pos, TipoEntidad t)
    : posicion(pos), velocidad(0, 0),
    colision(pos.x, pos.y, 32, 32),
    tipo(t),
    activo(true),
    fisica(nullptr),
    ancho(32), alto(32) {
    // Constructor con posicion y tipo
}

Entidad::~Entidad() {
    // Liberar memoria de fisica si existe
    if (fisica) {
        delete fisica;
        fisica = nullptr;
    }
}

// ========== METODOS VIRTUALES ==========

void Entidad::destruir() {
    // Desactiva la entidad (sera eliminada despues)
    activo = false;
}

void Entidad::onColision(Entidad* otra) {
    // Implementacion base vacia
    // Las clases derivadas pueden sobrescribir para manejar colisiones especificas
    (void)otra; // Evita warning de parametro no usado
}

// ========== FISICA ==========

void Entidad::setFisica(ComponenteFisica* f) {
    // Liberar fisica anterior si existe
    if (fisica) {
        delete fisica;
    }
    fisica = f;
}

ComponenteFisica* Entidad::getFisica() const {
    return fisica;
}

void Entidad::aplicarFisica(float dt) {
    // Si tiene componente de fisica, aplicarlo
    if (fisica) {
        Vector2D nuevaPosicion = fisica->calcular(this, dt);
        setPosicion(nuevaPosicion);
    }
}

// ========== COLISIONES ==========

bool Entidad::colisionaCon(Entidad* otra) const {
    if (!otra || !activo || !otra->estaActivo()) {
        return false;
    }

    // Usar el metodo de CajaColision
    return colision.colisionaCon(otra->getColision());
}

CajaColision Entidad::getColision() const {
    return colision;
}

void Entidad::actualizarColision() {
    // Sincronizar caja de colision con posicion actual
    colision.x = posicion.x;
    colision.y = posicion.y;
    colision.ancho = ancho;
    colision.alto = alto;
}

// ========== GETTERS ==========

Vector2D Entidad::getPosicion() const {
    return posicion;
}

Vector2D Entidad::getVelocidad() const {
    return velocidad;
}

TipoEntidad Entidad::getTipo() const {
    return tipo;
}

bool Entidad::estaActivo() const {
    return activo;
}

int Entidad::getAncho() const {
    return ancho;
}

int Entidad::getAlto() const {
    return alto;
}

// ========== SETTERS ==========

void Entidad::setPosicion(const Vector2D& pos) {
    posicion = pos;
    actualizarColision();
}

void Entidad::setVelocidad(const Vector2D& vel) {
    velocidad = vel;
}

void Entidad::setActivo(bool estado) {
    activo = estado;
}

void Entidad::setDimensiones(int w, int h) {
    ancho = w;
    alto = h;
    actualizarColision();
}
