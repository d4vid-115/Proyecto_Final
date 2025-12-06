#include "objetojuego.h"
#include <cmath>
#include <QColor>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ========== CONSTRUCTORES ==========

ObjetoJuego::ObjetoJuego()
    : Entidad(Vector2D(0, 0), TipoEntidad::OBJETO),
    tipoObjeto(TipoObjeto::MALETA),
    masa(1.0f),
    friccion(0.3f),
    enSuelo(true),
    suspendido(false),
    anguloBarco(0.0f),
    aceleracionDeslizamiento(0.0f),
    movCircular(nullptr),
    puntoAnclaje(0, 0),
    danioContacto(15.0f) {

    setDimensiones(32, 24);
    configurarSegunTipo();
}

ObjetoJuego::ObjetoJuego(const Vector2D& pos, TipoObjeto tipo)
    : Entidad(pos, TipoEntidad::OBJETO),
    tipoObjeto(tipo),
    masa(1.0f),
    friccion(0.3f),
    enSuelo(true),
    suspendido(false),
    anguloBarco(0.0f),
    aceleracionDeslizamiento(0.0f),
    movCircular(nullptr),
    puntoAnclaje(0, 0),
    danioContacto(15.0f) {

    setDimensiones(32, 24);
    configurarSegunTipo();
}

ObjetoJuego::~ObjetoJuego() {
    movCircular = nullptr;
}

// ========== CONFIGURACION ==========

void ObjetoJuego::configurarSegunTipo() {
    switch (tipoObjeto) {
    case TipoObjeto::MALETA:
        // Maleta: media masa, buena friccion
        masa = 5.0f;
        friccion = 0.4f;
        danioContacto = 10.0f;
        setDimensiones(40, 28);
        enSuelo = true;
        suspendido = false;
        break;

    case TipoObjeto::SILLA:
        // Silla: ligera, poca friccion
        masa = 3.0f;
        friccion = 0.2f;
        danioContacto = 15.0f;
        setDimensiones(32, 48);
        enSuelo = true;
        suspendido = false;
        break;

    case TipoObjeto::LAMPARA:
        // Lampara: colgante, no toca el suelo
        masa = 2.0f;
        friccion = 0.0f;
        danioContacto = 20.0f;
        setDimensiones(24, 32);
        enSuelo = false;
        suspendido = true;

        // Crear fisica MCU para oscilacion
        movCircular = new MovimientoCircular(60.0f, 0.5f, posicion);
        setFisica(movCircular);
        puntoAnclaje = posicion;
        break;

    case TipoObjeto::ESCOMBRO_PEQUENO:
        // Escombro pequeño: muy ligero, poca friccion
        masa = 1.0f;
        friccion = 0.15f;
        danioContacto = 5.0f;
        setDimensiones(20, 20);
        enSuelo = true;
        suspendido = false;
        break;

    case TipoObjeto::ESCOMBRO_GRANDE:
        // Escombro grande: muy pesado, mas daño
        masa = 10.0f;
        friccion = 0.5f;
        danioContacto = 30.0f;
        setDimensiones(48, 48);
        enSuelo = true;
        suspendido = false;
        break;
    }
}

// ========== METODOS HEREDADOS ==========

void ObjetoJuego::actualizar(float dt) {
    if (!activo) return;

    if (suspendido && movCircular) {
        // Lampara colgante - usar MCU
        movCircular->aplicar(this);

        // Aumentar velocidad angular con la inclinacion
        float velocidadAngular = 0.5f + (anguloBarco / 50.0f);
        movCircular->setVelocidadAngular(velocidadAngular);
    } else if (enSuelo) {
        // Objeto en el suelo - afectado por inclinacion
        actualizarPorInclinacion(dt);
    } else {
        // Objeto en el aire - caida libre
        velocidad.y += 9.8f * dt; // Gravedad
        posicion += velocidad * dt;
    }

    actualizarColision();
}

