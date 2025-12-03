#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>
#include <QElapsedTimer>
#include "motorjuego.h"
#include "hud.h"

// GameWidget - Widget principal del juego
// Maneja: bucle de juego, input, renderizado
class GameWidget : public QWidget {
    Q_OBJECT

private:
    // ===== MOTOR DEL JUEGO =====
    MotorJuego* motor;

    // ===== BUCLE DEL JUEGO =====
    QTimer* timerJuego;
    QElapsedTimer tiempoElapsed;
    float deltaTiempo;                  // Delta time en segundos
    int fps;                            // FPS objetivo (60)
    int intervaloMs;                    // Intervalo en ms (16.67ms ≈ 60 FPS)

    // ===== HUD =====
    HUD* hud;

    // ===== INPUT =====
    bool teclas[256];                   // Estado de teclas

    // Movimiento del jugador
    void actualizarMovimientoJugador(float dt);

    // ===== RENDERIZADO =====
    void renderizarJuego(QPainter& painter);
    void renderizarMenu(QPainter& painter);
    void renderizarPausa(QPainter& painter);
    void renderizarGameOver(QPainter& painter);
    void renderizarVictoria(QPainter& painter);

public:
    // ========== CONSTRUCTOR ==========
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

    // ========== CONTROL DEL JUEGO ==========
    void iniciarJuego();
    void pausarJuego();
    void reanudarJuego();
    void reiniciarNivel();
    void volverAlMenu();

    // ========== GETTERS =====
    int getFPS() const;

protected:
    // ========== EVENTOS Qt ==========
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private slots:
    // ========== SLOT DEL BUCLE =====
    void actualizar();
};

#endif // GAMEWIDGET_H
