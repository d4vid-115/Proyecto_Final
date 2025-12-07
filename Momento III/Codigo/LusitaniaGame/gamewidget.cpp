#include "gamewidget.h"
#include "jugador.h"
#include "nivel3submarino.h"
#include "gestorsonidos.h"
#include <QPainter>

// ========== CONSTRUCTOR ==========

GameWidget::GameWidget(QWidget* parent)
    : QWidget(parent),
    motorJuego(nullptr),
    timer(nullptr),
    deltaTime(0.0f),
    fps(0),
    fpsObjetivo(60),
    hud(nullptr),
    juegoIniciado(false) {

    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(800, 600);

    motorJuego = MotorJuego::obtenerInstancia();
    hud = new HUD();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWidget::actualizar);

    int intervalo = 1000 / fpsObjetivo;
    timer->setInterval(intervalo);

    tiempoFrame.start();
}

GameWidget::~GameWidget() {
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }

    if (hud) {
        delete hud;
        hud = nullptr;
    }

    motorJuego = nullptr;
}

// ========== CONTROL DEL JUEGO ==========

void GameWidget::iniciarJuego(int nivel) {
    if (!motorJuego) return;

    motorJuego->cargarNivel(nivel);

    timer->start();
    juegoIniciado = true;

    tiempoFrame.restart();
}

void GameWidget::pausarJuego() {
    if (motorJuego) {
        motorJuego->pausar();
        GestorSonidos::obtenerInstancia()->pausarMusica();
    }
}

void GameWidget::reanudarJuego() {
    if (motorJuego) {
        motorJuego->reanudar();
        GestorSonidos::obtenerInstancia()->reanudarMusica();
    }
}

void GameWidget::reiniciarJuego() {
    if (motorJuego) {
        motorJuego->reiniciarNivel();

        int nivelActual = motorJuego->getNumeroNivelActual();
        QString musicaNivel = QString("nivel%1").arg(nivelActual);
        GestorSonidos::obtenerInstancia()->reproducirMusica(musicaNivel);
    }
}

void GameWidget::volverAlMenu() {
    timer->stop();
    juegoIniciado = false;

    if (motorJuego) {
        motorJuego->volverAlMenu();
    }

    teclasPresionadas.clear();
    GestorSonidos::obtenerInstancia()->reproducirMusica("menu");
}

// ========== BUCLE PRINCIPAL ==========

void GameWidget::actualizar() {
    if (!motorJuego || !juegoIniciado) return;

    deltaTime = tiempoFrame.elapsed() / 1000.0f;
    tiempoFrame.restart();

    if (deltaTime > 0.0f) {
        fps = (int)(1.0f / deltaTime);
    }

    procesarInput();

    motorJuego->actualizar(deltaTime);

    Nivel* nivel = motorJuego->getNivelActual();
    if (nivel) {
        if (nivel->estaCompletado()) {
            timer->stop();

            int puntos = nivel->getPuntuacion();
            int nivelNum = motorJuego->getNumeroNivelActual();

            emit victoria(puntos, nivelNum);

            motorJuego->siguienteNivel();

            if (motorJuego->getEstado() == EstadoJuego::VICTORIA) {
                volverAlMenu();
            } else {
                int siguienteNivel = motorJuego->getNumeroNivelActual();
                QString musicaNivel = QString("nivel%1").arg(siguienteNivel);
                GestorSonidos::obtenerInstancia()->reproducirMusica(musicaNivel);

                timer->start();
            }
        }

        // ===== VERIFICAR DERROTA CON DELAY PARA ANIMACION =====
        if (nivel->haFallado()) {
            Jugador* jugador = nivel->getJugador();

            // Si es nivel 3 y el jugador esta en animacion de muerte, esperar
            Nivel3Submarino* nivel3 = dynamic_cast<Nivel3Submarino*>(nivel);
            if (nivel3 && jugador && jugador->estaEnAnimacionMuerte()) {
                // Esperar 3 segundos para que termine la animacion
                if (jugador->getTiempoMuerte() >= 3.0f) {
                    timer->stop();
                    int puntos = nivel->getPuntuacion();
                    int nivelNum = motorJuego->getNumeroNivelActual();
                    emit derrota(puntos, nivelNum);
                }
            } else {
                // Otros niveles o muerte inmediata
                timer->stop();
                int puntos = nivel->getPuntuacion();
                int nivelNum = motorJuego->getNumeroNivelActual();
                emit derrota(puntos, nivelNum);
            }
        }
    }

    update();
}

// ========== PROCESAMIENTO DE INPUT ==========