void ObjetoJuego::actualizarPorInclinacion(float dt) {
    // Calcular aceleracion por deslizamiento en plano inclinado
    // a = g × sin(θ)

    float anguloRad = anguloBarco * M_PI / 180.0f;
    aceleracionDeslizamiento = 9.8f * std::sin(anguloRad);

    // Direccion del deslizamiento (hacia la derecha si inclinado a la derecha)
    float direccion = (anguloBarco > 0) ? 1.0f : -1.0f;

    // Aplicar aceleracion
    velocidad.x += aceleracionDeslizamiento * direccion * dt;

    // Aplicar friccion
    velocidad.x *= (1.0f - friccion);

    // Limitar velocidad segun masa (objetos pesados son mas lentos)
    float velocidadMax = 200.0f / masa;
    if (std::abs(velocidad.x) > velocidadMax) {
        velocidad.x = (velocidad.x > 0) ? velocidadMax : -velocidadMax;
    }

    // Actualizar posicion
    posicion += velocidad * dt;
}

void ObjetoJuego::renderizar(QPainter& painter) {
    if (!activo) return;

    painter.save();
    painter.setPen(Qt::black);

    switch (tipoObjeto) {
    case TipoObjeto::MALETA:
    {
        painter.setBrush(QColor(139, 69, 19));
        painter.drawRect(posicion.x, posicion.y, ancho, alto);

        painter.setPen(QPen(QColor(101, 50, 10), 2));
        painter.drawLine(posicion.x + ancho/2, posicion.y,
                         posicion.x + ancho/2, posicion.y + alto);
        painter.drawLine(posicion.x, posicion.y + alto/2,
                         posicion.x + ancho, posicion.y + alto/2);

        painter.setBrush(QColor(180, 100, 30));
        painter.drawRect(posicion.x + ancho/2 - 10, posicion.y - 5, 20, 5);
    }
    break;

    case TipoObjeto::SILLA:
    {
        painter.setBrush(QColor(160, 82, 45));

        painter.drawRect(posicion.x, posicion.y + alto - 24, ancho, 8);
        painter.drawRect(posicion.x + 4, posicion.y, 4, alto - 20);
        painter.drawRect(posicion.x + 2, posicion.y + alto - 16, 4, 16);
        painter.drawRect(posicion.x + ancho - 6, posicion.y + alto - 16, 4, 16);
        painter.drawRect(posicion.x + 2, posicion.y + alto - 8, 4, 8);
        painter.drawRect(posicion.x + ancho - 6, posicion.y + alto - 8, 4, 8);
    }
    break;

    case TipoObjeto::LAMPARA:
    {
        painter.setPen(QPen(QColor(100, 100, 100), 2));
        painter.drawLine(posicion.x + ancho/2, posicion.y,
                         puntoAnclaje.x, puntoAnclaje.y);

        painter.setBrush(QColor(255, 215, 0));
        painter.setPen(Qt::black);

        QPolygon pantalla;
        pantalla << QPoint(posicion.x + ancho/2 - 12, posicion.y + 8)
                 << QPoint(posicion.x + ancho/2 + 12, posicion.y + 8)
                 << QPoint(posicion.x + ancho/2 + 16, posicion.y + alto)
                 << QPoint(posicion.x + ancho/2 - 16, posicion.y + alto);
        painter.drawPolygon(pantalla);

        painter.setBrush(QColor(255, 255, 200));
        painter.drawEllipse(posicion.x + ancho/2 - 6, posicion.y + 12, 12, 12);

        painter.setBrush(QColor(255, 255, 150, 50));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(posicion.x + ancho/2 - 20, posicion.y + 10, 40, 40);
    }
    break;

    case TipoObjeto::ESCOMBRO_PEQUENO:
    {
        painter.setBrush(QColor(120, 120, 120));
        painter.drawEllipse(posicion.x, posicion.y, ancho, alto);

        painter.setBrush(QColor(90, 90, 90));
        painter.drawEllipse(posicion.x + 5, posicion.y + 3, 8, 8);
    }
    break;

    case TipoObjeto::ESCOMBRO_GRANDE:
    {
        painter.setBrush(QColor(80, 80, 80));

        QPolygon escombro;
        escombro << QPoint(posicion.x, posicion.y + alto/2)
                 << QPoint(posicion.x + ancho/3, posicion.y)
                 << QPoint(posicion.x + 2*ancho/3, posicion.y + alto/4)
                 << QPoint(posicion.x + ancho, posicion.y + alto/3)
                 << QPoint(posicion.x + ancho, posicion.y + alto)
                 << QPoint(posicion.x + ancho/2, posicion.y + alto);
        painter.drawPolygon(escombro);

        painter.setPen(QPen(QColor(50, 50, 50), 2));
        painter.drawLine(posicion.x + 10, posicion.y + 10,
                         posicion.x + 30, posicion.y + 40);
    }
    break;
    }

    painter.restore();

    // Indicador de velocidad
    if (velocidad.magnitud() > 50.0f && !suspendido) {
        painter.setPen(QPen(QColor(255, 0, 0), 2));
        float angulo = std::atan2(velocidad.y, velocidad.x);
        float longFlecha = 20.0f;

        float dx = longFlecha * std::cos(angulo);
        float dy = longFlecha * std::sin(angulo);

        painter.drawLine(
            posicion.x + ancho/2,
            posicion.y + alto/2,
            posicion.x + ancho/2 + dx,
            posicion.y + alto/2 + dy
            );
    }
}

