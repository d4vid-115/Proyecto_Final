#include "nivel3submarino.h"
#include "gestorsprites.h"
#include <QColor>
#include <cstdlib>

// ========== CONSTRUCTOR ==========

Nivel3Submarino::Nivel3Submarino()
    : Nivel(),
    profundidadInicial(3000.0f),
    profundidadObjetivo(0.0f),
    profundidadActual(3000.0f),
    tiempoSpawnVortice(0.0f),
    intervaloSpawnVortice(12.0f),
    tiempoSpawnEscombro(0.0f),
    intervaloSpawnEscombro(2.5f),
    maxVortices(4),
    escombrosEvitados(0),
    vorticesEvitados(0) {

    // Configurar nivel LARGO
    anchoNivel = 1600;
    altoNivel = 3500;
    tiempoLimite = 0.0f;

    // Camara dinamica que sigue al jugador
    camara->setLimites(Vector2D(0, 0), Vector2D(anchoNivel, altoNivel));
    camara->setTieneScroll(true);

    // Inicializar
    inicializar();
}

Nivel3Submarino::~Nivel3Submarino() {
    vortices.clear();
    escombros.clear();
}

// ========== INICIALIZACION ==========

void Nivel3Submarino::inicializar() {
    // Crear jugador en el FONDO
    jugador = new Jugador(Vector2D(800, profundidadInicial));
    jugador->setDimensiones(48, 32);
    jugador->setOxigeno(150.0f);
    motorFisica->agregarEntidad(jugador);

    // Spawnear escombros distribuidos verticalmente
    for (int i = 0; i < 40; i++) {
        spawnearEscombro();
    }

    // Spawnear vortices iniciales
    spawnearVortice();
    spawnearVortice();

    // Configurar gravedad (agua = flotabilidad)
    motorFisica->setGravedadActiva(true);
    motorFisica->setGravedad(1.2f);
}

// ========== ACTUALIZACION ==========

void Nivel3Submarino::actualizar(float dt) {
    if (completado || fallado) return;

    // Actualizar tiempo
    tiempoTranscurrido += dt;

    // Deplecionar oxigeno del jugador
    if (jugador) {
        float oxigenoActual = jugador->getOxigeno();
        oxigenoActual -= dt * 3.0f;

        if (oxigenoActual < 0.0f) {
            oxigenoActual = 0.0f;
            // Daño por falta de oxigeno
            jugador->recibirDanio(dt * 10.0f);
        }

        jugador->setOxigeno(oxigenoActual);
    }

    // Actualizar fisica
    motorFisica->actualizar(dt);

    // Actualizar vortices
    actualizarVortices(dt);

    // Actualizar escombros
    actualizarEscombros(dt);

    // Aplicar fuerzas de vortice
    aplicarFuerzasVortice();

    // Spawning de vortices
    tiempoSpawnVortice += dt;
    if (tiempoSpawnVortice >= intervaloSpawnVortice) {
        if ((int)vortices.size() < maxVortices) {
            spawnearVortice();
        }
        tiempoSpawnVortice = 0.0f;
    }

    // Spawning de escombros
    tiempoSpawnEscombro += dt;
    if (tiempoSpawnEscombro >= intervaloSpawnEscombro) {
        spawnearEscombro();
        tiempoSpawnEscombro = 0.0f;
    }

    // Actualizar profundidad actual
    if (jugador) {
        profundidadActual = jugador->getPosicion().y;

        // CAMARA DINAMICA: Seguir al jugador con smooth scrolling
        Vector2D posJugador = jugador->getPosicion();
        camara->seguirObjetivo(posJugador, dt);
    }

    // Verificar condiciones
    if (verificarVictoria()) {
        completado = true;
    }

    if (verificarDerrota()) {
        fallado = true;
    }
}

void Nivel3Submarino::actualizarVortices(float /*dt*/) {
    auto it = vortices.begin();

    while (it != vortices.end()) {
        Vortice* vortice = *it;

        if (!vortice || !vortice->estaActivo()) {
            it = vortices.erase(it);
            vorticesEvitados++;
            agregarPuntos(50);
        } else {
            ++it;
        }
    }
}

void Nivel3Submarino::actualizarEscombros(float dt) {
    auto it = escombros.begin();

    while (it != escombros.end()) {
        Escombro* escombro = *it;

        if (!escombro || !escombro->estaActivo()) {
            it = escombros.erase(it);
            escombrosEvitados++;
            agregarPuntos(10);
        } else {
            // Verificar colision con jugador
            if (jugador && escombro->colisionaCon(jugador)) {
                // Aplicar efecto segun tipo
                Vector2D velJugador = jugador->getVelocidad();

                switch (escombro->getTipo()) {
                case TipoEscombro::MADERA: {
                    // Flotacion: impulso hacia arriba
                    velJugador.y -= 50.0f * dt;
                    jugador->setVelocidad(velJugador);
                    break;
                }

                case TipoEscombro::METAL_PESADO: {
                    // Hunde: impulso hacia abajo
                    velJugador.y += 80.0f * dt;
                    jugador->setVelocidad(velJugador);
                    jugador->recibirDanio(5.0f);
                    break;
                }

                case TipoEscombro::SALVAVIDAS: {
                    // Restaura oxigeno
                    float oxigenoActual = jugador->getOxigeno();
                    jugador->setOxigeno(oxigenoActual + 30.0f);
                    agregarPuntos(50);
                    escombro->destruir();
                    break;
                }

                case TipoEscombro::NEUTRO: {
                    // No hace nada especial, solo colision fisica
                    break;
                }
                }
            }
            ++it;
        }
    }
}

