#include "nivel.h"

// ========== CONSTRUCTOR ==========

Nivel::Nivel()
    : motorFisica(nullptr),
    camara(nullptr),
    jugador(nullptr),
    completado(false),
    fallado(false),
    tiempoTranscurrido(0.0f),
    tiempoLimite(0.0f),
    puntuacion(0),
    objetivosPrincipales(0),
    objetivosSecundarios(0),
    anchoNivel(2000),
    altoNivel(1200) {

    // Crear motor de fisica
    motorFisica = new MotorFisica();

    // Crear camara (800x600 por defecto)
    camara = new Camara(800, 600);
    camara->setLimites(Vector2D(0, 0), Vector2D(anchoNivel, altoNivel));
}

Nivel::~Nivel() {
    limpiar();
}

// ========== METODOS VIRTUALES ==========

void Nivel::manejarInput(int tecla, bool presionada) {
    // Implementacion base vacia
    // Las clases derivadas pueden sobrescribir
    (void)tecla;
    (void)presionada;
}

void Nivel::limpiar() {
    // Limpiar motor de fisica (esto elimina todas las entidades)
    if (motorFisica) {
        delete motorFisica;
        motorFisica = nullptr;
    }

    // Limpiar camara
    if (camara) {
        delete camara;
        camara = nullptr;
    }

    jugador = nullptr;
}

// ========== GETTERS ==========

MotorFisica* Nivel::getMotorFisica() const {
    return motorFisica;
}

Camara* Nivel::getCamara() const {
    return camara;
}

Jugador* Nivel::getJugador() const {
    return jugador;
}

bool Nivel::estaCompletado() const {
    return completado;
}

bool Nivel::haFallado() const {
    return fallado;
}

float Nivel::getTiempoTranscurrido() const {
    return tiempoTranscurrido;
}

float Nivel::getTiempoLimite() const {
    return tiempoLimite;
}

int Nivel::getPuntuacion() const {
    return puntuacion;
}

int Nivel::getAnchoNivel() const {
    return anchoNivel;
}

int Nivel::getAltoNivel() const {
    return altoNivel;
}

// ========== SETTERS ==========

void Nivel::setCompletado(bool comp) {
    completado = comp;
}

void Nivel::setFallado(bool fall) {
    fallado = fall;
}

void Nivel::setPuntuacion(int puntos) {
    puntuacion = puntos;
}

void Nivel::agregarPuntos(int puntos) {
    puntuacion += puntos;
}
