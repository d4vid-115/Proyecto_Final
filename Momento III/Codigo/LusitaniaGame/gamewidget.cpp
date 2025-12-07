#include "gamewidget.h"
#include "jugador.h"
#include <QPainter>
#include <QMessageBox>

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

    // Configurar widget
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(800, 600);

    // Obtener motor del juego
    motorJuego = MotorJuego::obtenerInstancia();

    // Crear HUD
    hud = new HUD();

    // Crear timer del bucle del juego
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &GameWidget::actualizar);

    // Configurar FPS
    int intervalo = 1000 / fpsObjetivo;
    timer->setInterval(intervalo);

    // Iniciar tiempo
    tiempoFrame.start();
}

GameWidget::~GameWidget() {
    // Detener timer
    if (timer) {
        timer->stop();
        delete timer;
        timer = nullptr;
    }

    // Eliminar HUD
    if (hud) {
        delete hud;
        hud = nullptr;
    }

    motorJuego = nullptr;
}

// ========== CONTROL DEL JUEGO ==========

void GameWidget::iniciarJuego(int nivel) {
    if (!motorJuego) return;

    // Cargar nivel
    motorJuego->cargarNivel(nivel);

    // Iniciar timer
    timer->start();
    juegoIniciado = true;

    // Resetear tiempo
    tiempoFrame.restart();
}

void GameWidget::pausarJuego() {
    if (motorJuego) {
        motorJuego->pausar();
    }
}

void GameWidget::reanudarJuego() {
    if (motorJuego) {
        motorJuego->reanudar();
    }
}

void GameWidget::reiniciarJuego() {
    if (motorJuego) {
        motorJuego->reiniciarNivel();
    }
}

void GameWidget::volverAlMenu() {
    timer->stop();
    juegoIniciado = false;

    if (motorJuego) {
        motorJuego->volverAlMenu();
    }

    teclasPresionadas.clear();
}

// ========== BUCLE PRINCIPAL ==========

void GameWidget::actualizar() {
    if (!motorJuego || !juegoIniciado) return;

    // Calcular delta time
    deltaTime = tiempoFrame.elapsed() / 1000.0f;
    tiempoFrame.restart();

    // Calcular FPS
    if (deltaTime > 0.0f) {
        fps = (int)(1.0f / deltaTime);
    }

    // Procesar input
    procesarInput();

    // Actualizar motor del juego
    motorJuego->actualizar(deltaTime);

    // Verificar condiciones de fin
    Nivel* nivel = motorJuego->getNivelActual();
    if (nivel) {
        if (nivel->estaCompletado()) {
            // ¡Nivel completado!
            timer->stop();

            int puntos = nivel->getPuntuacion();
            int nivelNum = motorJuego->getNumeroNivelActual();

            // Emitir señal de victoria
            emit victoria(puntos, nivelNum);

            // Cargar siguiente nivel
            motorJuego->siguienteNivel();

            if (motorJuego->getEstado() == EstadoJuego::VICTORIA) {
                // Victoria total - ya emitido arriba
                volverAlMenu();
            } else {
                // Continuar al siguiente nivel
                timer->start();
            }
        }

        if (nivel->haFallado()) {
            // Game Over
            timer->stop();

            int puntos = nivel->getPuntuacion();
            int nivelNum = motorJuego->getNumeroNivelActual();

            // Emitir señal de derrota
            emit derrota(puntos, nivelNum);
        }
    }

    // Redibujar
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

    // Manejar movimiento del jugador
    manejarMovimientoJugador();
}

void GameWidget::manejarMovimientoJugador() {
    if (!motorJuego) return;

    Nivel* nivel = motorJuego->getNivelActual();
    if (!nivel) return;

    Jugador* jugador = nivel->getJugador();
    if (!jugador) return;

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

    // Aplicar movimiento
    if (direccion.magnitud() > 0.0f) {
        jugador->mover(direccion, deltaTime);
    } else {
        jugador->detener();
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
        // Pantalla de inicio
        painter.fillRect(rect(), QColor(20, 20, 40));
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 24, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter,
                         "El Naufragio del Lusitania\n\n"
                         "Presiona M para Menu");
        return;
    }

    // Renderizar nivel actual
    Nivel* nivel = motorJuego->getNivelActual();
    if (nivel) {
        nivel->renderizar(painter);

        // Renderizar HUD
        if (hud) {
            hud->renderizar(painter, nivel, fps);
        }

        // Mensaje de pausa
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

    // Pasar al nivel para input adicional
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

    // Pasar al nivel
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
