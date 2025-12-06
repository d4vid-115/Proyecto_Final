#include "gestorsprites.h"
#include <QDir>
#include <QFileInfo>

GestorSprites* GestorSprites::instancia = nullptr;

GestorSprites::GestorSprites() {
    rutaBase = "C:/Users/David/Desktop/Proyecto_Final_Info_II_2025_2/Proyecto_Final/Momento III/Sprites/";
}

GestorSprites::~GestorSprites() {
    sprites.clear();
}

GestorSprites* GestorSprites::obtenerInstancia() {
    if (!instancia) {
        instancia = new GestorSprites();
    }
    return instancia;
}

void GestorSprites::destruirInstancia() {
    if (instancia) {
        delete instancia;
        instancia = nullptr;
    }
}

void GestorSprites::setRutaBase(const QString& ruta) {
    rutaBase = ruta;
}

bool GestorSprites::cargarSprite(const QString& nombre, const QString& ruta) {
    QFileInfo checkFile(ruta);

    if (!checkFile.exists() || !checkFile.isFile()) {
        qDebug() << " Archivo no existe:" << ruta;
        return false;
    }

    QPixmap pixmap(ruta);

    if (pixmap.isNull()) {
        qDebug() << " Error al cargar sprite:" << ruta;
        return false;
    }

    sprites[nombre] = pixmap;
    qDebug() << " Sprite cargado:" << nombre;
    return true;
}

QPixmap GestorSprites::getSprite(const QString& nombre) {
    if (sprites.contains(nombre)) {
        return sprites[nombre];
    }

    qDebug() << " Sprite no encontrado:" << nombre;
    return QPixmap();
}

bool GestorSprites::tieneSprite(const QString& nombre) {
    return sprites.contains(nombre);
}

void GestorSprites::cargarCarpetaNivel(int nivel, const QString& nombreCarpeta, const QString& prefijo) {
    // Construir ruta: Sprites/Nivel X/nombreCarpeta
    QString rutaCarpeta = rutaBase + "Nivel " + QString::number(nivel) + "/" + nombreCarpeta;

    QDir dir(rutaCarpeta);
    if (!dir.exists()) {
        qDebug() << " Carpeta no encontrada:" << rutaCarpeta;
        return;
    }

    qDebug() << " Escaneando:" << nombreCarpeta << "en Nivel" << nivel;

    // Patrones de archivos
    QStringList filtros;
    filtros << "*.png" << "*.PNG" << "*.jpg" << "*.JPG" << "*.jpeg" << "*.JPEG";

    // Obtener TODOS los archivos que coincidan con los filtros
    QStringList archivos = dir.entryList(filtros, QDir::Files);

    if (archivos.isEmpty()) {
        qDebug() << " No se encontraron imagenes";
        return;
    }

    // Cargar cada archivo encontrado
    for (const QString& archivo : archivos) {
        QString rutaCompleta = dir.filePath(archivo);
        QString nombreSprite = prefijo + "_" + archivo;

        cargarSprite(nombreSprite, rutaCompleta);
    }
}

void GestorSprites::cargarTodosLosSprites() {
    qDebug() << "========================================";
    qDebug() << "INICIANDO CARGA DE SPRITES";
    qDebug() << "Ruta base:" << rutaBase;
    qDebug() << "========================================";

    // Verificar que la ruta base existe
    QDir dirBase(rutaBase);
    if (!dirBase.exists()) {
        qDebug() << "ERROR: La ruta base no existe:" << rutaBase;
        qDebug() << "Ruta absoluta:" << dirBase.absolutePath();
        return;
    }

    qDebug() << "Ruta base encontrada";

    // ========== NIVEL 1: OCÉANO ==========
    qDebug() << "\n === NIVEL 1: OCEANO ===";

    cargarCarpetaNivel(1, "barco", "barco");
    cargarCarpetaNivel(1, "Oceano_Fondo", "oceano");
    cargarCarpetaNivel(1, "submarino", "submarino");
    cargarCarpetaNivel(1, "Torpedos", "torpedo");

    // ========== NIVEL 2: BARCO ==========
    qDebug() << "\n === NIVEL 2: BARCO ===";

    cargarCarpetaNivel(2, "Escaleras", "escalera");
    cargarCarpetaNivel(2, "Estructura_Fondo", "estructura");
    cargarCarpetaNivel(2, "JugadorNPCs", "npc");
    cargarCarpetaNivel(2, "Objetos dinamicos", "objeto");

    // ========== NIVEL 3: SUBMARINO ==========
    qDebug() << "\n === NIVEL 3: SUBMARINO ===";

    cargarCarpetaNivel(3, "Escombros(Flotando_Hundiendo)", "escombro");
    cargarCarpetaNivel(3, "Fondo Submarino", "fondo_sub");
    cargarCarpetaNivel(3, "Vortices submarinos", "vortice");

    qDebug() << "\n========================================";
    qDebug() << "CARGA COMPLETADA";
    qDebug() << "Total de sprites cargados:" << sprites.size();
    qDebug() << "========================================";

    // Listar todos los sprites cargados
    if (sprites.size() > 0) {
        qDebug() << "\n LISTA DE SPRITES CARGADOS:";
        for (auto it = sprites.begin(); it != sprites.end(); ++it) {
            qDebug() << "  -" << it.key();
        }
    }
}
