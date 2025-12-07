#include "jugador.h"
#include "gestorsprites.h"
#include <QColor>

// ========== CONSTRUCTORES ==========

Jugador::Jugador()
    : Entidad(Vector2D(400, 300), TipoEntidad::JUGADOR),
    salud(200.0f), saludMaxima(200.0f),
    vidas(1), puntuacion(0),
    velocidadBase(150.0f),
    oxigeno(150.0f), oxigenoMaximo(150.0f),
    estadoActual(EstadoAnimacion::IDLE),
    tiempoMuerte(0.0f),
    habilidadActiva(false), tiempoHabilidad(0.0f),
    cooldownHabilidad(5.0f), tiempoCooldown(0.0f),
    invencible(false), tiempoInvencibilidad(0.0f) {

    setDimensiones(48, 48);
    cargarAnimaciones();
}

Jugador::Jugador(const Vector2D& pos)
    : Entidad(pos, TipoEntidad::JUGADOR),
    salud(200.0f), saludMaxima(200.0f),
    vidas(1), puntuacion(0),
    velocidadBase(150.0f),
    oxigeno(150.0f), oxigenoMaximo(150.0f),
    estadoActual(EstadoAnimacion::IDLE),
    tiempoMuerte(0.0f),
    habilidadActiva(false), tiempoHabilidad(0.0f),
    cooldownHabilidad(5.0f), tiempoCooldown(0.0f),
    invencible(false), tiempoInvencibilidad(0.0f) {

    setDimensiones(48, 48);
    cargarAnimaciones();
}

Jugador::~Jugador() {
}

// ========== CARGAR ANIMACIONES ==========

void Jugador::cargarAnimaciones() {
    GestorSprites* gestor = GestorSprites::obtenerInstancia();

    QPixmap idle = gestor->getSprite("jugador_idle");
    QPixmap swim = gestor->getSprite("jugador_swim");
    QPixmap hurt = gestor->getSprite("jugador_hurt");
    QPixmap death = gestor->getSprite("jugador_death");

    // ===== CICLOS POR SEGUNDO (cuantas veces se repite la animacion completa) =====
    // Valores altos = animacion SUPER RAPIDA
    if (!idle.isNull()) animIdle = AnimacionSprite(idle, 64, 64, 6, 8.0f);
    if (!swim.isNull()) animSwim = AnimacionSprite(swim, 64, 64, 6, 10.0f);
    if (!hurt.isNull()) animHurt = AnimacionSprite(hurt, 64, 64, 3, 12.0f);
    if (!death.isNull()) animDeath = AnimacionSprite(death, 64, 64, 6, 3.0f);
}

// ========== METODOS HEREDADOS ==========

void Jugador::actualizar(float dt) {
    if (!activo) return;

    // ===== ANIMACION DE MUERTE =====
    if (salud <= 0.0f && estadoActual != EstadoAnimacion::DEATH) {
        estadoActual = EstadoAnimacion::DEATH;
        animDeath.reiniciar();
        tiempoMuerte = 0.0f;
        velocidad = Vector2D(0, 0);  // Detener movimiento
    }

    if (estadoActual == EstadoAnimacion::DEATH) {
        tiempoMuerte += dt;
        animDeath.actualizar(dt);

        // Despues de 3 segundos de animacion, marcar como muerto
        if (tiempoMuerte >= 3.0f) {
            activo = false;
            vidas = 0;
        }
        return;
    }

    // Aplicar fisica
    aplicarFisica(dt);

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
            estadoActual = EstadoAnimacion::IDLE;
        }
    }

    // ===== ACTUALIZAR ANIMACION SEGUN ESTADO =====
    if (invencible && estadoActual != EstadoAnimacion::HURT) {
        estadoActual = EstadoAnimacion::HURT;
        animHurt.reiniciar();
    } else if (!invencible) {
        if (velocidad.magnitud() > 5.0f) {
            if (estadoActual != EstadoAnimacion::SWIM) {
                estadoActual = EstadoAnimacion::SWIM;
                animSwim.reiniciar();
            }
        } else {
            if (estadoActual != EstadoAnimacion::IDLE) {
                estadoActual = EstadoAnimacion::IDLE;
                animIdle.reiniciar();
            }
        }
    }

    // Actualizar animacion actual
    switch (estadoActual) {
    case EstadoAnimacion::IDLE:
        animIdle.actualizar(dt);
        break;
    case EstadoAnimacion::SWIM:
        animSwim.actualizar(dt);
        break;
    case EstadoAnimacion::HURT:
        animHurt.actualizar(dt);
        break;
    case EstadoAnimacion::DEATH:
        animDeath.actualizar(dt);
        break;
    }

    // Actualizar posicion
    posicion += velocidad * dt;
    actualizarColision();
}

