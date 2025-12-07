#include "submarino.h"
#include "torpedo.h"
#include <cmath>
#include <cstdlib>
#include <QColor>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ========== CONSTRUCTORES ==========

Submarino::Submarino()
    : Enemigo(Vector2D(100, 100), TipoEntidad::ENEMIGO_SUBMARINO),
    estado(EstadoSubmarino::PATRULLANDO),
    agenteIA(new AgenteIA(this, nullptr)),
    puntoPatrulla(200, 100),
    tiempoEnPunto(0.0f),
    tiempoEsperaPunto(2.0f),
    tiempoUltimoDisparo(0.0f),
    cooldownDisparo(1.5f),
    torpedosDisparados(0),
    maxTorpedos(5),
    profundidad(100.0f),
    moviendose(true) {

    setDimensiones(64, 32);
    salud = 150.0f;
    saludMaxima = 150.0f;
    velocidadBase = 60.0f;
    rangoDeteccion = 300.0f;
    rangoAtaque = 250.0f;

    elegirNuevoPuntoPatrulla();
}

Submarino::Submarino(const Vector2D& pos)
    : Enemigo(pos, TipoEntidad::ENEMIGO_SUBMARINO),
    estado(EstadoSubmarino::PATRULLANDO),
    agenteIA(new AgenteIA(this, nullptr)),
    puntoPatrulla(pos.x + 200, pos.y),
    tiempoEnPunto(0.0f),
    tiempoEsperaPunto(2.0f),
    tiempoUltimoDisparo(0.0f),
    cooldownDisparo(1.5f),
    torpedosDisparados(0),
    maxTorpedos(5),
    profundidad(100.0f),
    moviendose(true) {

    setDimensiones(64, 32);
    salud = 150.0f;
    saludMaxima = 150.0f;
    velocidadBase = 60.0f;
    rangoDeteccion = 300.0f;
    rangoAtaque = 250.0f;
}

Submarino::~Submarino() {
    if (agenteIA) {
        delete agenteIA;
        agenteIA = nullptr;
    }
}

// ========== METODOS HEREDADOS ==========

void Submarino::actualizar(float dt) {
    if (!activo) return;

    if (tiempoUltimoDisparo > 0.0f) {
        tiempoUltimoDisparo -= dt;
    }

    if (agenteIA && objetivo) {
        agenteIA->setObjetivo(objetivo);
        agenteIA->actualizar(dt);

        EstadoIA estadoIA = agenteIA->getEstadoActual();

        switch(estadoIA) {
        case EstadoIA::PATRULLANDO:
            estado = EstadoSubmarino::PATRULLANDO;
            break;
        case EstadoIA::ACERCANDOSE:
            estado = EstadoSubmarino::DETECTANDO;
            break;
        case EstadoIA::ATACANDO:
            estado = EstadoSubmarino::ATACANDO;
            break;
        case EstadoIA::EVADIENDO:
            estado = EstadoSubmarino::EVADIENDO;
            break;
        }
    } else {
        actualizarIABasica(dt);
    }

    posicion += velocidad * dt;
    actualizarColision();
}

void Submarino::renderizar(QPainter& painter) {
    if (!activo) return;

    painter.save();

    // Calcular angulo de orientacion
    float angulo = 0.0f;
    if (velocidad.magnitud() > 0.1f) {
        angulo = std::atan2(velocidad.y, velocidad.x) * 180.0f / M_PI;
    }

    // Trasladar al centro del submarino
    painter.translate(posicion.x + ancho/2, posicion.y + alto/2);
    painter.rotate(angulo);

    // ===== DIBUJO MANUAL DEL SUBMARINO =====

    // Casco principal (elipse gris oscuro)
    painter.setBrush(QColor(50, 50, 50));
    painter.setPen(QPen(Qt::black, 2));
    painter.drawEllipse(-ancho/2, -alto/2, ancho, alto);

    // Torre de mando (conning tower)
    painter.setBrush(QColor(40, 40, 40));
    painter.drawRect(-12, -alto/2 - 10, 24, 10);

    // Periscopio
    painter.setPen(QPen(QColor(60, 60, 60), 2));
    painter.drawLine(0, -alto/2 - 10, 0, -alto/2 - 18);

    // Antena
    painter.drawLine(-8, -alto/2 - 10, -8, -alto/2 - 15);

    // Helice (en la parte trasera)
    painter.setBrush(QColor(100, 100, 100));
    painter.drawEllipse(-ancho/2 - 6, -6, 8, 12);

    // Timones
    painter.setBrush(QColor(70, 70, 70));
    QPolygon timonSuperior;
    timonSuperior << QPoint(-ancho/2 + 5, -alto/2)
                  << QPoint(-ancho/2 - 5, -alto/2 - 8)
                  << QPoint(-ancho/2 + 10, -alto/2);
    painter.drawPolygon(timonSuperior);

    QPolygon timonInferior;
    timonInferior << QPoint(-ancho/2 + 5, alto/2)
                  << QPoint(-ancho/2 - 5, alto/2 + 8)
                  << QPoint(-ancho/2 + 10, alto/2);
    painter.drawPolygon(timonInferior);

    // Ventanas/portillas (3 circulos pequeños)
    painter.setBrush(QColor(100, 150, 200, 150));
    painter.setPen(QPen(QColor(30, 30, 30), 1));
    painter.drawEllipse(10, -4, 6, 6);
    painter.drawEllipse(20, -4, 6, 6);
    painter.drawEllipse(30, -4, 6, 6);

    // Indicador de estado (luz de navegación)
    QColor colorEstado;
    switch (estado) {
    case EstadoSubmarino::PATRULLANDO:
        colorEstado = QColor(0, 255, 0);
        break;
    case EstadoSubmarino::DETECTANDO:
        colorEstado = QColor(255, 255, 0);
        break;
    case EstadoSubmarino::ATACANDO:
        colorEstado = QColor(255, 0, 0);
        break;
    case EstadoSubmarino::EVADIENDO:
        colorEstado = QColor(0, 100, 255);
        break;
    }

    painter.setBrush(colorEstado);
    painter.setPen(Qt::black);
    painter.drawEllipse(ancho/2 - 10, -5, 8, 8);

    painter.restore();

    // Barra de vida
    float porcentajeSalud = salud / saludMaxima;

    painter.setBrush(QColor(255, 0, 0));
    painter.setPen(Qt::NoPen);
    painter.drawRect(posicion.x, posicion.y - 10, ancho, 5);

    painter.setBrush(QColor(0, 255, 0));
    painter.drawRect(posicion.x, posicion.y - 10, ancho * porcentajeSalud, 5);
}

