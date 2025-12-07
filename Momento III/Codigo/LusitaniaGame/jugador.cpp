#include "jugador.h"
#include "gestorsprites.h"
#include <QColor>

// ========== CONSTRUCTORES ==========

Jugador::Jugador()
    : Entidad(Vector2D(400, 300), TipoEntidad::JUGADOR),
    salud(100.0f), saludMaxima(100.0f),
    vidas(3), puntuacion(0),
    velocidadBase(150.0f),
    oxigeno(100.0f),
    habilidadActiva(false), tiempoHabilidad(0.0f),
    cooldownHabilidad(5.0f), tiempoCooldown(0.0f),
    invencible(false), tiempoInvencibilidad(0.0f) {

    setDimensiones(32, 48);
}

Jugador::Jugador(const Vector2D& pos)
    : Entidad(pos, TipoEntidad::JUGADOR),
    salud(100.0f), saludMaxima(100.0f),
    vidas(3), puntuacion(0),
    velocidadBase(150.0f),
    oxigeno(100.0f),
    habilidadActiva(false), tiempoHabilidad(0.0f),
    cooldownHabilidad(5.0f), tiempoCooldown(0.0f),
    invencible(false), tiempoInvencibilidad(0.0f) {

    setDimensiones(32, 48);
}

Jugador::~Jugador() {
    // Destructor
}

// ========== METODOS HEREDADOS ==========

void Jugador::actualizar(float dt) {
    if (!activo) return;

    // Actualizar fisica si existe
    aplicarFisica(dt);

    // Decrementar oxigeno (solo en nivel 3 submarino)
    if (oxigeno > 0.0f) {
        oxigeno -= dt * 2.0f; // Pierde 2 puntos por segundo
        if (oxigeno < 0.0f) oxigeno = 0.0f;
    }

    // Actualizar timers de habilidad
    if (tiempoCooldown > 0.0f) {
        tiempoCooldown -= dt;
    }

    if (habilidadActiva) {
        tiempoHabilidad -= dt;
        if (tiempoHabilidad <= 0.0f) {
            habilidadActiva = false;
        }
    }

    // Actualizar invencibilidad
    if (invencible) {
        tiempoInvencibilidad -= dt;
        if (tiempoInvencibilidad <= 0.0f) {
            invencible = false;
        }
    }

    // Actualizar posicion con velocidad
    posicion += velocidad * dt;
    actualizarColision();
}

void Jugador::renderizar(QPainter& painter) {
    if (!activo) return;

    // Parpadear si esta invencible
    if (invencible) {
        int parpadeo = (int)(tiempoInvencibilidad * 10) % 2;
        if (parpadeo == 0) return;
    }

    painter.save();

    // Dibujar jugador (barco pequeño con color segun estado)
    if (habilidadActiva) {
        painter.setBrush(QColor(255, 215, 0)); // Dorado si habilidad activa
    } else {
        painter.setBrush(QColor(139, 69, 19)); // Marron (barco de madera)
    }

    painter.setPen(Qt::black);

    // Casco del barco
    QPolygon casco;
    casco << QPoint(posicion.x, posicion.y + alto)
          << QPoint(posicion.x + ancho/2, posicion.y)
          << QPoint(posicion.x + ancho, posicion.y + alto);
    painter.drawPolygon(casco);

    // Vela/Mastil
    painter.setBrush(Qt::white);
    painter.drawRect(posicion.x + ancho/2 - 2, posicion.y + 5, 4, alto - 10);

    QPolygon vela;
    vela << QPoint(posicion.x + ancho/2, posicion.y + 8)
         << QPoint(posicion.x + ancho/2 + 12, posicion.y + 15)
         << QPoint(posicion.x + ancho/2, posicion.y + 22);
    painter.drawPolygon(vela);

    painter.restore();

    // Barra de vida encima del jugador
    float porcentajeSalud = salud / saludMaxima;

    painter.setBrush(QColor(255, 0, 0));
    painter.drawRect(posicion.x, posicion.y - 10, ancho, 5);

    painter.setBrush(QColor(0, 255, 0));
    painter.drawRect(posicion.x, posicion.y - 10, ancho * porcentajeSalud, 5);
}