void Nivel3Submarino::aplicarFuerzasVortice() {
    if (!jugador) return;

    for (auto* vortice : vortices) {
        if (vortice && vortice->estaActivo()) {
            // Aplicar al jugador
            vortice->aplicarFuerzaA(jugador);

            // Aplicar a escombros
            for (auto* escombro : escombros) {
                if (escombro && escombro->estaActivo()) {
                    vortice->aplicarFuerzaA(escombro);
                }
            }
        }
    }
}

void Nivel3Submarino::spawnearVortice() {
    // Spawnear en rango visible o cerca
    float x = 200.0f + (rand() % 1200);

    // Spawnear en un rango vertical amplio
    float y = profundidadActual + (rand() % 1000) - 500;

    // Mantener dentro de limites
    if (y < 100) y = 100;
    if (y > altoNivel - 100) y = altoNivel - 100;

    Vortice* vortice = new Vortice(Vector2D(x, y), 120.0f, 15.0f);

    motorFisica->agregarEntidad(vortice);
    vortices.push_back(vortice);
}

void Nivel3Submarino::spawnearEscombro() {
    // Tipo aleatorio con probabilidades
    TipoEscombro tipo;
    int tipoRand = rand() % 10;

    if (tipoRand < 4) tipo = TipoEscombro::MADERA;  // 40%
    else if (tipoRand < 7) tipo = TipoEscombro::METAL_PESADO;  // 30%
    else if (tipoRand < 9) tipo = TipoEscombro::SALVAVIDAS;  // 20%
    else tipo = TipoEscombro::NEUTRO;  // 10%

    // Posicion cerca del jugador
    float x = 100.0f + (rand() % 1400);
    float y = profundidadActual + (rand() % 800) - 400;

    if (y < 50) y = 50;
    if (y > altoNivel - 50) y = altoNivel - 50;

    Escombro* escombro = new Escombro(Vector2D(x, y), tipo);

    motorFisica->agregarEntidad(escombro);
    escombros.push_back(escombro);
}

// ========== RENDERIZADO ==========

void Nivel3Submarino::renderizar(QPainter& painter) {
    if (!camara) return;

    painter.save();

    GestorSprites* gestor = GestorSprites::obtenerInstancia();

    // ===== FONDO: OCEANO PROFUNDO =====
    QPixmap fondoProfundo = gestor->getSprite("oceano_profundo");

    if (!fondoProfundo.isNull()) {
        QPixmap fondoEscalado = fondoProfundo.scaled(800, 600,
                                                     Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap(0, 0, fondoEscalado);
    } else {
        // Fallback: degradado oscuro
        QLinearGradient gradiente(0, 0, 0, 600);
        gradiente.setColorAt(0, QColor(0, 20, 40));
        gradiente.setColorAt(1, QColor(0, 5, 15));
        painter.fillRect(0, 0, 800, 600, gradiente);
    }

    // ===== OFFSET DE CAMARA =====
    Vector2D offsetCamara = camara->getPosicion();
    painter.translate(-offsetCamara.x, -offsetCamara.y);

    // ===== SUPERFICIE (meta) =====
    painter.setBrush(QColor(100, 200, 255, 180));
    painter.setPen(QPen(QColor(255, 255, 100), 4));
    painter.drawRect(0, -50, anchoNivel, 100);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 24, QFont::Bold));
    painter.drawText(600, 20, "¡SUPERFICIE!");

    // ===== RENDERIZAR ENTIDADES =====
    for (auto* entidad : motorFisica->getEntidades()) {
        if (entidad && entidad->estaActivo()) {
            entidad->renderizar(painter);
        }
    }

    painter.restore();
}

// ========== REINICIAR ==========

void Nivel3Submarino::reiniciar() {
    limpiar();

    completado = false;
    fallado = false;
    tiempoTranscurrido = 0.0f;
    puntuacion = 0;
    profundidadActual = profundidadInicial;
    escombrosEvitados = 0;
    vorticesEvitados = 0;
    tiempoSpawnVortice = 0.0f;
    tiempoSpawnEscombro = 0.0f;

    vortices.clear();
    escombros.clear();

    motorFisica = new MotorFisica();
    camara = new Camara(800, 600);
    camara->setLimites(Vector2D(0, 0), Vector2D(anchoNivel, altoNivel));

    inicializar();
}

// ========== CONDICIONES ==========

bool Nivel3Submarino::verificarVictoria() {
    return jugador && jugador->getPosicion().y < 50.0f;
}

bool Nivel3Submarino::verificarDerrota() {
    return jugador && jugador->estaMuerto();
}

void Nivel3Submarino::manejarInput(int /*tecla*/, bool /*presionada*/) {
    // Metodo reservado para input adicional especifico del nivel
    // Actualmente no se usa porque el input se maneja en GameWidget
}
