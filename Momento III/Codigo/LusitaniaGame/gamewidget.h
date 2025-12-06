#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>
#include <QElapsedTimer>
#include <QSet>
#include "motorjuego.h"
#include "hud.h"

class GameWidget : public QWidget {
    Q_OBJECT

private:
    // ===== MOTOR DEL JUEGO =====
    MotorJuego* motorJuego;

    // ===== BUCLE DEL JUEGO =====
    QTimer* timer;
    QElapsedTimer tiempoFrame;
    float deltaTime;
    int fps;
    int fpsObjetivo;

    // ===== HUD =====
    HUD* hud;

    // ===== INPUT =====
    QSet<int> teclasPresionadas;

    // ===== ESTADO =====
    bool juegoIniciado;

    // Metodos privados
    void procesarInput();
    void manejarMovimientoJugador();

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget();

    // ========== CONTROL DEL JUEGO ==========
    void iniciarJuego(int nivel);
    void pausarJuego();
    void reanudarJuego();
    void reiniciarJuego();
    void volverAlMenu();

    // ========== GETTERS =====
    int getFPS() const;
    float getDeltaTime() const;

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
