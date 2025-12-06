#ifndef GESTORSPRITES_H
#define GESTORSPRITES_H

#include <QPixmap>
#include <QMap>
#include <QString>
#include <QDebug>

class GestorSprites {
private:
    static GestorSprites* instancia;
    QMap<QString, QPixmap> sprites;
    QString rutaBase;

    GestorSprites();

    void cargarCarpetaNivel(int nivel, const QString& nombreCarpeta, const QString& prefijo);

public:
    static GestorSprites* obtenerInstancia();
    static void destruirInstancia();

    ~GestorSprites();

    bool cargarSprite(const QString& nombre, const QString& ruta);
    QPixmap getSprite(const QString& nombre);
    bool tieneSprite(const QString& nombre);

    void cargarTodosLosSprites();
    void setRutaBase(const QString& ruta);
};

#endif // GESTORSPRITES_H
