#ifndef GESTORSPRITES_H
#define GESTORSPRITES_H

#include <QPixmap>
#include <QMap>
#include <QString>
#include <QDebug>
#include <QVector>
#include <QDir>
#include <QCoreApplication>

// Estructura para animaciones
struct Animacion {
    QVector<QPixmap> frames;
    int frameActual;
    float tiempoFrame;      // Tiempo entre frames
    float tiempoAcumulado;
    bool loop;

    Animacion() : frameActual(0), tiempoFrame(0.1f), tiempoAcumulado(0.0f), loop(true) {}
};

class GestorSprites {
private:
    static GestorSprites* instancia;
    QMap<QString, QPixmap> sprites;
    QMap<QString, Animacion> animaciones;
    QString rutaBase;

    GestorSprites();

public:
    static GestorSprites* obtenerInstancia();
    static void destruirInstancia();

    ~GestorSprites();

    // Cargar sprites individuales
    bool cargarSprite(const QString& nombre, const QString& ruta);
    QPixmap getSprite(const QString& nombre);
    bool tieneSprite(const QString& nombre);

    // Cargar animaciones (sprites secuenciales)
    bool cargarAnimacion(const QString& nombre, const QString& rutaCarpeta,
                         const QString& prefijo, int numFrames, float tiempoFrame = 0.1f);
    QPixmap getFrameAnimacion(const QString& nombre, float dt);
    void reiniciarAnimacion(const QString& nombre);

    // Cargar todos los assets del juego
    void cargarTodosLosSprites();
    void setRutaBase(const QString& ruta);

    // Obtener ruta de assets (relativa al ejecutable)
    QString obtenerRutaAssets();

private:
    void cargarSpritesNivel1();
    void cargarSpritesNivel2();
    void cargarSpritesNivel3();
    void cargarSpritesUI();
};

#endif // GESTORSPRITES_H
