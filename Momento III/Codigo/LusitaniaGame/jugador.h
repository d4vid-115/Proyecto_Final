#ifndef JUGADOR_H
#define JUGADOR_H

#include "entidad.h"
#include <QPainter>
#include <QPixmap>

// Clase de animacion para sprite sheets
class AnimacionSprite {
private:
    QPixmap spriteSheet;
    int frameAncho;
    int frameAlto;
    int numFrames;
    int frameActual;
    float duracionCicloCompleto;
    float tiempoAcumulado;

public:
    AnimacionSprite() : frameAncho(64), frameAlto(64), numFrames(1), frameActual(0),
        duracionCicloCompleto(1.0f), tiempoAcumulado(0.0f) {}

    // ciclosSegundo = cuantas veces se repite la animacion completa por segundo
    AnimacionSprite(const QPixmap& sheet, int ancho, int alto, int frames, float ciclosSegundo)
        : spriteSheet(sheet), frameAncho(ancho), frameAlto(alto), numFrames(frames),
        frameActual(0), duracionCicloCompleto(1.0f / ciclosSegundo), tiempoAcumulado(0.0f) {}

    void actualizar(float dt) {
        tiempoAcumulado += dt;

        // Calcular que frame mostrar segun el tiempo transcurrido
        float tiempoPorFrame = duracionCicloCompleto / numFrames;
        frameActual = (int)(tiempoAcumulado / tiempoPorFrame) % numFrames;
    }

    QPixmap getFrameActual() const {
        if (spriteSheet.isNull()) return QPixmap();
        int x = frameActual * frameAncho;
        return spriteSheet.copy(x, 0, frameAncho, frameAlto);
    }

    void reiniciar() {
        frameActual = 0;
        tiempoAcumulado = 0.0f;
    }
};

// Clase Jugador
class Jugador : public Entidad {
private:
    float salud;
    float saludMaxima;
    int vidas;
    int puntuacion;
    float velocidadBase;
    float oxigeno;
    float oxigenoMaximo;

    // Animaciones
    AnimacionSprite animIdle;
    AnimacionSprite animSwim;
    AnimacionSprite animHurt;
    AnimacionSprite animDeath;

    enum class EstadoAnimacion {
        IDLE,
        SWIM,
        HURT,
        DEATH
    };

    EstadoAnimacion estadoActual;
    float tiempoMuerte;  // Contador para animacion de muerte

    // Habilidad especial
    bool habilidadActiva;
    float tiempoHabilidad;
    float cooldownHabilidad;
    float tiempoCooldown;

    // Invencibilidad temporal
    bool invencible;
    float tiempoInvencibilidad;

    void cargarAnimaciones();

public:
    // ========== CONSTRUCTORES ==========
    Jugador();
    Jugador(const Vector2D& pos);
    ~Jugador();

    // ========== METODOS HEREDADOS ==========
    void actualizar(float dt) override;
    void renderizar(QPainter& painter) override;
    void renderizarNadador(QPainter& painter);
    void onColision(Entidad* otra) override;

    // ========== MOVIMIENTO ==========
    void mover(const Vector2D& direccion, float dt);
    void detener();

    // ========== SALUD Y DAÑO ==========
    void recibirDanio(float cantidad);
    void curar(float cantidad);
    bool estaMuerto() const;
    bool estaEnAnimacionMuerte() const { return estadoActual == EstadoAnimacion::DEATH; }
    float getTiempoMuerte() const { return tiempoMuerte; }

    // ========== HABILIDAD ESPECIAL ==========
    void usarHabilidad();
    bool puedeUsarHabilidad() const;

    // ========== VIDAS Y PUNTUACION ==========
    void perderVida();
    void ganarVida();
    void agregarPuntos(int puntos);

    // ========== GETTERS ==========
    float getSalud() const;
    float getSaludMaxima() const;
    int getVidas() const;
    int getPuntuacion() const;
    float getVelocidadBase() const;
    float getOxigeno() const { return oxigeno; }
    float getOxigenoMaximo() const { return oxigenoMaximo; }
    bool estaInvencible() const;

    // ========== SETTERS ==========
    void setSalud(float s);
    void setVidas(int v);
    void setOxigeno(float o) { oxigeno = o; }
};

#endif // JUGADOR_H
