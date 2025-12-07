#include "nivel1oceano.h"
#include "gestorsprites.h"
#include <QColor>
#include <cstdlib>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ========== CONSTRUCTOR ==========

Nivel1Oceano::Nivel1Oceano()
    : Nivel(),
    posicionBarco(400, 200),
    oscilacionBarco(nullptr),
    tiempoSpawn(0.0f),
    intervaloSpawn(12.0f),
    maxSubmarinos(5),  //
    submarinosDestruidos(0),
    torpedosEvadidos(0) {

    anchoNivel = 1600;
    altoNivel = 800;
    tiempoLimite = 90.0f;

    camara->setLimites(Vector2D(0, 0), Vector2D(anchoNivel, altoNivel));
    camara->setTieneScroll(true);

    inicializar();
}

Nivel1Oceano::~Nivel1Oceano() {
    if (oscilacionBarco) {
        delete oscilacionBarco;
        oscilacionBarco = nullptr;
    }

    submarinos.clear();
    torpedos.clear();
}

// ========== INICIALIZACION ==========

void Nivel1Oceano::inicializar() {
    jugador = new Jugador(Vector2D(400, 500));
    jugador->setDimensiones(48, 32);
    motorFisica->agregarEntidad(jugador);

    oscilacionBarco = new OsciladorArmonico(15.0f, 2.0f * M_PI / 3.0f, 0.0f);
    oscilacionBarco->setPosicionBase(posicionBarco.y);

    // Spawnear 2 submarinos iniciales
    spawnearSubmarino();
    spawnearSubmarino();

    motorFisica->setGravedadActiva(false);
}

// ========== ACTUALIZACION ==========

void Nivel1Oceano::actualizar(float dt) {
    if (completado || fallado) return;

    tiempoTranscurrido += dt;

    if (oscilacionBarco) {
        posicionBarco.y = oscilacionBarco->calcularDesplazamiento(tiempoTranscurrido);
    }

    motorFisica->actualizar(dt);

    actualizarSubmarinos(dt);
    actualizarTorpedos(dt);
    verificarColisionesTorpedos();

    // Spawning mas rapido
    tiempoSpawn += dt;
    if (tiempoSpawn >= intervaloSpawn) {
        if ((int)submarinos.size() < maxSubmarinos) {
            spawnearSubmarino();
        }
        tiempoSpawn = 0.0f;
    }

    if (jugador) {
        camara->seguirObjetivo(jugador->getPosicion(), dt);
    }

    if (verificarVictoria()) {
        completado = true;
    }

    if (verificarDerrota()) {
        fallado = true;
    }
}

void Nivel1Oceano::actualizarSubmarinos(float /*dt*/) {
    auto it = submarinos.begin();

    while (it != submarinos.end()) {
        Submarino* sub = *it;

        if (!sub || !sub->estaActivo()) {
            it = submarinos.erase(it);
            submarinosDestruidos++;
            agregarPuntos(100);
            continue;
        }

        sub->setObjetivo(jugador);

        if (sub->puedeDisparar() && sub->getEstado() == EstadoSubmarino::ATACANDO) {
            Torpedo* torpedo = sub->dispararTorpedo();
            if (torpedo) {
                motorFisica->agregarEntidad(torpedo);
                torpedos.push_back(torpedo);
            }
        }

        ++it;
    }
}

void Nivel1Oceano::actualizarTorpedos(float /*dt*/) {
    auto it = torpedos.begin();

    while (it != torpedos.end()) {
        Torpedo* torpedo = *it;

        if (!torpedo || !torpedo->estaActivo()) {
            it = torpedos.erase(it);
            torpedosEvadidos++;
            agregarPuntos(10);
        } else {
            ++it;
        }
    }
}

void Nivel1Oceano::verificarColisionesTorpedos() {
    for (auto* torpedo : torpedos) {
        if (torpedo && torpedo->estaActivo() && jugador) {
            if (torpedo->colisionaCon(jugador)) {
                jugador->recibirDanio(25.0f);
                torpedo->destruir();
            }
        }
    }
}

