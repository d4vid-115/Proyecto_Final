#include "torpedo.h"
#include <cmath>
#include <QColor>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ========== CONSTRUCTORES ==========

Torpedo::Torpedo()
    : Entidad(Vector2D(0, 0), TipoEntidad::TORPEDO),
    anguloDisparo(0.0f),
    velocidadInicial(200.0f),
    tiempoVida(6.0f),
    tiempoActual(0.0f),
    trayectoria(nullptr) {

    setDimensiones(24, 8);

    trayectoria = new TrayectoriaParabolica(0, 0, 50.0f);
    setFisica(trayectoria);
}

Torpedo::Torpedo(const Vector2D& pos, float angulo, float velocidad)
    : Entidad(pos, TipoEntidad::TORPEDO),
    anguloDisparo(angulo),
    velocidadInicial(velocidad),
    tiempoVida(6.0f),
    tiempoActual(0.0f),
    trayectoria(nullptr) {

    setDimensiones(24, 8);

    configurarTrayectoria(pos, angulo, velocidad);
}

Torpedo::~Torpedo() {
    trayectoria = nullptr;
}

// ========== METODOS HEREDADOS ==========

void Torpedo::actualizar(float dt) {
    if (!activo) return;

    tiempoActual += dt;

    if (tiempoActual >= tiempoVida) {
        destruir();
        return;
    }

    if (trayectoria) {
        trayectoria->aplicar(this);
    }

    actualizarColision();
}

void Torpedo::renderizar(QPainter& painter) {
    if (!activo) return;

    // Calcular angulo de rotacion basado en velocidad
    float anguloRender = std::atan2(velocidad.y, velocidad.x);

    painter.save();

    // Trasladar al centro del torpedo
    painter.translate(posicion.x + ancho/2, posicion.y + alto/2);
    painter.rotate(anguloRender * 180.0f / M_PI);

    // ===== DIBUJO MANUAL DEL TORPEDO =====

    // Cuerpo principal (cilindro rojo)
    painter.setBrush(QColor(180, 50, 50));
    painter.setPen(QPen(Qt::black, 2));
    painter.drawRect(-ancho/2, -alto/2, ancho - 6, alto);

    // Punta conica (ojiva)
    QPolygon punta;
    punta << QPoint(ancho/2 - 6, -alto/2)
          << QPoint(ancho/2 + 6, 0)
          << QPoint(ancho/2 - 6, alto/2);
    painter.setBrush(QColor(150, 30, 30));
    painter.drawPolygon(punta);

    // Aletas estabilizadoras (4 aletas)
    painter.setBrush(QColor(100, 30, 30));

    // Aleta superior
    QPolygon aletaSuperior;
    aletaSuperior << QPoint(-ancho/2 + 8, -alto/2)
                  << QPoint(-ancho/2 + 2, -alto/2 - 6)
                  << QPoint(-ancho/2 + 14, -alto/2);
    painter.drawPolygon(aletaSuperior);

    // Aleta inferior
    QPolygon aletaInferior;
    aletaInferior << QPoint(-ancho/2 + 8, alto/2)
                  << QPoint(-ancho/2 + 2, alto/2 + 6)
                  << QPoint(-ancho/2 + 14, alto/2);
    painter.drawPolygon(aletaInferior);

    // Helice en la cola
    painter.setBrush(QColor(120, 120, 120));
    painter.drawEllipse(-ancho/2 - 3, -4, 6, 8);

    // Detalles: lineas de union
    painter.setPen(QPen(QColor(100, 20, 20), 1));
    painter.drawLine(-ancho/2 + 4, 0, ancho/2 - 6, 0);

    painter.restore();

    // Estela de burbujas (trail effect)
    painter.setPen(Qt::NoPen);
    for (int i = 0; i < 5; i++) {
        float offsetX = -velocidad.x * 0.01f * i;
        float offsetY = -velocidad.y * 0.01f * i;
        int alpha = 200 - (i * 40);

        painter.setBrush(QColor(255, 255, 255, alpha));
        painter.drawEllipse(
            posicion.x + offsetX - 2,
            posicion.y + offsetY - 2,
            4, 4
            );
    }
}

void Torpedo::onColision(Entidad* otra) {
    if (!otra) return;

    if (otra->getTipo() != TipoEntidad::ENEMIGO_SUBMARINO) {
        destruir();
    }
}

// ========== CONFIGURACION ==========

void Torpedo::configurarTrayectoria(const Vector2D& posInicial, float angulo, float vel) {
    anguloDisparo = angulo;
    velocidadInicial = vel;

    float vx = velocidadInicial * std::cos(angulo);
    float vy = velocidadInicial * std::sin(angulo);

    trayectoria = new TrayectoriaParabolica(vx, vy, 50.0f);
    trayectoria->setPosicionInicial(posInicial);
    trayectoria->reiniciarTiempo();

    setFisica(trayectoria);
    setPosicion(posInicial);
    setVelocidad(Vector2D(vx, vy));
}

// ========== GETTERS ==========

float Torpedo::getAnguloDisparo() const {
    return anguloDisparo;
}

float Torpedo::getTiempoRestante() const {
    return tiempoVida - tiempoActual;
}
