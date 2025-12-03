#include "npc.h"
#include <cmath>
#include <cstdlib>
#include <QColor>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ========== CONSTRUCTORES ==========

NPC::NPC()
    : Entidad(Vector2D(0, 0), TipoEntidad::NPC),
    tipoNPC(TipoNPC::HOMBRE),
    estadoNPC(EstadoNPC::PANICO),
    velocidadBase(80.0f),
    direccionPanico(1, 0),
    tiempoDireccion(0.0f),
    jugador(nullptr),
    rangoSeguimiento(100.0f),
    resistencia(100.0f),
    consciente(true),
    tiempoInconsciente(0.0f),
    frameTiempo(0.0f),
    frameActual(0) {

    setDimensiones(24, 40);
    configurarSegunTipo();
}

NPC::NPC(const Vector2D& pos, TipoNPC tipo)
    : Entidad(pos, TipoEntidad::NPC),
    tipoNPC(tipo),
    estadoNPC(EstadoNPC::PANICO),
    velocidadBase(80.0f),
    direccionPanico(1, 0),
    tiempoDireccion(0.0f),
    jugador(nullptr),
    rangoSeguimiento(100.0f),
    resistencia(100.0f),
    consciente(true),
    tiempoInconsciente(0.0f),
    frameTiempo(0.0f),
    frameActual(0) {

    setDimensiones(24, 40);
    configurarSegunTipo();
}

NPC::~NPC() {
    jugador = nullptr;
}

// ========== CONFIGURACION ==========

void NPC::configurarSegunTipo() {
    // Cada tipo tiene caracteristicas diferentes
    switch (tipoNPC) {
    case TipoNPC::HOMBRE:
        // Hombres: mas rapidos, mas resistentes
        velocidadBase = 100.0f;
        resistencia = 100.0f;
        setDimensiones(26, 42);
        break;

    case TipoNPC::MUJER:
        // Mujeres: velocidad media, resistencia media
        velocidadBase = 90.0f;
        resistencia = 90.0f;
        setDimensiones(24, 40);
        break;

    case TipoNPC::NINO:
        // Niños: mas rapidos pero menos resistentes
        velocidadBase = 110.0f;
        resistencia = 70.0f;
        setDimensiones(20, 32);
        break;
    }

    // Elegir direccion inicial aleatoria
    float angulo = (rand() % 360) * M_PI / 180.0f;
    direccionPanico = Vector2D(std::cos(angulo), std::sin(angulo));
}

// ========== METODOS HEREDADOS ==========

void NPC::actualizar(float dt) {
    if (!activo) return;

    // Actualizar animacion
    frameTiempo += dt;
    if (frameTiempo > 0.1f) {
        frameActual = (frameActual + 1) % 4;
        frameTiempo = 0.0f;
    }

    // Si esta inconsciente, decrementar tiempo
    if (!consciente) {
        tiempoInconsciente -= dt;
        if (tiempoInconsciente <= 0.0f) {
            despertar();
        }
        return; // No se mueve mientras esta inconsciente
    }

    // Actualizar segun estado
    switch (estadoNPC) {
    case EstadoNPC::PANICO:
        actualizarMovimiento(dt);
        break;

    case EstadoNPC::SIGUIENDO:
        if (jugador && jugador->estaActivo()) {
            Vector2D direccion = jugador->getPosicion() - posicion;
            float distancia = direccion.magnitud();

            if (distancia > 50.0f) {
                // Seguir al jugador
                velocidad = direccion.normalizado() * velocidadBase;
            } else {
                // Muy cerca, frenar
                velocidad *= 0.95f;
            }
        } else {
            // Perdio al jugador, volver a panico
            estadoNPC = EstadoNPC::PANICO;
        }
        break;

    case EstadoNPC::RESCATADO:
        // Rescatado, no se mueve
        velocidad = Vector2D(0, 0);
        break;

    case EstadoNPC::CAIDO:
        // Caido, no se mueve (hasta despertar)
        velocidad = Vector2D(0, 0);
        break;
    }

    // Aplicar velocidad
    posicion += velocidad * dt;
    actualizarColision();
}

void NPC::actualizarMovimiento(float dt) {
    // Movimiento erratico en panico
    tiempoDireccion -= dt;

    if (tiempoDireccion <= 0.0f) {
        // Cambiar direccion aleatoriamente
        float angulo = (rand() % 360) * M_PI / 180.0f;
        direccionPanico = Vector2D(std::cos(angulo), std::sin(angulo));
        tiempoDireccion = 1.0f + (rand() % 200) / 100.0f; // 1-3 segundos
    }

    // Movimiento con la direccion de panico
    velocidad = direccionPanico * velocidadBase * 0.7f; // 70% velocidad en panico

    // Verificar si el jugador esta cerca
    if (jugador && jugador->estaActivo()) {
        float distancia = posicion.distanciaA(jugador->getPosicion());
        if (distancia < rangoSeguimiento) {
            // Cambiar a seguir al jugador
            estadoNPC = EstadoNPC::SIGUIENDO;
        }
    }
}

