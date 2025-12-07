#include "vortice.h"
#include <QColor>
#include <cmath>
#include <cstdlib>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ========== CONSTRUCTORES ==========

Vortice::Vortice()
    : Entidad(Vector2D(0, 0), TipoEntidad::VORTICE),
    fisicaVortice(nullptr),
    radioActual(150.0f),
    radioInicial(150.0f),
    tiempoVida(8.0f),
    tiempoActual(0.0f),
    rotacionActual(0.0f),
    rangoAtraccion(300.0f) {

    setDimensiones(radioInicial * 2, radioInicial * 2);
    fisicaVortice = new FisicaVortice(posicion, radioInicial, 0.15f);
    inicializarParticulas();
}


Vortice::Vortice(const Vector2D& centro)
    : Entidad(centro, TipoEntidad::VORTICE),
    fisicaVortice(nullptr),
    radioActual(150.0f),
    radioInicial(150.0f),
    tiempoVida(8.0f),
    tiempoActual(0.0f),
    rotacionActual(0.0f),
    rangoAtraccion(300.0f) {

    setDimensiones(radioInicial * 2, radioInicial * 2);
    fisicaVortice = new FisicaVortice(centro, radioInicial, 0.15f);
    inicializarParticulas();
}

Vortice::Vortice(const Vector2D& centro, float radio, float vida)
    : Entidad(centro, TipoEntidad::VORTICE),
    fisicaVortice(nullptr),
    radioActual(radio),
    radioInicial(radio),
    tiempoVida(vida),
    tiempoActual(0.0f),
    rotacionActual(0.0f),
    rangoAtraccion(radio * 2.0f) {

    setDimensiones(radio * 2, radio * 2);
    fisicaVortice = new FisicaVortice(centro, radio, 0.15f);
    inicializarParticulas();
}

Vortice::~Vortice() {
    fisicaVortice = nullptr;
}

// ========== INICIALIZACION ==========

void Vortice::inicializarParticulas() {
    // Crear 20 particulas distribuidas en espiral
    for (int i = 0; i < 20; i++) {
        particulas[i].angulo = (i * 18.0f) * M_PI / 180.0f; // 18° entre cada una
        particulas[i].distancia = (i / 20.0f) * radioInicial; // Distribuidas desde centro
        particulas[i].velocidadAngular = 2.0f + (rand() % 100) / 100.0f; // Velocidad variable
    }
}

// ========== METODOS HEREDADOS ==========

void Vortice::actualizar(float dt) {
    if (!activo) return;

    // Actualizar tiempo
    tiempoActual += dt;

    // Actualizar fisica del vortice (decaimiento del radio)
    if (fisicaVortice) {
        fisicaVortice->actualizar(dt);
        radioActual = fisicaVortice->getRadioActual();
    }

    // Actualizar rotacion visual
    rotacionActual += 2.0f * M_PI / 3.0f * dt; // ω = 2π/3
    if (rotacionActual > 2.0f * M_PI) {
        rotacionActual -= 2.0f * M_PI;
    }

    // Actualizar particulas
    for (int i = 0; i < 20; i++) {
        particulas[i].angulo += particulas[i].velocidadAngular * dt;
        if (particulas[i].angulo > 2.0f * M_PI) {
            particulas[i].angulo -= 2.0f * M_PI;
        }

        // Las particulas se acercan al centro con el tiempo
        particulas[i].distancia = (i / 20.0f) * radioActual;
    }

    // Desactivar si el tiempo de vida se acabo
    if (tiempoActual >= tiempoVida || radioActual < 10.0f) {
        destruir();
    }
}

void Vortice::renderizar(QPainter& painter) {
    if (!activo) return;

    painter.save();

    // Centro del vortice
    float centroX = posicion.x;
    float centroY = posicion.y;

    // Dibujar circulos concentricos (efecto de profundidad)
    int numCirculos = 5;
    for (int i = 0; i < numCirculos; i++) {
        float radioCirculo = radioActual * (1.0f - i / (float)numCirculos);
        int alpha = 255 - (i * 40); // Mas transparente hacia el centro

        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 50, 100, alpha));
        painter.drawEllipse(
            centroX - radioCirculo,
            centroY - radioCirculo,
            radioCirculo * 2,
            radioCirculo * 2
            );
    }

    // Dibujar espiral con particulas
    painter.setPen(QPen(QColor(100, 200, 255, 200), 2));

    for (int i = 0; i < 20; i++) {
        // Calcular posicion de la particula
        float angulo = particulas[i].angulo + rotacionActual;
        float dist = particulas[i].distancia;

        float x = centroX + dist * std::cos(angulo);
        float y = centroY + dist * std::sin(angulo);

        // Dibujar particula
        painter.setBrush(QColor(150, 220, 255, 180));
        painter.drawEllipse(x - 3, y - 3, 6, 6);

        // Conectar con la siguiente particula (espiral continua)
        if (i < 19) {
            float siguienteAngulo = particulas[i + 1].angulo + rotacionActual;
            float siguienteDist = particulas[i + 1].distancia;
            float siguienteX = centroX + siguienteDist * std::cos(siguienteAngulo);
            float siguienteY = centroY + siguienteDist * std::sin(siguienteAngulo);

            painter.setPen(QPen(QColor(100, 200, 255, 100), 1));
            painter.drawLine(x, y, siguienteX, siguienteY);
        }
    }

    // Dibujar centro oscuro (ojo del vortice)
    painter.setBrush(QColor(0, 10, 30, 200));
    painter.drawEllipse(
        centroX - 10, centroY - 10,
        20, 20
        );

    // Indicador de tiempo restante (barra circular)
    float porcentajeVida = 1.0f - (tiempoActual / tiempoVida);
    painter.setPen(QPen(QColor(255, 100, 100), 3));
    painter.setBrush(Qt::NoBrush);
    painter.drawArc(
        centroX - radioActual - 10,
        centroY - radioActual - 10,
        (radioActual + 10) * 2,
        (radioActual + 10) * 2,
        90 * 16,  // Empezar arriba
        -360 * 16 * porcentajeVida  // Decrecer
        );

    painter.restore();
}

void Vortice::onColision(Entidad* otra) {
    // El vortice no colisiona directamente
    // Su efecto se aplica a traves de aplicarFuerzaA()
    (void)otra;
}

// ========== METODOS ESPECIFICOS ==========

void Vortice::aplicarFuerzaA(Entidad* e) {
    if (!e || !activo || !fisicaVortice) return;

    // Calcular distancia a la entidad
    Vector2D posicionEntidad = e->getPosicion();
    float distancia = posicion.distanciaA(posicionEntidad);

    // Si esta fuera del radio, no aplicar fuerza
    if (distancia >= radioActual) return;

    // Aplicar la fisica del vortice a la entidad
    fisicaVortice->aplicar(e);
}

float Vortice::getRadioActual() const {
    return radioActual;
}

float Vortice::getTiempoRestante() const {
    return tiempoVida - tiempoActual;
}

bool Vortice::haExpirado() const {
    return tiempoActual >= tiempoVida || !activo;
}

// ========== GETTERS ==========

Vector2D Vortice::getCentro() const {
    return posicion;
}

float Vortice::getFuerzaEn(const Vector2D& punto) const {
    if (!fisicaVortice) return 0.0f;

    float distancia = posicion.distanciaA(punto);

    if (distancia >= radioActual) return 0.0f;

    // Calcular fuerza usando la formula del vortice
    float distanciaNormalizada = distancia / radioActual;
    float factor = 1.0f - distanciaNormalizada;

    return 300.0f * factor * factor; // F_max = 300
}
