#include "torpedo.h"
#include "gestorsprites.h"
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

    setDimensiones(16, 8); // Torpedo mas pequeño

    // Crear fisica parabolica
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

    setDimensiones(16, 8);

    // Crear y configurar trayectoria
    configurarTrayectoria(pos, angulo, velocidad);
}

Torpedo::~Torpedo() {
    trayectoria = nullptr;
}

// ========== METODOS HEREDADOS ==========

void Torpedo::actualizar(float dt) {
    if (!activo) return;

    // Incrementar tiempo
    tiempoActual += dt;

    // Desactivar si excede tiempo de vida
    if (tiempoActual >= tiempoVida) {
        destruir();
        return;
    }

    // Aplicar fisica parabolica
    if (trayectoria) {
        trayectoria->aplicar(this);
    }

    actualizarColision();
}

void Torpedo::renderizar(QPainter& painter) {
    if (!activo) return;

    GestorSprites* gestor = GestorSprites::obtenerInstancia();
    QPixmap spriteTorpedo = gestor->getSprite("torpedo");

    // Calcular angulo de rotacion basado en velocidad
    float anguloRender = std::atan2(velocidad.y, velocidad.x);

    painter.save();

    // Trasladar al centro del torpedo
    painter.translate(posicion.x + ancho/2, posicion.y + alto/2);
    painter.rotate(anguloRender * 180.0f / M_PI);

    if (!spriteTorpedo.isNull()) {
        // Usar sprite
        QPixmap torpedoEscalado = spriteTorpedo.scaled(ancho * 2, alto * 2,
                                                       Qt::KeepAspectRatio, Qt::SmoothTransformation);

        painter.drawPixmap(-ancho, -alto, torpedoEscalado);
    } else {
        // Fallback: dibujo manual
        painter.setBrush(QColor(255, 50, 50));
        painter.setPen(Qt::black);
        painter.drawEllipse(-ancho/2, -alto/2, ancho, alto);

        // Punta del torpedo
        QPolygon punta;
        punta << QPoint(ancho/2, 0)
              << QPoint(ancho/2 + 8, -4)
              << QPoint(ancho/2 + 8, 4);
        painter.setBrush(QColor(150, 30, 30));
        painter.drawPolygon(punta);
    }

    painter.restore();

    // Estela (trail)
    painter.setPen(QPen(QColor(255, 255, 255, 150), 2));
    painter.drawLine(
        posicion.x, posicion.y,
        posicion.x - velocidad.x * 0.05f,
        posicion.y - velocidad.y * 0.05f
        );
}

void Torpedo::onColision(Entidad* otra) {
    if (!otra) return;

    // El torpedo se destruye al colisionar con cualquier cosa
    // excepto con el enemigo que lo disparo
    if (otra->getTipo() != TipoEntidad::ENEMIGO_SUBMARINO) {
        destruir();
    }
}

// ========== CONFIGURACION ==========

void Torpedo::configurarTrayectoria(const Vector2D& posInicial, float angulo, float vel) {
    anguloDisparo = angulo;
    velocidadInicial = vel;

    // Calcular componentes de velocidad
    // vx = v × cos(θ)
    // vy = v × sin(θ)
    float vx = velocidadInicial * std::cos(angulo);
    float vy = velocidadInicial * std::sin(angulo);

    // Crear nueva fisica parabolica
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
