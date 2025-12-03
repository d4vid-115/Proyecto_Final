#include "motorjuego.h"
#include "nivel.h"
#include "nivel1oceano.h"
#include "nivel2barco.h"
#include "nivel3submarino.h"

// ===== INICIALIZACION DEL SINGLETON =====
MotorJuego* MotorJuego::instancia = nullptr;

// ===== CONSTRUCTOR PRIVADO =====
MotorJuego::MotorJuego()
    : estadoActual(EstadoJuego::MENU_PRINCIPAL),
    estadoAnterior(EstadoJuego::MENU_PRINCIPAL),
    nivelActual(nullptr),
    numeroNivelActual(0),
    tiempoTotal(0.0f),
    tiempoPausado(0.0f),
    pausado(false),
    puntuacionTotal(0),
    vidasRestantes(3) {
    // Constructor privado del singleton
}

// ===== SINGLETON - OBTENER INSTANCIA =====

MotorJuego* MotorJuego::obtenerInstancia() {
    // Lazy initialization: crear solo cuando se necesita
    if (!instancia) {
        instancia = new MotorJuego();
    }
    return instancia;
}

void MotorJuego::destruirInstancia() {
    if (instancia) {
        delete instancia;
        instancia = nullptr;
    }
}

// ===== DESTRUCTOR =====

MotorJuego::~MotorJuego() {
    if (nivelActual) {
        delete nivelActual;
        nivelActual = nullptr;
    }
}

// ===== CICLO PRINCIPAL =====

void MotorJuego::actualizar(float dt) {
    // Si esta pausado, no actualizar
    if (pausado) {
        tiempoPausado += dt;
        return;
    }

    // Actualizar tiempo total
    tiempoTotal += dt;

    // Actualizar segun el estado
    switch (estadoActual) {
    case EstadoJuego::MENU_PRINCIPAL:
        break;

    case EstadoJuego::JUGANDO:
        // Actualizar nivel actual
        if (nivelActual) {
            nivelActual->actualizar(dt);

            // Verificar condiciones de victoria/derrota del nivel
        }
        break;

    case EstadoJuego::PAUSADO:
        // No hacer nada
        break;

    case EstadoJuego::GAME_OVER:
        // Pantalla de game over
        break;

    case EstadoJuego::VICTORIA:
        // Pantalla de victoria
        break;

    case EstadoJuego::TRANSICION_NIVEL:
        // Transicion entre niveles
    }
}

void MotorJuego::renderizar() {
    if (nivelActual) {
        // nivelActual->renderizar(painter);
    }
}

// ===== GESTION DE NIVELES =====

void MotorJuego::cargarNivel(int numeroNivel) {
    // Limpiar nivel anterior
    if (nivelActual) {
        delete nivelActual;
        nivelActual = nullptr;
    }

    // Crear nuevo nivel segun el numero
    numeroNivelActual = numeroNivel;

    // Cambiar estado a jugando
    cambiarEstado(EstadoJuego::JUGANDO);
}

void MotorJuego::siguienteNivel() {
    int siguienteNumero = numeroNivelActual + 1;

    if (siguienteNumero > 3) {
        // Completo todos los niveles - Victoria
        victoria();
    } else {
        // Cargar siguiente nivel
        cambiarEstado(EstadoJuego::TRANSICION_NIVEL);
        cargarNivel(siguienteNumero);
    }
}

void MotorJuego::reiniciarNivel() {
    // Recargar el nivel actual
    cargarNivel(numeroNivelActual);
}

void MotorJuego::volverAlMenu() {
    // Limpiar nivel
    if (nivelActual) {
        delete nivelActual;
        nivelActual = nullptr;
    }

    numeroNivelActual = 0;
    cambiarEstado(EstadoJuego::MENU_PRINCIPAL);
}

// ===== GESTION DE ESTADOS =====

void MotorJuego::cambiarEstado(EstadoJuego nuevoEstado) {
    estadoAnterior = estadoActual;
    estadoActual = nuevoEstado;

    // Acciones al entrar en ciertos estados
    switch (nuevoEstado) {
    case EstadoJuego::PAUSADO:
        pausado = true;
        break;

    case EstadoJuego::JUGANDO:
        pausado = false;
        break;

    case EstadoJuego::GAME_OVER:
        pausado = true;
        // Aqui se reproduce sonido de game over
        break;

    case EstadoJuego::VICTORIA:
        pausado = true;
        // Aqui se reproduce sonido de victoria
        break;

    default:
        break;
    }
}

void MotorJuego::pausar() {
    if (estadoActual == EstadoJuego::JUGANDO) {
        cambiarEstado(EstadoJuego::PAUSADO);
    }
}

void MotorJuego::reanudar() {
    if (estadoActual == EstadoJuego::PAUSADO) {
        cambiarEstado(EstadoJuego::JUGANDO);
    }
}

void MotorJuego::gameOver() {
    cambiarEstado(EstadoJuego::GAME_OVER);
}

void MotorJuego::victoria() {
    cambiarEstado(EstadoJuego::VICTORIA);
}

// ===== GETTERS =====

EstadoJuego MotorJuego::getEstado() const {
    return estadoActual;
}

EstadoJuego MotorJuego::getEstadoAnterior() const {
    return estadoAnterior;
}

Nivel* MotorJuego::getNivelActual() const {
    return nivelActual;
}

int MotorJuego::getNumeroNivelActual() const {
    return numeroNivelActual;
}

float MotorJuego::getTiempoTotal() const {
    return tiempoTotal;
}

int MotorJuego::getPuntuacionTotal() const {
    return puntuacionTotal;
}

int MotorJuego::getVidasRestantes() const {
    return vidasRestantes;
}

bool MotorJuego::estaPausado() const {
    return pausado;
}

// ===== SETTERS =====

void MotorJuego::setPuntuacionTotal(int puntos) {
    puntuacionTotal = puntos;
}

void MotorJuego::agregarPuntos(int puntos) {
    puntuacionTotal += puntos;
}

void MotorJuego::setVidasRestantes(int vidas) {
    vidasRestantes = vidas;
}
