#include "nivel1oceano.h"
#include <QColor>
#include <cstdlib>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ========== CONSTRUCTOR ==========

Nivel1Oceano::Nivel1Oceano()
    : Nivel(),
    posicionBarco(400, 200),
    oscilacionBarco(nullptr),
    tiempoSpawn(0.0f),
    intervaloSpawn(20.0f),
    maxSubmarinos(3),
    submarinosDestruidos(0),
    torpedosEvadidos(0) {

    // Configurar nivel
    anchoNivel = 2400;
    altoNivel = 1200;
    tiempoLimite = 180.0f;  // 3 minutos

    // Actualizar limites de camara
    camara->setLimites(Vector2D(0, 0), Vector2D(anchoNivel, altoNivel));
    camara->setTieneScroll(true);

    // Inicializar
    inicializar();
}

Nivel1Oceano::~Nivel1Oceano() {
    // Limpiar oscilacion del barco
    if (oscilacionBarco) {
        delete oscilacionBarco;
        oscilacionBarco = nullptr;
    }

    // Los submarinos y torpedos son eliminados por motorFisica
    submarinos.clear();
    torpedos.clear();
}

// ========== INICIALIZACION ==========

void Nivel1Oceano::inicializar() {
    // Crear jugador (el jugador controla un barco pequeño)
    jugador = new Jugador(Vector2D(400, 500));
    jugador->setDimensiones(48, 32);
    motorFisica->agregarEntidad(jugador);

    // Configurar fisica del barco Lusitania (oscilacion)
    oscilacionBarco = new OsciladorArmonico(15.0f, 2.0f * M_PI / 3.0f, 0.0f);
    oscilacionBarco->setPosicionBase(posicionBarco.y);

    // Spawnear submarinos iniciales
    spawnearSubmarino();

    // Configurar gravedad (desactivada en este nivel)
    motorFisica->setGravedadActiva(false);
}

// ========== ACTUALIZACION ==========

void Nivel1Oceano::actualizar(float dt) {
    if (completado || fallado) return;

    // Actualizar tiempo
    tiempoTranscurrido += dt;

    // Actualizar oscilacion del barco Lusitania
    if (oscilacionBarco) {
        // El barco oscila pero no se mueve (es visual)
        posicionBarco.y = oscilacionBarco->calcularDesplazamiento(tiempoTranscurrido);
    }

    // Actualizar fisica
    motorFisica->actualizar(dt);

    // Actualizar submarinos
    actualizarSubmarinos(dt);

    // Actualizar torpedos
    actualizarTorpedos(dt);

    // Verificar colisiones de torpedos
    verificarColisionesTorpedos();

    // Spawning de submarinos
    tiempoSpawn += dt;
    if (tiempoSpawn >= intervaloSpawn) {
        if ((int)submarinos.size() < maxSubmarinos) {
            spawnearSubmarino();
        }
        tiempoSpawn = 0.0f;
    }

    // Actualizar camara (seguir al jugador)
    if (jugador) {
        camara->seguirObjetivo(jugador->getPosicion(), dt);
    }

    // Verificar condiciones
    if (verificarVictoria()) {
        completado = true;
    }

    if (verificarDerrota()) {
        fallado = true;
    }
}

void Nivel1Oceano::actualizarSubmarinos(float dt) {
    // Actualizar cada submarino y hacer que disparen
    auto it = submarinos.begin();

    while (it != submarinos.end()) {
        Submarino* sub = *it;

        if (!sub || !sub->estaActivo()) {
            // Submarino destruido
            it = submarinos.erase(it);
            submarinosDestruidos++;
            agregarPuntos(100); // 100 puntos por submarino
            continue;
        }

        // Configurar jugador como objetivo
        sub->setObjetivo(jugador);

        // Verificar si debe disparar
        if (sub->puedeDisparar() && sub->getEstado() == EstadoSubmarino::ATACANDO) {
            Torpedo* torpedo = sub->dispararTorpedo();
            if (torpedo) {
                motorFisica->agregarEntidad(torpedo);
                torpedos.push_back(torpedo);
            }
        }

        ++it;
    }
}

void Nivel1Oceano::actualizarTorpedos(float dt) {
    // Limpiar torpedos inactivos
    auto it = torpedos.begin();

    while (it != torpedos.end()) {
        Torpedo* torpedo = *it;

        if (!torpedo || !torpedo->estaActivo()) {
            it = torpedos.erase(it);
            torpedosEvadidos++;
            agregarPuntos(10); // 10 puntos por evadir
        } else {
            ++it;
        }
    }
}

