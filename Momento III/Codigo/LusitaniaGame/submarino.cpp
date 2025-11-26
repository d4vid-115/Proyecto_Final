#include "submarino.h"
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
    agenteIA(nullptr),
    puntoPatrulla(200, 100),
    tiempoEnPunto(0.0f),
    tiempoEsperaPunto(2.0f),
    tiempoUltimoDisparo(0.0f),
    cooldownDisparo(3.0f),
    torpedosDisparados(0),
    maxTorpedos(5),
    profundidad(100.0f),
    moviendose(true) {

    // Configurar submarino
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
    agenteIA(nullptr),
    puntoPatrulla(pos.x + 200, pos.y),
    tiempoEnPunto(0.0f),
    tiempoEsperaPunto(2.0f),
    tiempoUltimoDisparo(0.0f),
    cooldownDisparo(3.0f),
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

    // Actualizar cooldown de disparo
    if (tiempoUltimoDisparo > 0.0f) {
        tiempoUltimoDisparo -= dt;
    }

    // Usar IA basica por ahora (luego sera AgenteIA)
    actualizarIABasica(dt);

    // Actualizar posicion
    posicion += velocidad * dt;
    actualizarColision();
}

void Submarino::renderizar(QPainter& painter) {
    if (!activo) return;

    painter.save();

    // Calcular angulo de orientacion
    float angulo = 0.0f;
    if (velocidad.magnitud() > 0.1f) {
        angulo = std::atan2(velocidad.y, velocidad.x);
    }

    // Trasladar y rotar
    painter.translate(posicion.x + ancho/2, posicion.y + alto/2);
    painter.rotate(angulo * 180.0f / M_PI);

    // Cuerpo del submarino (gris oscuro)
    painter.setBrush(QColor(50, 50, 50));
    painter.setPen(Qt::black);
    painter.drawEllipse(-ancho/2, -alto/2, ancho, alto);

    // Torre de mando
    painter.setBrush(QColor(40, 40, 40));
    painter.drawRect(-10, -alto/2 - 8, 20, 8);

    // Periscopio (si esta detectando)
    if (estado == EstadoSubmarino::DETECTANDO ||
        estado == EstadoSubmarino::ATACANDO) {
        painter.setPen(QPen(QColor(100, 100, 100), 2));
        painter.drawLine(0, -alto/2 - 8, 0, -alto/2 - 20);
    }

    // Indicador de estado (luz)
    QColor colorEstado;
    switch (estado) {
    case EstadoSubmarino::PATRULLANDO:
        colorEstado = QColor(0, 255, 0);   // Verde
        break;
    case EstadoSubmarino::DETECTANDO:
        colorEstado = QColor(255, 255, 0); // Amarillo
        break;
    case EstadoSubmarino::ATACANDO:
        colorEstado = QColor(255, 0, 0);   // Rojo
        break;
    case EstadoSubmarino::EVADIENDO:
        colorEstado = QColor(0, 100, 255); // Azul
        break;
    }
    painter.setBrush(colorEstado);
    painter.drawEllipse(ancho/2 - 8, -4, 8, 8);

    painter.restore();

    // Barra de vida
    float porcentajeSalud = salud / saludMaxima;
    painter.setBrush(QColor(255, 0, 0));
    painter.drawRect(posicion.x, posicion.y - 10, ancho, 5);
    painter.setBrush(QColor(0, 255, 0));
    painter.drawRect(posicion.x, posicion.y - 10, ancho * porcentajeSalud, 5);
}

void Submarino::patrullar(float dt) {
    // Moverse hacia el punto de patrulla
    Vector2D direccion = puntoPatrulla - posicion;
    float distancia = direccion.magnitud();

    if (distancia < 20.0f) {
        // Llego al punto, esperar
        tiempoEnPunto += dt;
        velocidad = Vector2D(0, 0);
        moviendose = false;

        if (tiempoEnPunto >= tiempoEsperaPunto) {
            // Elegir nuevo punto
            elegirNuevoPuntoPatrulla();
            tiempoEnPunto = 0.0f;
            moviendose = true;
        }
    } else {
        // Moverse hacia el punto
        velocidad = direccion.normalizado() * velocidadBase;
        moviendose = true;
    }
}

