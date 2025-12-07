#ifndef CONFIGURACIONSPRITES_H
#define CONFIGURACIONSPRITES_H

#include <QString>
#include <QMap>
#include <QDebug>

// Configuracion de mapeo de sprites
// Relaciona nombres de sprites con sus rutas de archivos
class ConfiguracionSprites {
private:
    QMap<QString, QString> mapaSprites;

public:
    ConfiguracionSprites();
    ~ConfiguracionSprites();

    // Obtener ruta de un sprite por nombre
    QString obtenerRuta(const QString& nombre) const;

    // Verificar si existe un sprite
    bool existe(const QString& nombre) const;
};

#endif // CONFIGURACIONSPRITES_H