void Submarino::patrullar(float dt) {
    Vector2D direccion = puntoPatrulla - posicion;
    float distancia = direccion.magnitud();

    if (distancia < 20.0f) {
        tiempoEnPunto += dt;
        velocidad = Vector2D(0, 0);
        moviendose = false;

        if (tiempoEnPunto >= tiempoEsperaPunto) {
            elegirNuevoPuntoPatrulla();
            tiempoEnPunto = 0.0f;
            moviendose = true;
        }
    } else {
        velocidad = direccion.normalizado() * velocidadBase;
        moviendose = true;
    }
}

void Submarino::perseguir(Entidad* objetivo, float dt) {
    if (!objetivo) return;

    Vector2D direccion = objetivo->getPosicion() - posicion;
    float distancia = direccion.magnitud();

    if (distancia > rangoAtaque * 0.8f) {
        velocidad = direccion.normalizado() * velocidadBase;
    } else if (distancia < rangoAtaque * 0.5f) {
        velocidad = direccion.normalizado() * (-velocidadBase * 0.5f);
        estado = EstadoSubmarino::EVADIENDO;
    } else {
        velocidad = Vector2D(0, 0);
    }
}

void Submarino::atacar(Entidad* objetivo, float dt) {
    if (!objetivo || !puedeDisparar()) return;

    Vector2D direccion = objetivo->getPosicion() - posicion;
    float distancia = direccion.magnitud();

    if (distancia > rangoAtaque) {
        estado = EstadoSubmarino::DETECTANDO;
        perseguir(objetivo, dt);
    } else {
        velocidad *= 0.95f;
    }
}

// ========== IA BASICA ==========

void Submarino::actualizarIABasica(float dt) {
    if (objetivo && puedeDetectar(objetivo)) {
        float distancia = posicion.distanciaA(objetivo->getPosicion());

        if (distancia < 100.0f) {
            estado = EstadoSubmarino::EVADIENDO;
            Vector2D direccion = posicion - objetivo->getPosicion();
            velocidad = direccion.normalizado() * velocidadBase;
        } else if (distancia < rangoAtaque) {
            estado = EstadoSubmarino::ATACANDO;
            atacar(objetivo, dt);
        } else {
            estado = EstadoSubmarino::DETECTANDO;
            perseguir(objetivo, dt);
        }
    } else {
        estado = EstadoSubmarino::PATRULLANDO;
        patrullar(dt);
    }
}

void Submarino::elegirNuevoPuntoPatrulla() {
    float x = posicion.x + (rand() % 400) - 200;
    float y = profundidad + (rand() % 60) - 30;

    puntoPatrulla = Vector2D(x, y);
}

float Submarino::calcularAnguloInterceptacion(const Vector2D& objetivo) {
    Vector2D direccion = objetivo - posicion;
    return std::atan2(direccion.y, direccion.x);
}

// ========== METODOS ESPECIFICOS ==========

Torpedo* Submarino::dispararTorpedo() {
    if (!puedeDisparar() || !objetivo) return nullptr;

    float angulo = calcularAnguloInterceptacion(objetivo->getPosicion());

    Vector2D posicionDisparo = posicion + Vector2D(ancho, alto/2);
    Torpedo* torpedo = new Torpedo(posicionDisparo, angulo, 200.0f);

    tiempoUltimoDisparo = cooldownDisparo;
    torpedosDisparados++;

    return torpedo;
}

void Submarino::cambiarEstado(EstadoSubmarino nuevoEstado) {
    estado = nuevoEstado;
}

bool Submarino::puedeDisparar() const {
    return tiempoUltimoDisparo <= 0.0f &&
           torpedosDisparados < maxTorpedos &&
           activo;
}

// ========== GETTERS ==========

EstadoSubmarino Submarino::getEstado() const {
    return estado;
}

float Submarino::getProfundidad() const {
    return profundidad;
}

int Submarino::getTorpedosDisparados() const {
    return torpedosDisparados;
}

// ========== SETTERS ==========

void Submarino::setEstado(EstadoSubmarino est) {
    estado = est;
}

void Submarino::setProfundidad(float prof) {
    profundidad = prof;
}