void ObjetoJuego::onColision(Entidad* otra) {
    if (!otra) return;

    // Causar daño segun el tipo de entidad
    switch (otra->getTipo()) {
    case TipoEntidad::JUGADOR:
        // El jugador recibira el daño
        break;

    case TipoEntidad::NPC:
        // El NPC recibira el daño
        break;

    default:
        break;
    }

    // Rebotar ligeramente al colisionar
    if (!suspendido && velocidad.magnitud() > 10.0f) {
        velocidad *= -0.3f; // Rebotar con 30% de la velocidad
    }
}

// ========== METODOS ESPECIFICOS ==========

void ObjetoJuego::setAnguloBarco(float angulo) {
    anguloBarco = angulo;

    // Actualizar velocidad angular de lampara si es suspendida
    if (suspendido && movCircular) {
        float velocidadAngular = 0.5f + (angulo / 50.0f);
        movCircular->setVelocidadAngular(velocidadAngular);
    }
}

void ObjetoJuego::colgar(const Vector2D& anclaje) {
    suspendido = true;
    enSuelo = false;
    puntoAnclaje = anclaje;

    // Crear fisica MCU
    if (!movCircular) {
        float radio = posicion.distanciaA(anclaje);
        movCircular = new MovimientoCircular(radio, 0.5f, anclaje);
        movCircular->setCentroPivote(anclaje);
        setFisica(movCircular);
    }
}

void ObjetoJuego::soltar() {
    suspendido = false;
    enSuelo = false; // Esta cayendo

    // Eliminar fisica MCU
    if (movCircular) {
        setFisica(nullptr); // Esto eliminara movCircular
        movCircular = nullptr;
    }
}

// ========== GETTERS ==========

TipoObjeto ObjetoJuego::getTipoObjeto() const {
    return tipoObjeto;
}

float ObjetoJuego::getMasa() const {
    return masa;
}

bool ObjetoJuego::estaSuspendido() const {
    return suspendido;
}

bool ObjetoJuego::estaEnSuelo() const {
    return enSuelo;
}

float ObjetoJuego::getDanioContacto() const {
    // El daño aumenta con la velocidad
    float factorVelocidad = velocidad.magnitud() / 100.0f;
    return danioContacto * (1.0f + factorVelocidad);
}