void NPC::renderizar(QPainter& painter) {
    if (!activo) return;

    painter.save();

    // Color segun tipo
    QColor colorBase;
    switch (tipoNPC) {
    case TipoNPC::HOMBRE:
        colorBase = QColor(50, 50, 150);  // Azul oscuro
        break;
    case TipoNPC::MUJER:
        colorBase = QColor(150, 50, 50);  // Rojo oscuro
        break;
    case TipoNPC::NINO:
        colorBase = QColor(50, 150, 50);  // Verde
        break;
    }

    // Si esta inconsciente, dibujar acostado
    if (!consciente || estadoNPC == EstadoNPC::CAIDO) {
        painter.setBrush(colorBase);
        painter.setPen(Qt::black);
        painter.drawEllipse(posicion.x, posicion.y + alto/2, ancho * 1.5f, alto/2);

        // Zzz para indicar inconsciente
        painter.setPen(QPen(Qt::white, 2));
        painter.drawText(posicion.x + ancho + 5, posicion.y, "Zzz");

        painter.restore();
        return;
    }

    // Dibujar NPC de pie
    painter.setBrush(colorBase);
    painter.setPen(Qt::black);

    // Cuerpo
    painter.drawRect(posicion.x + 4, posicion.y + 12, ancho - 8, alto - 12);

    // Cabeza
    painter.drawEllipse(posicion.x + 4, posicion.y, ancho - 8, 12);

    // Brazos (animados segun frame)
    int offsetBrazo = (frameActual % 2 == 0) ? 2 : -2;
    painter.drawLine(posicion.x + 2, posicion.y + 15,
                     posicion.x - 2, posicion.y + 25 + offsetBrazo);
    painter.drawLine(posicion.x + ancho - 2, posicion.y + 15,
                     posicion.x + ancho + 2, posicion.y + 25 - offsetBrazo);

    // Piernas (animadas)
    painter.drawLine(posicion.x + 8, posicion.y + alto,
                     posicion.x + 6, posicion.y + alto + 5 + offsetBrazo);
    painter.drawLine(posicion.x + ancho - 8, posicion.y + alto,
                     posicion.x + ancho - 6, posicion.y + alto + 5 - offsetBrazo);

    // Indicador de estado
    QColor colorEstado;
    switch (estadoNPC) {
    case EstadoNPC::PANICO:
        colorEstado = QColor(255, 255, 0);   // Amarillo
        break;
    case EstadoNPC::SIGUIENDO:
        colorEstado = QColor(0, 255, 0);     // Verde
        break;
    case EstadoNPC::RESCATADO:
        colorEstado = QColor(0, 255, 255);   // Cyan
        break;
    case EstadoNPC::CAIDO:
        colorEstado = QColor(255, 0, 0);     // Rojo
        break;
    }

    // Circulo de estado sobre la cabeza
    painter.setBrush(colorEstado);
    painter.drawEllipse(posicion.x + ancho/2 - 3, posicion.y - 8, 6, 6);

    // Barra de resistencia
    float porcentaje = resistencia / 100.0f;
    painter.setBrush(QColor(255, 0, 0));
    painter.drawRect(posicion.x, posicion.y - 15, ancho, 3);
    painter.setBrush(QColor(0, 255, 0));
    painter.drawRect(posicion.x, posicion.y - 15, ancho * porcentaje, 3);

    painter.restore();
}

void NPC::onColision(Entidad* otra) {
    if (!otra) return;

    switch (otra->getTipo()) {
    case TipoEntidad::OBJETO:
        // Colision con objeto cayendo
        caer(30.0f);
        break;

    case TipoEntidad::JUGADOR:
        // Colision con jugador - empezar a seguir
        if (estadoNPC == EstadoNPC::PANICO) {
            seguirJugador(otra);
        }
        break;

    default:
        break;
    }
}

// ========== COMPORTAMIENTO ==========

void NPC::entrarEnPanico() {
    estadoNPC = EstadoNPC::PANICO;

    // Elegir nueva direccion aleatoria
    float angulo = (rand() % 360) * M_PI / 180.0f;
    direccionPanico = Vector2D(std::cos(angulo), std::sin(angulo));
    tiempoDireccion = 2.0f;
}

void NPC::seguirJugador(Entidad* j) {
    jugador = j;
    estadoNPC = EstadoNPC::SIGUIENDO;
}

void NPC::rescatar() {
    estadoNPC = EstadoNPC::RESCATADO;
    velocidad = Vector2D(0, 0);
    activo = false; // Se "elimina" del juego (contado como rescatado)
}

void NPC::caer(float danio) {
    if (!consciente) return; // Ya esta caido

    resistencia -= danio;

    if (resistencia <= 0.0f) {
        // Caer inconsciente
        resistencia = 0.0f;
        consciente = false;
        estadoNPC = EstadoNPC::CAIDO;
        tiempoInconsciente = 5.0f; // 5 segundos inconsciente
    }
}

void NPC::despertar() {
    consciente = true;
    resistencia = 50.0f; // Se levanta con 50% de resistencia
    estadoNPC = EstadoNPC::PANICO;
    tiempoInconsciente = 0.0f;
}

// ========== GETTERS ==========

TipoNPC NPC::getTipoNPC() const {
    return tipoNPC;
}

EstadoNPC NPC::getEstadoNPC() const {
    return estadoNPC;
}

bool NPC::estaRescatado() const {
    return estadoNPC == EstadoNPC::RESCATADO;
}

bool NPC::estaConsciente() const {
    return consciente;
}

float NPC::getResistencia() const {
    return resistencia;
}

// ========== SETTERS ==========

void NPC::setEstadoNPC(EstadoNPC estado) {
    estadoNPC = estado;
}

void NPC::setJugador(Entidad* j) {
    jugador = j;
}
