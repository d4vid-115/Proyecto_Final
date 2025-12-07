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
    // Configurar densidad segun el tipo de escombro

    switch (tipoEscombro) {
    case TipoEscombro::MADERA:
        // Madera: ρ = 0.6 (flota rapido)
        densidad = 0.6f;
        velocidadTerminal = 80.0f;
        setDimensiones(32, 16); // Tabla alargada
        break;

    case TipoEscombro::METAL_PESADO:
        // Metal: ρ = 7.8 (se hunde muy rapido)
        densidad = 7.8f;
        velocidadTerminal = 150.0f;
        setDimensiones(20, 20); // Compacto y pesado
        break;

    case TipoEscombro::SALVAVIDAS:
        // Salvavidas: ρ = 0.3 (flota muy rapido)
        densidad = 0.3f;
        velocidadTerminal = 120.0f;
        setDimensiones(28, 28); // Circular
        break;

    case TipoEscombro::NEUTRO:
        // Neutro: ρ = 1.0 (flotacion neutra)
        densidad = 1.0f;
        velocidadTerminal = 50.0f;
        setDimensiones(24, 24);
        break;
    }

    // Crear fisica de flotacion
    flotacion = new FisicaFlotacion(densidad);
    flotacion->setVelocidadTerminal(velocidadTerminal);
    setFisica(flotacion);
}

// ========== METODOS HEREDADOS ==========

void Escombro::actualizar(float dt) {
    if (!activo) return;

    // Solo aplicar fisica si esta en el agua
    if (enAgua && flotacion) {
        flotacion->aplicar(this);
    } else {
        // Si no esta en agua, simplemente cae por gravedad
        velocidad.y += 9.8f * dt;
        posicion += velocidad * dt;
    }

    actualizarColision();

    // Destruir si sale muy abajo o muy arriba de la pantalla
    if (posicion.y > 1000.0f || posicion.y < -500.0f) {
        destruir();
    }
}

void Escombro::renderizar(QPainter& painter) {
    if (!activo) return;

    painter.setPen(Qt::black);

    // Renderizar segun el tipo
    switch (tipoEscombro) {
    case TipoEscombro::MADERA:
        // Tabla de madera (marron)
        painter.setBrush(QColor(139, 69, 19));
        painter.drawRect(posicion.x, posicion.y, ancho, alto);

        // Vetas de la madera
        painter.setPen(QColor(101, 50, 10));
        painter.drawLine(posicion.x + 5, posicion.y,
                         posicion.x + 5, posicion.y + alto);
        painter.drawLine(posicion.x + ancho - 5, posicion.y,
                         posicion.x + ancho - 5, posicion.y + alto);
        break;

    case TipoEscombro::METAL_PESADO:
        // Metal pesado (gris oscuro)
        painter.setBrush(QColor(70, 70, 70));
        painter.drawRect(posicion.x, posicion.y, ancho, alto);

        // Brillo metalico
        painter.setBrush(QColor(120, 120, 120, 100));
        painter.drawRect(posicion.x + 2, posicion.y + 2,
                         ancho - 4, alto - 4);
        break;

    case TipoEscombro::SALVAVIDAS:
        // Salvavidas (naranja con blanco)
        painter.setBrush(QColor(255, 140, 0));
        painter.drawEllipse(posicion.x, posicion.y, ancho, alto);

        // Rayas blancas
        painter.setBrush(Qt::white);
        painter.drawPie(posicion.x, posicion.y, ancho, alto, 0, 90 * 16);
        painter.drawPie(posicion.x, posicion.y, ancho, alto, 180 * 16, 90 * 16);
        break;

    case TipoEscombro::NEUTRO:
        // Neutro (azul grisaceo)
        painter.setBrush(QColor(100, 150, 180));
        painter.drawRect(posicion.x, posicion.y, ancho, alto);
        break;
    }

    // Indicador de flotacion (flecha)
    if (enAgua) {
        painter.setPen(QPen(Qt::white, 2));
        if (estaFlotando()) {
            // Flecha hacia arriba
            painter.drawLine(posicion.x + ancho/2, posicion.y - 5,
                             posicion.x + ancho/2, posicion.y - 15);
            painter.drawLine(posicion.x + ancho/2 - 3, posicion.y - 12,
                             posicion.x + ancho/2, posicion.y - 15);
            painter.drawLine(posicion.x + ancho/2 + 3, posicion.y - 12,
                             posicion.x + ancho/2, posicion.y - 15);
        } else if (estaHundiendo()) {
            // Flecha hacia abajo
            painter.drawLine(posicion.x + ancho/2, posicion.y + alto + 5,
                             posicion.x + ancho/2, posicion.y + alto + 15);
            painter.drawLine(posicion.x + ancho/2 - 3, posicion.y + alto + 12,
                             posicion.x + ancho/2, posicion.y + alto + 15);
            painter.drawLine(posicion.x + ancho/2 + 3, posicion.y + alto + 12,
                             posicion.x + ancho/2, posicion.y + alto + 15);
        }
    }
}

void Escombro::onColision(Entidad* otra) {
    if (!otra) return;

    // Interaccion con el jugador
    if (otra->getTipo() == TipoEntidad::JUGADOR) {
        // El jugador puede agarrarse a escombros flotantes
        if (estaFlotando()) {
            // Dar puntos por agarrarse a escombro flotante
        }
    }
}

// ========== METODOS ESPECIFICOS ==========

void Escombro::flotar() {
    // Forzar flotacion (cambiar a densidad baja)
    if (flotacion) {
        flotacion->setDensidadObjeto(0.5f);
    }
}

void Escombro::hundir() {
    // Forzar hundimiento (cambiar a densidad alta)
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