void Nivel1Oceano::spawnearSubmarino() {
    Vector2D posCamara = camara->getPosicion();

    float x = (rand() % 2 == 0) ? posCamara.x - 50.0f : posCamara.x + 850.0f;
    float y = posCamara.y + 100.0f + (rand() % 400);

    Submarino* sub = new Submarino(Vector2D(x, y));
    sub->setObjetivo(jugador);
    sub->setProfundidad(y);

    motorFisica->agregarEntidad(sub);
    submarinos.push_back(sub);
}

// ========== RENDERIZADO ==========

void Nivel1Oceano::renderizar(QPainter& painter) {
    if (!camara) return;

    painter.save();

    // Fondo oceano claro
    QLinearGradient gradient(0, 0, 0, 600);
    gradient.setColorAt(0, QColor(50, 150, 220));
    gradient.setColorAt(0.5, QColor(20, 100, 180));
    gradient.setColorAt(1, QColor(0, 50, 120));
    painter.fillRect(0, 0, 800, 600, gradient);

    Vector2D offsetCamara = camara->getPosicion();
    painter.translate(-offsetCamara.x, -offsetCamara.y);

    // Barco Lusitania
    painter.setBrush(QColor(100, 50, 0));
    painter.setPen(QPen(Qt::black, 2));

    painter.drawRect(posicionBarco.x - 100, posicionBarco.y - 40, 200, 80);

    QPolygon proa;
    proa << QPoint(posicionBarco.x + 100, posicionBarco.y - 40)
         << QPoint(posicionBarco.x + 120, posicionBarco.y)
         << QPoint(posicionBarco.x + 100, posicionBarco.y + 40);
    painter.drawPolygon(proa);

    painter.setBrush(QColor(80, 40, 0));
    painter.drawRect(posicionBarco.x - 90, posicionBarco.y - 60, 180, 20);

    painter.setBrush(QColor(50, 25, 0));
    painter.drawRect(posicionBarco.x - 60, posicionBarco.y - 80, 20, 20);
    painter.drawRect(posicionBarco.x - 10, posicionBarco.y - 80, 20, 20);
    painter.drawRect(posicionBarco.x + 40, posicionBarco.y - 80, 20, 20);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(100, 100, 100, 150));
    painter.drawEllipse(posicionBarco.x - 55, posicionBarco.y - 95, 10, 15);
    painter.drawEllipse(posicionBarco.x - 5, posicionBarco.y - 95, 10, 15);
    painter.drawEllipse(posicionBarco.x + 45, posicionBarco.y - 95, 10, 15);

    // Renderizar entidades
    for (auto* entidad : motorFisica->getEntidades()) {
        if (entidad && entidad->estaActivo()) {
            entidad->renderizar(painter);
        }
    }

    painter.restore();
}

// ========== REINICIAR ==========

void Nivel1Oceano::reiniciar() {
    limpiar();

    completado = false;
    fallado = false;
    tiempoTranscurrido = 0.0f;
    puntuacion = 0;
    submarinosDestruidos = 0;
    torpedosEvadidos = 0;
    tiempoSpawn = 0.0f;

    submarinos.clear();
    torpedos.clear();

    motorFisica = new MotorFisica();
    camara = new Camara(800, 600);
    camara->setLimites(Vector2D(0, 0), Vector2D(anchoNivel, altoNivel));

    inicializar();
}

// ========== CONDICIONES ==========

bool Nivel1Oceano::verificarVictoria() {
    return tiempoTranscurrido >= tiempoLimite;
}

bool Nivel1Oceano::verificarDerrota() {
    return jugador && jugador->estaMuerto();
}

void Nivel1Oceano::manejarInput(int /*tecla*/, bool /*presionada*/) {
}
