#ifndef GESTORSONIDOS_H
#define GESTORSONIDOS_H

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QMap>
#include <QString>
#include <QUrl>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>

class GestorSonidos {
private:
    static GestorSonidos* instancia;

    // Reproductores
    QMediaPlayer* reproductorMusica;
    QAudioOutput* salidaMusica;

    QMap<QString, QMediaPlayer*> reproductoresEfectos;
    QMap<QString, QAudioOutput*> salidasEfectos;

    QString rutaBase;
    float volumenMusica;
    float volumenEfectos;
    bool musicaHabilitada;
    bool efectosHabilitados;

    GestorSonidos();
    QString obtenerRutaSonidos();

public:
    static GestorSonidos* obtenerInstancia();
    static void destruirInstancia();
    ~GestorSonidos();

    // Musica de fondo
    void reproducirMusica(const QString& nombre, bool loop = true);
    void pausarMusica();
    void reanudarMusica();
    void detenerMusica();

    // Efectos de sonido
    void reproducirEfecto(const QString& nombre);

    // Cargar sonidos
    void cargarTodosLosSonidos();
    bool cargarMusica(const QString& nombre, const QString& archivo);
    bool cargarEfecto(const QString& nombre, const QString& archivo);

    // Control de volumen
    void setVolumenMusica(float volumen); // 0.0 - 1.0
    void setVolumenEfectos(float volumen); // 0.0 - 1.0
    void toggleMusica();
    void toggleEfectos();

    // Getters
    float getVolumenMusica() const { return volumenMusica; }
    float getVolumenEfectos() const { return volumenEfectos; }
    bool getMusicaHabilitada() const { return musicaHabilitada; }
    bool getEfectosHabilitados() const { return efectosHabilitados; }
};

#endif // GESTORSONIDOS_H
