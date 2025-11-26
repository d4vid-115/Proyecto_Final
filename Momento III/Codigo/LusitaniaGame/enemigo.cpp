#include "enemigo.h"

// ========== CONSTRUCTORES ==========

Enemigo::Enemigo()
    : Entidad(Vector2D(0, 0), TipoEntidad::ENEMIGO_SUBMARINO),
    salud(100.0f), saludMaxima(100.0f),
    danio(20.0f), velocidadBase(80.0f),
    agresivo(false),
    rangoDeteccion(300.0f),
    rangoAtaque(200.0f),
    objetivo(nullptr) {
    // Constructor base
}

Enemigo::Enemigo(const Vector2D& pos, TipoEntidad tipo)
    : Entidad(pos, tipo),
    salud(100.0f), saludMaxima(100.0f),
    danio(20.0f), velocidadBase(80.0f),
    agresivo(false),
    rangoDeteccion(300.0f),
    rangoAtaque(200.0f),
    objetivo(nullptr) {
    // Constructor con posicion y tipo
}

Enemigo::~Enemigo() {
    objetivo = nullptr;
}

// ========== METODOS VIRTUALES ==========

void Enemigo::recibirDanio(float cantidad) {
    salud -= cantidad;

    // Ponerse agresivo al recibir daño
    agresivo = true;

    // Morir si la salud llega a 0
    if (salud <= 0.0f) {
        salud = 0.0f;
        morir();
    }
}

void Enemigo::onColision(Entidad* otra) {
    if (!otra) return;

    // Si colisiona con el jugador, causarle daño
    if (otra->getTipo() == TipoEntidad::JUGADOR) {
        // El daño se maneja desde el jugador
        // Aqui solo registramos que hubo colision
    }
}

void Enemigo::morir() {
    activo = false;

    // Aqui se podria generar particulas de explosion,
    // sumar puntos al jugador, etc.
}

// ========== DETECCION Y OBJETIVO ==========

bool Enemigo::puedeDetectar(Entidad* e) const {
    if (!e || !e->estaActivo()) return false;

    // Calcular distancia al objetivo
    float distancia = posicion.distanciaA(e->getPosicion());

    // Puede detectar si esta dentro del rango
    return distancia <= rangoDeteccion;
}

bool Enemigo::puedeAtacar(Entidad* e) const {
    if (!e || !e->estaActivo()) return false;

    float distancia = posicion.distanciaA(e->getPosicion());

    // Puede atacar si esta dentro del rango de ataque
    return distancia <= rangoAtaque;
}

void Enemigo::setObjetivo(Entidad* obj) {
    objetivo = obj;

    // Si tiene objetivo, ponerse agresivo
    if (objetivo) {
        agresivo = true;
    }
}

Entidad* Enemigo::getObjetivo() const {
    return objetivo;
}

// ========== GETTERS ==========

float Enemigo::getSalud() const {
    return salud;
}

float Enemigo::getSaludMaxima() const {
    return saludMaxima;
}

float Enemigo::getDanio() const {
    return danio;
}

bool Enemigo::estaAgresivo() const {
    return agresivo;
}

float Enemigo::getRangoDeteccion() const {
    return rangoDeteccion;
}

float Enemigo::getRangoAtaque() const {
    return rangoAtaque;
}

// ========== SETTERS ==========

void Enemigo::setSalud(float s) {
    salud = s;
    if (salud > saludMaxima) salud = saludMaxima;
    if (salud < 0.0f) salud = 0.0f;
}

void Enemigo::setAgresivo(bool agr) {
    agresivo = agr;
}