void Jugador::renderizarNadador(QPainter& painter) {
    if (!activo) return;

    // Parpadear si esta invencible
    if (invencible) {
        int parpadeo = (int)(tiempoInvencibilidad * 10) % 2;
        if (parpadeo == 0) return;
    }

    painter.save();

    GestorSprites* gestor = GestorSprites::obtenerInstancia();
    QPixmap spriteJugador;

    // Seleccionar sprite segun estado
    if (salud <= 0) {
        spriteJugador = gestor->getSprite("jugador_death");
    } else if (invencible) {
        spriteJugador = gestor->getSprite("jugador_hurt");
    } else if (velocidad.magnitud() > 1.0f) {
        spriteJugador = gestor->getSprite("jugador_swim");
    } else {
        spriteJugador = gestor->getSprite("jugador_idle");
    }

    // Determinar direccion (voltear sprite si va a la izquierda)
    bool mirandoDerecha = velocidad.x >= 0;

    if (!spriteJugador.isNull()) {
        QPixmap jugadorEscalado = spriteJugador.scaled(ancho * 1.5f, alto * 1.5f,
                                                       Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // Voltear horizontalmente si va a la izquierda
        if (!mirandoDerecha) {
            jugadorEscalado = jugadorEscalado.transformed(
                QTransform().scale(-1, 1));
        }

        painter.drawPixmap(posicion.x - ancho * 0.25f,
                           posicion.y - alto * 0.25f,
                           jugadorEscalado);
    } else {
        // Fallback: dibujo manual simple
        if (habilidadActiva) {
            painter.setBrush(QColor(255, 215, 0));
        } else {
            painter.setBrush(QColor(255, 200, 150));
        }

        painter.setPen(Qt::black);

        // Cuerpo
        painter.drawEllipse(posicion.x, posicion.y, ancho, alto);

        // Brazos/piernas (lineas simples)
        painter.drawLine(posicion.x + ancho/2, posicion.y + alto/2,
                         posicion.x + ancho/2 - 8, posicion.y + alto/2 + 10);
        painter.drawLine(posicion.x + ancho/2, posicion.y + alto/2,
                         posicion.x + ancho/2 + 8, posicion.y + alto/2 + 10);
    }

    painter.restore();

    // Barra de vida
    float porcentajeSalud = salud / saludMaxima;

    painter.setBrush(QColor(255, 0, 0));
    painter.drawRect(posicion.x, posicion.y - 10, ancho, 5);

    painter.setBrush(QColor(0, 255, 0));
    painter.drawRect(posicion.x, posicion.y - 10, ancho * porcentajeSalud, 5);
}

void Jugador::onColision(Entidad* otra) {
    if (!otra || invencible) return;

    // Manejar colisiones segun el tipo de entidad
    switch (otra->getTipo()) {
    case TipoEntidad::TORPEDO:
    case TipoEntidad::ENEMIGO_SUBMARINO:
        recibirDanio(25.0f);
        break;

    case TipoEntidad::VORTICE:
        // El vortice aplica fuerza, no daño directo
        break;

    case TipoEntidad::ESCOMBRO:
        // Algunos escombros pueden dar puntos o daño
        break;

    default:
        break;
    }
}

// ========== MOVIMIENTO ==========

void Jugador::mover(const Vector2D& direccion, float dt) {
    // Normalizar direccion si no es cero
    if (direccion.magnitud() > 0.01f) {
        Vector2D direccionNorm = direccion.normalizado();
        velocidad = direccionNorm * velocidadBase;

        // Si la habilidad esta activa, mover mas rapido
        if (habilidadActiva) {
            velocidad *= 1.5f;
        }
    } else {
        velocidad = Vector2D(0, 0);
    }
}

void Jugador::detener() {
    velocidad = Vector2D(0, 0);
}

// ========== SALUD Y DAÑO ==========

void Jugador::recibirDanio(float cantidad) {
    if (invencible) return;

    salud -= cantidad;

    // Activar invencibilidad temporal (1 segundo)
    invencible = true;
    tiempoInvencibilidad = 1.0f;

    // Morir si la salud llega a 0
    if (salud <= 0.0f) {
        salud = 0.0f;
        perderVida();
    }
}

void Jugador::curar(float cantidad) {
    salud += cantidad;
    if (salud > saludMaxima) {
        salud = saludMaxima;
    }
}

bool Jugador::estaMuerto() const {
    return vidas <= 0;
}

// ========== HABILIDAD ESPECIAL ==========

void Jugador::usarHabilidad() {
    if (!puedeUsarHabilidad()) return;

    // Activar habilidad por 3 segundos
    habilidadActiva = true;
    tiempoHabilidad = 3.0f;

    // Empezar cooldown
    tiempoCooldown = cooldownHabilidad;

    // Efecto de la habilidad: velocidad aumentada (ya implementado en mover)
}

bool Jugador::puedeUsarHabilidad() const {
    return !habilidadActiva && tiempoCooldown <= 0.0f;
}

// ========== VIDAS Y PUNTUACION ==========

void Jugador::perderVida() {
    vidas--;

    if (vidas > 0) {
        // Reiniciar salud
        salud = saludMaxima;
    } else {
        // Game Over
        activo = false;
    }
}

void Jugador::ganarVida() {
    vidas++;
}

void Jugador::agregarPuntos(int puntos) {
    puntuacion += puntos;
}

// ========== GETTERS ==========

float Jugador::getSalud() const {
    return salud;
}

float Jugador::getSaludMaxima() const {
    return saludMaxima;
}

int Jugador::getVidas() const {
    return vidas;
}

int Jugador::getPuntuacion() const {
    return puntuacion;
}

float Jugador::getVelocidadBase() const {
    return velocidadBase;
}

bool Jugador::estaInvencible() const {
    return invencible;
}

// ========== SETTERS ==========

void Jugador::setSalud(float s) {
    salud = s;
    if (salud > saludMaxima) salud = saludMaxima;
    if (salud < 0.0f) salud = 0.0f;
}

void Jugador::setVidas(int v) {
    vidas = v;
}