void Submarino::perseguir(Entidad* objetivo, float dt) {
    if (!objetivo) return;

    // Calcular direccion hacia el objetivo
    Vector2D direccion = objetivo->getPosicion() - posicion;
    float distancia = direccion.magnitud();

    // Mantener distancia de ataque (no acercarse demasiado)
    if (distancia > rangoAtaque * 0.8f) {
        // Acercarse
        velocidad = direccion.normalizado() * velocidadBase;
    } else if (distancia < rangoAtaque * 0.5f) {
        // Alejarse (evadir)
        velocidad = direccion.normalizado() * (-velocidadBase * 0.5f);
        estado = EstadoSubmarino::EVADIENDO;
    } else {
        // Mantener distancia
        velocidad = Vector2D(0, 0);
    }
}

void Submarino::atacar(Entidad* objetivo, float dt) {
    if (!objetivo || !puedeDisparar()) return;

    // El disparo se maneja externamente en el nivel
    // Aqui solo actualizamos el estado

    // Mantener posicion de ataque
    Vector2D direccion = objetivo->getPosicion() - posicion;
    float distancia = direccion.magnitud();

    if (distancia > rangoAtaque) {
        // Objetivo fuera de rango, perseguir
        estado = EstadoSubmarino::DETECTANDO;
        perseguir(objetivo, dt);
    } else {
        // Mantener posicion
        velocidad *= 0.95f; // Frenar gradualmente
    }
}

// ========== IA BASICA (TEMPORAL) ==========

void Submarino::actualizarIABasica(float dt) {
    // Comportamiento basico hasta implementar AgenteIA completo

    // Verificar si hay objetivo
    if (objetivo && puedeDetectar(objetivo)) {
        // Tiene objetivo en rango
        float distancia = posicion.distanciaA(objetivo->getPosicion());

        if (distancia < 100.0f) {
            // Muy cerca - evadir
            estado = EstadoSubmarino::EVADIENDO;
            Vector2D direccion = posicion - objetivo->getPosicion();
            velocidad = direccion.normalizado() * velocidadBase;
        } else if (distancia < rangoAtaque) {
            // En rango de ataque
            estado = EstadoSubmarino::ATACANDO;
            atacar(objetivo, dt);
        } else {
            // Detectado pero lejos - perseguir
            estado = EstadoSubmarino::DETECTANDO;
            perseguir(objetivo, dt);
        }
    } else {
        // Sin objetivo - patrullar
        estado = EstadoSubmarino::PATRULLANDO;
        patrullar(dt);
    }
}

void Submarino::elegirNuevoPuntoPatrulla() {
    // Elegir punto aleatorio en un area
    float x = posicion.x + (rand() % 400) - 200; // ±200 px
    float y = profundidad + (rand() % 60) - 30;  // ±30 px de profundidad

    puntoPatrulla = Vector2D(x, y);
}

float Submarino::calcularAnguloInterceptacion(const Vector2D& objetivo) {
    // Calculo simple de angulo hacia el objetivo
    // (mas adelante el AgenteIA hara prediccion avanzada)
    Vector2D direccion = objetivo - posicion;
    return std::atan2(direccion.y, direccion.x);
}

// ========== METODOS ESPECIFICOS ==========

Torpedo* Submarino::dispararTorpedo() {
    if (!puedeDisparar() || !objetivo) return nullptr;

    // Calcular angulo de disparo
    float angulo = calcularAnguloInterceptacion(objetivo->getPosicion());

    // Crear torpedo
    Vector2D posicionDisparo = posicion + Vector2D(ancho, alto/2);
    Torpedo* torpedo = new Torpedo(posicionDisparo, angulo, 200.0f);

    // Actualizar estado
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