void Nivel1Oceano::verificarColisionesTorpedos() {
    // Verificar si los torpedos golpean al jugador
    for (auto* torpedo : torpedos) {
        if (torpedo && torpedo->estaActivo() && jugador) {
            if (torpedo->colisionaCon(jugador)) {
                // Jugador recibe daño
                jugador->recibirDanio(25.0f);

                // Destruir torpedo
                torpedo->destruir();
            }
        }
    }
}

void Nivel1Oceano::spawnearSubmarino() {
    // Spawnear submarino en posicion aleatoria fuera de pantalla
    float x = (rand() % 2 == 0) ? -100.0f : anchoNivel + 100.0f;
    float y = 100.0f + (rand() % 400);

    Submarino* sub = new Submarino(Vector2D(x, y));
    sub->setObjetivo(jugador);
    sub->setProfundidad(y);

    motorFisica->agregarEntidad(sub);
    submarinos.push_back(sub);
}

// ========== RENDERIZADO ==========

void Nivel1Oceano::renderizar(QPainter& painter) {
    if (!camara) return;

    painter.save();

    // ===== FONDO: OCEANO =====
    painter.fillRect(0, 0, 800, 600, QColor(0, 50, 100));

    // Dibujar olas decorativas (animadas)
    painter.setPen(QPen(QColor(0, 100, 150, 150), 2));
    for (int y = 50; y < 600; y += 100) {
        int offset = (int)(tiempoTranscurrido * 50) % 100;
        for (int x = -offset; x < 800; x += 100) {
            painter.drawArc(x, y, 100, 50, 0, 180 * 16);
        }
    }

    // ===== RENDERIZAR ENTIDADES (con camara) =====
    Vector2D offsetCamara = camara->getPosicion();
    painter.translate(-offsetCamara.x, -offsetCamara.y);

    // Dibujar barco Lusitania (grande, en el fondo)
    painter.setBrush(QColor(100, 50, 0));
    painter.setPen(Qt::black);
    painter.drawRect(posicionBarco.x - 100, posicionBarco.y - 40, 200, 80);

    // Chimeneas del Lusitania
    painter.setBrush(QColor(50, 25, 0));
    painter.drawRect(posicionBarco.x - 60, posicionBarco.y - 60, 20, 20);
    painter.drawRect(posicionBarco.x - 20, posicionBarco.y - 60, 20, 20);
    painter.drawRect(posicionBarco.x + 20, posicionBarco.y - 60, 20, 20);

    // Renderizar todas las entidades del motor de fisica
    for (auto* entidad : motorFisica->getEntidades()) {
        if (entidad && entidad->estaActivo()) {
            entidad->renderizar(painter);
        }
    }

    painter.restore();

    // ===== HUD (sin offset de camara) =====
    painter.setPen(Qt::white);
    painter.drawText(10, 20, QString("Tiempo: %1 / %2 s")
                                 .arg((int)tiempoTranscurrido)
                                 .arg((int)tiempoLimite));
    painter.drawText(10, 40, QString("Puntos: %1").arg(puntuacion));
    painter.drawText(10, 60, QString("Submarinos destruidos: %1").arg(submarinosDestruidos));
    painter.drawText(10, 80, QString("Vidas: %1").arg(jugador ? jugador->getVidas() : 0));
}

// ========== REINICIAR ==========

void Nivel1Oceano::reiniciar() {
    // Limpiar todo
    limpiar();

    // Reiniciar variables
    completado = false;
    fallado = false;
    tiempoTranscurrido = 0.0f;
    puntuacion = 0;
    submarinosDestruidos = 0;
    torpedosEvadidos = 0;
    tiempoSpawn = 0.0f;

    submarinos.clear();
    torpedos.clear();

    // Reinicializar
    motorFisica = new MotorFisica();
    camara = new Camara(800, 600);
    camara->setLimites(Vector2D(0, 0), Vector2D(anchoNivel, altoNivel));

    inicializar();
}

// ========== CONDICIONES ==========

bool Nivel1Oceano::verificarVictoria() {
    // Victoria: sobrevivir 3 minutos
    return tiempoTranscurrido >= tiempoLimite;
}

bool Nivel1Oceano::verificarDerrota() {
    // Derrota: quedarse sin vidas
    return jugador && jugador->estaMuerto();
}

// ========== INPUT ==========

void Nivel1Oceano::manejarInput(int tecla, bool presionada) {
    if (!jugador) return;

    // Este metodo sera llamado por GameWidget
    (void)tecla;
    (void)presionada;
}
