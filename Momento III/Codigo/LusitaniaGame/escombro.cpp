#include "escombro.h"
#include <QColor>

// ========== CONSTRUCTORES ==========

Escombro::Escombro()
    : Entidad(Vector2D(0, 0), TipoEntidad::ESCOMBRO),
    tipoEscombro(TipoEscombro::NEUTRO),
    densidad(1.0f),
    flotacion(nullptr),
    enAgua(true),
    velocidadTerminal(100.0f) {

    setDimensiones(24, 24);
    configurarDensidad();
}

Escombro::Escombro(const Vector2D& pos, TipoEscombro tipo)
    : Entidad(pos, TipoEntidad::ESCOMBRO),
    tipoEscombro(tipo),
    densidad(1.0f),
    flotacion(nullptr),
    enAgua(true),
    velocidadTerminal(100.0f) {

    setDimensiones(24, 24);
    configurarDensidad();
}

Escombro::~Escombro() {
    flotacion = nullptr;
}

// ========== CONFIGURACION ==========

void Escombro::configurarDensidad() {
    switch (tipoEscombro) {
    case TipoEscombro::MADERA:
        densidad = 0.6f;  // Flota
        velocidadTerminal = 80.0f;
        setDimensiones(32, 16);
        break;

    case TipoEscombro::METAL_PESADO:
        densidad = 3.0f;  // Se hunde rapido
        velocidadTerminal = 200.0f;
        setDimensiones(20, 20);
        break;

    case TipoEscombro::SALVAVIDAS:
        densidad = 0.3f;  // Flota muy rapido
        velocidadTerminal = 120.0f;
        setDimensiones(28, 28);
        break;

    case TipoEscombro::NEUTRO:
        densidad = 1.0f;  // Neutro
        velocidadTerminal = 50.0f;
        setDimensiones(24, 24);
        break;
    }

    flotacion = new FisicaFlotacion(densidad);
    flotacion->setVelocidadTerminal(velocidadTerminal);
    setFisica(flotacion);
}

// ========== METODOS HEREDADOS ==========

void Escombro::actualizar(float dt) {
    if (!activo) return;

    // ===== APLICAR GRAVEDAD SEGUN DENSIDAD =====
    float gravedad = 98.0f;  // Gravedad del agua (mas fuerte que en aire)

    // Fuerza de flotacion = gravedad × (densidad - 1.0)
    // Si densidad < 1.0 - flota (fuerza negativa)
    // Si densidad > 1.0 - se hunde (fuerza positiva)
    float fuerzaFlotacion = gravedad * (densidad - 1.0f);

    velocidad.y += fuerzaFlotacion * dt;

    // Limitar velocidad terminal
    if (velocidad.y > velocidadTerminal) {
        velocidad.y = velocidadTerminal;
    } else if (velocidad.y < -velocidadTerminal) {
        velocidad.y = -velocidadTerminal;
    }

    // Fricción del agua (resistencia)
    velocidad *= 0.98f;

    posicion += velocidad * dt;
    actualizarColision();

    // Destruir si sale muy lejos
    if (posicion.y > 4000.0f || posicion.y < -500.0f) {
        destruir();
    }
}

void Escombro::renderizar(QPainter& painter) {
    if (!activo) return;

    painter.setPen(QPen(Qt::black, 2));

    switch (tipoEscombro) {
    case TipoEscombro::MADERA:
        painter.setBrush(QColor(139, 69, 19));
        painter.drawRect(posicion.x, posicion.y, ancho, alto);

        painter.setPen(QColor(101, 50, 10));
        painter.drawLine(posicion.x, posicion.y + alto/2,
                         posicion.x + ancho, posicion.y + alto/2);
        break;

    case TipoEscombro::METAL_PESADO:
        painter.setBrush(QColor(100, 100, 100));
        painter.drawRect(posicion.x, posicion.y, ancho, alto);

        painter.setBrush(QColor(150, 150, 150));
        painter.drawEllipse(posicion.x + 3, posicion.y + 3, 5, 5);
        painter.drawEllipse(posicion.x + ancho - 8, posicion.y + 3, 5, 5);
        break;

    case TipoEscombro::SALVAVIDAS:
        painter.setBrush(QColor(255, 100, 0));
        painter.drawEllipse(posicion.x, posicion.y, ancho, alto);

        painter.setBrush(QColor(20, 60, 100, 150));
        painter.drawEllipse(posicion.x + ancho/4, posicion.y + alto/4, ancho/2, alto/2);

        painter.setPen(QPen(Qt::white, 4));
        painter.drawLine(posicion.x, posicion.y + alto/2, posicion.x + ancho/4, posicion.y + alto/2);
        painter.drawLine(posicion.x + 3*ancho/4, posicion.y + alto/2, posicion.x + ancho, posicion.y + alto/2);
        break;

    case TipoEscombro::NEUTRO:
        painter.setBrush(QColor(100, 150, 180));
        painter.drawRect(posicion.x, posicion.y, ancho, alto);
        break;
    }

    // Indicador de direccion (flecha)
    painter.setPen(QPen(Qt::white, 2));
    if (velocidad.y < -10.0f) {
        // Flecha arriba (flotando)
        painter.drawLine(posicion.x + ancho/2, posicion.y - 5,
                         posicion.x + ancho/2, posicion.y - 15);
        painter.drawLine(posicion.x + ancho/2 - 3, posicion.y - 12,
                         posicion.x + ancho/2, posicion.y - 15);
        painter.drawLine(posicion.x + ancho/2 + 3, posicion.y - 12,
                         posicion.x + ancho/2, posicion.y - 15);
    } else if (velocidad.y > 10.0f) {
        // Flecha abajo (hundiendose)
        painter.drawLine(posicion.x + ancho/2, posicion.y + alto + 5,
                         posicion.x + ancho/2, posicion.y + alto + 15);
        painter.drawLine(posicion.x + ancho/2 - 3, posicion.y + alto + 12,
                         posicion.x + ancho/2, posicion.y + alto + 15);
        painter.drawLine(posicion.x + ancho/2 + 3, posicion.y + alto + 12,
                         posicion.x + ancho/2, posicion.y + alto + 15);
    }
}

void Escombro::onColision(Entidad* otra) {
    if (!otra) return;
}

// ========== METODOS ESPECIFICOS ==========

void Escombro::flotar() {
    if (flotacion) {
        flotacion->setDensidadObjeto(0.5f);
    }
}

void Escombro::hundir() {
    if (flotacion) {
        flotacion->setDensidadObjeto(2.0f);
    }
}

bool Escombro::estaFlotando() const {
    return flotacion && flotacion->estaFlotando();
}

bool Escombro::estaHundiendo() const {
    return flotacion && flotacion->estaHundiendo();
}

// ========== GETTERS ==========

float Escombro::getDensidad() const {
    return densidad;
}

bool Escombro::estaEnAgua() const {
    return enAgua;
}

// ========== SETTERS ==========

void Escombro::setTipoEscombro(TipoEscombro tipo) {
    tipoEscombro = tipo;
    configurarDensidad();
}

void Escombro::setEnAgua(bool agua) {
    enAgua = agua;
}