void GameWidget::procesarInput() {
    if (!motorJuego) return;

    Nivel* nivel = motorJuego->getNivelActual();
    if (!nivel) return;

    Jugador* jugador = nivel->getJugador();
    if (!jugador) return;

    // Pausa
    if (teclasPresionadas.contains(Qt::Key_P)) {
        if (motorJuego->getEstado() == EstadoJuego::JUGANDO) {
            pausarJuego();
        } else if (motorJuego->getEstado() == EstadoJuego::PAUSADO) {
            reanudarJuego();
        }
        teclasPresionadas.remove(Qt::Key_P);
    }

    manejarMovimientoJugador();
}

void GameWidget::manejarMovimientoJugador() {
    if (!motorJuego) return;

    Nivel* nivel = motorJuego->getNivelActual();
    if (!nivel) return;

    Jugador* jugador = nivel->getJugador();
    if (!jugador) return;

    // No permitir movimiento si esta muriendo
    if (jugador->estaEnAnimacionMuerte()) {
        return;
    }

    // Verificar si los controles estan bloqueados (Nivel 3)
    Nivel3Submarino* nivel3 = dynamic_cast<Nivel3Submarino*>(nivel);
    if (nivel3 && nivel3->estanControlsBloqueados()) {
        // Controles bloqueados por vortice - NO permitir input
        return;
    }

    // Vector de direccion
    Vector2D direccion(0, 0);

    // WASD / Flechas para movimiento
    if (teclasPresionadas.contains(Qt::Key_W) ||
        teclasPresionadas.contains(Qt::Key_Up)) {
        direccion.y -= 1.0f;
    }

    if (teclasPresionadas.contains(Qt::Key_S) ||
        teclasPresionadas.contains(Qt::Key_Down)) {
        direccion.y += 1.0f;
    }

    if (teclasPresionadas.contains(Qt::Key_A) ||
        teclasPresionadas.contains(Qt::Key_Left)) {
        direccion.x -= 1.0f;
    }

    if (teclasPresionadas.contains(Qt::Key_D) ||
        teclasPresionadas.contains(Qt::Key_Right)) {
        direccion.x += 1.0f;
    }

    // ===== PARA NIVEL 3: AGREGAR A LA VELOCIDAD EN LUGAR DE SOBRESCRIBIR =====
    if (nivel3) {
        if (direccion.magnitud() > 0.0f) {
            Vector2D direccionNorm = direccion.normalizado();
            Vector2D velocidadDeseada = direccionNorm * jugador->getVelocidadBase();

            // En nivel 3, sumamos a la velocidad actual (para que los vortices afecten)
            Vector2D velActual = jugador->getVelocidad();
            Vector2D nuevaVel = velActual * 0.85f + velocidadDeseada * 0.15f;  // Blend suave
            jugador->setVelocidad(nuevaVel);
        } else {
            // Si no hay input, aplicar friccion
            Vector2D velActual = jugador->getVelocidad();
            jugador->setVelocidad(velActual * 0.92f);
        }
    } else {
        // Otros niveles: comportamiento normal
        if (direccion.magnitud() > 0.0f) {
            jugador->mover(direccion, deltaTime);
        } else {
            jugador->detener();
        }
    }

    // Habilidad especial (Shift)
    if (teclasPresionadas.contains(Qt::Key_Shift)) {
        jugador->usarHabilidad();
    }
}

// ========== EVENTOS DE QT ==========

void GameWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!juegoIniciado || !motorJuego) {
        painter.fillRect(rect(), QColor(20, 20, 40));
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 24, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter,
                         "El Naufragio del Lusitania\n\nPresiona M para Menu");
        return;
    }

    Nivel* nivel = motorJuego->getNivelActual();
    if (nivel) {
        nivel->renderizar(painter);

        if (hud) {
            hud->renderizar(painter, nivel, fps);
        }

        if (motorJuego->estaPausado()) {
            painter.fillRect(rect(), QColor(0, 0, 0, 150));
            painter.setPen(Qt::white);
            painter.setFont(QFont("Arial", 36, QFont::Bold));
            painter.drawText(rect(), Qt::AlignCenter, "PAUSA\n\nPresiona P para continuar");
        }
    }
}

void GameWidget::keyPressEvent(QKeyEvent* event) {
    if (event->isAutoRepeat()) return;

    int tecla = event->key();
    teclasPresionadas.insert(tecla);

    if (motorJuego) {
        Nivel* nivel = motorJuego->getNivelActual();
        if (nivel) {
            nivel->manejarInput(tecla, true);
        }
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent* event) {
    if (event->isAutoRepeat()) return;

    int tecla = event->key();
    teclasPresionadas.remove(tecla);

    if (motorJuego) {
        Nivel* nivel = motorJuego->getNivelActual();
        if (nivel) {
            nivel->manejarInput(tecla, false);
        }
    }
}

// ========== GETTERS ==========

int GameWidget::getFPS() const {
    return fps;
}

float GameWidget::getDeltaTime() const {
    return deltaTime;
}
