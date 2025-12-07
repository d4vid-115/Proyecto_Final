#include "gestorsprites.h"
#include <QFileInfo>

GestorSprites* GestorSprites::instancia = nullptr;

GestorSprites::GestorSprites() {
    // Obtener ruta relativa de assets
    rutaBase = obtenerRutaAssets();
    qDebug() << "Ruta base de assets:" << rutaBase;
}

GestorSprites::~GestorSprites() {
    sprites.clear();
    animaciones.clear();
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

QString GestorSprites::obtenerRutaAssets() {
    if (!rutaBase.isEmpty()) {
        return rutaBase;
    }

    QStringList posiblesRutas = {
        QCoreApplication::applicationDirPath() + "/assets/sprites/",
        QCoreApplication::applicationDirPath() + "/../assets/sprites/",
        QCoreApplication::applicationDirPath() + "/../../assets/sprites/",
        QDir::currentPath() + "/assets/sprites/",
        QDir::currentPath() + "/../assets/sprites/",
    };

    qDebug() << "\n[GestorSprites] === BUSCANDO ASSETS ===";
    qDebug() << "[GestorSprites] applicationDirPath:" << QCoreApplication::applicationDirPath();
    qDebug() << "[GestorSprites] currentPath:" << QDir::currentPath();

    for (const QString& ruta : posiblesRutas) {
        QDir dir(ruta);
        qDebug() << "[GestorSprites] Probando:" << dir.absolutePath();

        if (dir.exists()) {
            qDebug() << "[GestorSprites] ENCONTRADO:" << dir.absolutePath();
            rutaBase = dir.absolutePath() + "/";
            return rutaBase;
        }
    }

    qWarning() << "[GestorSprites] NO SE ENCONTRO CARPETA DE ASSETS!";
    return "";
}

void GestorSprites::setRutaBase(const QString& ruta) {
    rutaBase = ruta;
}

bool GestorSprites::cargarSprite(const QString& nombre, const QString& ruta) {
    QFileInfo checkFile(ruta);

    if (!checkFile.exists() || !checkFile.isFile()) {
        qDebug() << "Archivo no existe: " << ruta;
        return false;
    }

    QPixmap pixmap(ruta);

    if (pixmap.isNull()) {
        qDebug() << "Error al cargar sprite: " << ruta;
        return false;
    }

    sprites[nombre] = pixmap;
    qDebug() << "Sprite cargado: " << nombre << "(" << pixmap.width() << "x" << pixmap.height() << ")";
    return true;
}

QPixmap GestorSprites::getSprite(const QString& nombre) {
    if (sprites.contains(nombre)) {
        return sprites[nombre];
    }

    qDebug() << "Sprite no encontrado: " << nombre;
    return QPixmap();
}

bool GestorSprites::tieneSprite(const QString& nombre) {
    return sprites.contains(nombre);
}

bool GestorSprites::cargarAnimacion(const QString& nombre, const QString& rutaCarpeta,
                                    const QString& prefijo, int numFrames, float tiempoFrame) {
    Animacion anim;
    anim.tiempoFrame = tiempoFrame;
    anim.loop = true;

    for (int i = 0; i < numFrames; i++) {
        QString nombreArchivo = QString("%1%2.png").arg(prefijo).arg(i, 2, 10, QChar('0'));
        QString rutaCompleta = rutaCarpeta + nombreArchivo;

        QPixmap frame(rutaCompleta);
        if (!frame.isNull()) {
            anim.frames.append(frame);
        } else {
            qDebug() << "Error cargando frame: " << rutaCompleta;
        }
    }

    if (anim.frames.isEmpty()) {
        qDebug() << "No se pudo cargar animacion:" << nombre;
        return false;
    }

    animaciones[nombre] = anim;
    qDebug() << "Animacion cargada: " << nombre << "(" << anim.frames.size() << " frames)";
    return true;
}

QPixmap GestorSprites::getFrameAnimacion(const QString& nombre, float dt) {
    if (!animaciones.contains(nombre)) {
        qDebug() << "Animacion no encontrada: " << nombre;
        return QPixmap();
    }

    Animacion& anim = animaciones[nombre];

    // Actualizar tiempo
    anim.tiempoAcumulado += dt;

    if (anim.tiempoAcumulado >= anim.tiempoFrame) {
        anim.tiempoAcumulado = 0.0f;
        anim.frameActual++;

        if (anim.frameActual >= anim.frames.size()) {
            if (anim.loop) {
                anim.frameActual = 0;
            } else {
                anim.frameActual = anim.frames.size() - 1;
            }
        }
    }

    return anim.frames[anim.frameActual];
}

void GestorSprites::reiniciarAnimacion(const QString& nombre) {
    if (animaciones.contains(nombre)) {
        animaciones[nombre].frameActual = 0;
        animaciones[nombre].tiempoAcumulado = 0.0f;
    }
}

// ========================================
// CARGA DE TODOS LOS SPRITES
// ========================================

void GestorSprites::cargarTodosLosSprites() {
    qDebug() << "\n========================================";
    qDebug() << "INICIANDO CARGA DE SPRITES";
    qDebug() << "========================================\n";

    cargarSpritesNivel1();
    cargarSpritesNivel2();
    cargarSpritesNivel3();
    cargarSpritesUI();

    qDebug() << "\n========================================";
    qDebug() << "CARGA COMPLETADA";
    qDebug() << "Total sprites:" << sprites.size();
    qDebug() << "Total animaciones:" << animaciones.size();
    qDebug() << "========================================\n";
}

void GestorSprites::cargarSpritesNivel1() {
    qDebug() << "\n=== NIVEL 1: OCEANO ===";

    QString rutaNivel1 = rutaBase + "nivel1/";

    // Barco Lusitania
    cargarSprite("lusitania", rutaNivel1 + "barco/lusitania.png");

    // Oceano animado (si tienes frames)
    // cargarAnimacion("oceano", rutaNivel1 + "oceano/", "oceano_", 12, 0.08f);

    // Submarino
    cargarSprite("submarino", rutaNivel1 + "submarino/submarino.png");

    // Torpedo
    cargarSprite("torpedo", rutaNivel1 + "torpedo/torpedo.png");
}

void GestorSprites::cargarSpritesNivel2() {
    qDebug() << "\n=== NIVEL 2: BARCO ===";

    QString rutaNivel2 = rutaBase + "nivel2/";

    // Escalera
    cargarSprite("escalera", rutaNivel2 + "escalera/escalera.png");

    // Fondo
    cargarSprite("estructura1", rutaNivel2 + "fondo/estructura1.png");
    cargarSprite("estructura2", rutaNivel2 + "fondo/estructura2.png");

    // NPCs
    for (int i = 1; i <= 8; i++) {
        QString nombre = QString("npc_%1").arg(i, 2, 10, QChar('0'));
        QString ruta = rutaNivel2 + QString("npcs/npc_%1.png").arg(i, 2, 10, QChar('0'));
        cargarSprite(nombre, ruta);
    }
    cargarSprite("npc_generico", rutaNivel2 + "npcs/npc_generico.png");

    // Objetos
    cargarSprite("cofres", rutaNivel2 + "objetos/cofres.png");
    cargarSprite("lamparas", rutaNivel2 + "objetos/lamparas.png");
}

void GestorSprites::cargarSpritesNivel3() {
    qDebug() << "\n=== NIVEL 3: SUBMARINO ===";

    QString rutaNivel3 = rutaBase + "nivel3/";

    // Escombros
    cargarSprite("escombro_madera", rutaNivel3 + "escombros/madera.png");
    cargarSprite("escombro_metal", rutaNivel3 + "escombros/metal.png");
    cargarSprite("escombro_salvavidas", rutaNivel3 + "escombros/salvavidas.png");

    // Fondo
    cargarSprite("oceano_profundo", rutaNivel3 + "fondo/oceano_profundo.jpg");

    // Jugador
    cargarSprite("jugador_idle", rutaNivel3 + "jugador/jugador_idle.png");
    cargarSprite("jugador_swim", rutaNivel3 + "jugador/jugador_swim.png");
    cargarSprite("jugador_hurt", rutaNivel3 + "jugador/jugador_hurt.png");
    cargarSprite("jugador_death", rutaNivel3 + "jugador/jugador_death.png");

    // Vortice
    cargarSprite("vortice", rutaNivel3 + "vortice/vortice.png");
}

void GestorSprites::cargarSpritesUI() {
    qDebug() << "\n=== UI/MENU ===";

    QString rutaUI = rutaBase + "ui/";

    // Fondo de menu
    cargarSprite("menu_fondo", rutaUI + "menu_fondo.jpg");
}