void Jugador::renderizar(QPainter& painter) {
    if (!activo) return;

    painter.save();

    // Dibujar jugador (barco pequeño)
    if (habilidadActiva) {
        painter.setBrush(QColor(255, 215, 0));
    } else {
        painter.setBrush(QColor(139, 69, 19));
    }

    painter.setPen(Qt::black);

    // Casco del barco
    QPolygon casco;
    casco << QPoint(posicion.x, posicion.y + alto)
          << QPoint(posicion.x + ancho/2, posicion.y)
          << QPoint(posicion.x + ancho, posicion.y + alto);
    painter.drawPolygon(casco);

    // Vela
    painter.setBrush(Qt::white);
    painter.drawRect(posicion.x + ancho/2 - 2, posicion.y + 5, 4, alto - 10);

    QPolygon vela;
    vela << QPoint(posicion.x + ancho/2, posicion.y + 8)
         << QPoint(posicion.x + ancho/2 + 12, posicion.y + 15)
         << QPoint(posicion.x + ancho/2, posicion.y + 22);
    painter.drawPolygon(vela);

    painter.restore();

    // Barra de vida
    float porcentajeSalud = salud / saludMaxima;

    painter.setBrush(QColor(255, 0, 0));
    painter.drawRect(posicion.x, posicion.y - 10, ancho, 5);

    painter.setBrush(QColor(0, 255, 0));
    painter.drawRect(posicion.x, posicion.y - 10, ancho * porcentajeSalud, 5);
}

void Jugador::renderizarNadador(QPainter& painter) {
    if (!activo && estadoActual != EstadoAnimacion::DEATH) return;

    painter.save();

    // Obtener frame actual segun estado
    QPixmap frameActual;

    switch (estadoActual) {
    case EstadoAnimacion::IDLE:
        frameActual = animIdle.getFrameActual();
        break;
    case EstadoAnimacion::SWIM:
        frameActual = animSwim.getFrameActual();
        break;
    case EstadoAnimacion::HURT:
        frameActual = animHurt.getFrameActual();
        break;
    case EstadoAnimacion::DEATH:
        frameActual = animDeath.getFrameActual();
        break;
    }

    // Voltear horizontalmente si va a la izquierda
    bool mirandoDerecha = velocidad.x >= 0;

    if (!frameActual.isNull()) {
        QPixmap frame = frameActual.scaled(ancho * 1.5f, alto * 1.5f,
                                           Qt::KeepAspectRatio, Qt::SmoothTransformation);

        if (!mirandoDerecha) {
            frame = frame.transformed(QTransform().scale(-1, 1));
        }

        painter.drawPixmap(posicion.x - ancho * 0.25f,
                           posicion.y - alto * 0.25f,
                           frame);
    } else {
        // Fallback
        painter.setBrush(QColor(255, 200, 150));
        painter.setPen(Qt::black);
        painter.drawEllipse(posicion.x, posicion.y, ancho, alto);
    }

    painter.restore();
}

void Jugador::onColision(Entidad* otra) {
    if (!otra || invencible) return;

    switch (otra->getTipo()) {
    case TipoEntidad::TORPEDO:
    case TipoEntidad::ENEMIGO_SUBMARINO:
        recibirDanio(25.0f);
        break;
    default:
        break;
    }
}

// ========== MOVIMIENTO ==========

void Jugador::mover(const Vector2D& direccion, float dt) {
    if (estadoActual == EstadoAnimacion::DEATH) return;  // No moverse si esta muriendo

    if (direccion.magnitud() > 0.01f) {
        Vector2D direccionNorm = direccion.normalizado();
        velocidad = direccionNorm * velocidadBase;

        if (habilidadActiva) {
            velocidad *= 1.5f;
        }
    } else {
        velocidad = Vector2D(0, 0);
    }
}

void Jugador::detener() {
    if (estadoActual != EstadoAnimacion::DEATH) {
        velocidad = Vector2D(0, 0);
    }
}

// ========== SALUD Y DAÑO ==========

void Jugador::recibirDanio(float cantidad) {
    if (invencible || estadoActual == EstadoAnimacion::DEATH) return;

    salud -= cantidad;

    invencible = true;
    tiempoInvencibilidad = 1.0f;

    if (salud <= 0.0f) {
        salud = 0.0f;
        // La muerte se maneja en actualizar()
    }
}

void Jugador::curar(float cantidad) {
    if (estadoActual == EstadoAnimacion::DEATH) return;

    salud += cantidad;
    if (salud > saludMaxima) {
        salud = saludMaxima;
    }
}

bool Jugador::estaMuerto() const {
    return vidas <= 0 || salud <= 0.0f;
}

// ========== HABILIDAD ESPECIAL ==========

void Jugador::usarHabilidad() {
    if (!puedeUsarHabilidad() || estadoActual == EstadoAnimacion::DEATH) return;

    habilidadActiva = true;
    tiempoHabilidad = 3.0f;
    tiempoCooldown = cooldownHabilidad;
}

bool Jugador::puedeUsarHabilidad() const {
    return !habilidadActiva && tiempoCooldown <= 0.0f;
}

// ========== VIDAS Y PUNTUACION ==========

void Jugador::perderVida() {
    vidas = 0;
    activo = false;
}

void Jugador::ganarVida() {
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
