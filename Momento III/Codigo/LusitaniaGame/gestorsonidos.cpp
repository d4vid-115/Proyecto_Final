#include "gestorsonidos.h"

GestorSonidos* GestorSonidos::instancia = nullptr;

GestorSonidos::GestorSonidos()
    : volumenMusica(0.5f),
    volumenEfectos(0.7f),
    musicaHabilitada(true),
    efectosHabilitados(true) {

    rutaBase = obtenerRutaSonidos();

    // Crear reproductor de musica
    reproductorMusica = new QMediaPlayer();
    salidaMusica = new QAudioOutput();
    reproductorMusica->setAudioOutput(salidaMusica);
    salidaMusica->setVolume(volumenMusica);

    qDebug() << "GestorSonidos inicializado. Ruta:" << rutaBase;
}

GestorSonidos::~GestorSonidos() {
    if (reproductorMusica) {
        reproductorMusica->stop();
        delete reproductorMusica;
        reproductorMusica = nullptr;
    }

    if (salidaMusica) {
        delete salidaMusica;
        salidaMusica = nullptr;
    }

    // Limpiar efectos
    for (auto* reproductor : reproductoresEfectos) {
        if (reproductor) {
            reproductor->stop();
            delete reproductor;
        }
    }

    for (auto* salida : salidasEfectos) {
        if (salida) {
            delete salida;
        }
    }

    reproductoresEfectos.clear();
    salidasEfectos.clear();
}

GestorSonidos* GestorSonidos::obtenerInstancia() {
    if (!instancia) {
        instancia = new GestorSonidos();
    }
    return instancia;
}

void GestorSonidos::destruirInstancia() {
    if (instancia) {
        delete instancia;
        instancia = nullptr;
    }
}

QString GestorSonidos::obtenerRutaSonidos() {
    if (!rutaBase.isEmpty()) {
        return rutaBase;
    }

    QStringList posiblesRutas = {
        QCoreApplication::applicationDirPath() + "/assets/sounds/",
        QCoreApplication::applicationDirPath() + "/../assets/sounds/",
        QCoreApplication::applicationDirPath() + "/../../assets/sounds/",
        QDir::currentPath() + "/assets/sounds/",
        QDir::currentPath() + "/../assets/sounds/",
    };

    qDebug() << "\n[GestorSonidos] === BUSCANDO SONIDOS ===";
    qDebug() << "[GestorSonidos] applicationDirPath:" << QCoreApplication::applicationDirPath();

    for (const QString& ruta : posiblesRutas) {
        QDir dir(ruta);
        qDebug() << "[GestorSonidos] Probando:" << dir.absolutePath();

        if (dir.exists()) {
            qDebug() << "[GestorSonidos] ENCONTRADO:" << dir.absolutePath();
            rutaBase = dir.absolutePath() + "/";
            return rutaBase;
        }
    }

    qWarning() << "[GestorSonidos] NO SE ENCONTRO CARPETA DE SONIDOS!";
    return "";
}

bool GestorSonidos::cargarMusica(const QString& nombre, const QString& archivo) {
    QString rutaCompleta = rutaBase + archivo;
    QFileInfo checkFile(rutaCompleta);

    if (!checkFile.exists()) {
        qDebug() << "Musica no encontrada: " << rutaCompleta;
        return false;
    }

    qDebug() << "Musica registrada: " << nombre;
    return true;
}

bool GestorSonidos::cargarEfecto(const QString& nombre, const QString& archivo) {
    QString rutaCompleta = rutaBase + archivo;
    QFileInfo checkFile(rutaCompleta);

    if (!checkFile.exists()) {
        qDebug() << "Efecto no encontrado: " << rutaCompleta;
        return false;
    }

    // Crear reproductor para este efecto
    QMediaPlayer* reproductor = new QMediaPlayer();
    QAudioOutput* salida = new QAudioOutput();
    reproductor->setAudioOutput(salida);
    salida->setVolume(volumenEfectos);
    reproductor->setSource(QUrl::fromLocalFile(rutaCompleta));

    reproductoresEfectos[nombre] = reproductor;
    salidasEfectos[nombre] = salida;

    qDebug() << "Efecto cargado: " << nombre;
    return true;
}

void GestorSonidos::cargarTodosLosSonidos() {
    qDebug() << "\n========================================";
    qDebug() << "CARGANDO SONIDOS";
    qDebug() << "========================================\n";

    // Música
    cargarMusica("menu", "menu.mp3");
    cargarMusica("nivel1", "nivel1.mp3");
    cargarMusica("nivel2", "nivel2.mp3");
    cargarMusica("nivel3", "nivel3.mp3");
    cargarMusica("victoria", "victoria.mp3");
    cargarMusica("derrota", "derrota.mp3");

    qDebug() << "\n========================================";
    qDebug() << "CARGA DE SONIDOS COMPLETADA";
    qDebug() << "========================================\n";
}

void GestorSonidos::reproducirMusica(const QString& nombre, bool loop) {
    if (!musicaHabilitada || !reproductorMusica) return;

    QString rutaCompleta = rutaBase + nombre + ".mp3";
    QFileInfo checkFile(rutaCompleta);

    if (!checkFile.exists()) {
        qDebug() << "No se puede reproducir musica:" << nombre;
        return;
    }

    reproductorMusica->stop();
    reproductorMusica->setSource(QUrl::fromLocalFile(rutaCompleta));

    if (loop) {
        reproductorMusica->setLoops(QMediaPlayer::Infinite);
    } else {
        reproductorMusica->setLoops(1);
    }

    reproductorMusica->play();
    qDebug() << "Reproduciendo musica: " << nombre;
}

void GestorSonidos::pausarMusica() {
    if (reproductorMusica) {
        reproductorMusica->pause();
    }
}

void GestorSonidos::reanudarMusica() {
    if (reproductorMusica && musicaHabilitada) {
        reproductorMusica->play();
    }
}

void GestorSonidos::detenerMusica() {
    if (reproductorMusica) {
        reproductorMusica->stop();
    }
}

void GestorSonidos::reproducirEfecto(const QString& nombre) {
    if (!efectosHabilitados) return;

    if (reproductoresEfectos.contains(nombre)) {
        reproductoresEfectos[nombre]->stop();
        reproductoresEfectos[nombre]->play();
    } else {
        qDebug() << "Efecto no encontrado: " << nombre;
    }
}

void GestorSonidos::setVolumenMusica(float volumen) {
    volumenMusica = qBound(0.0f, volumen, 1.0f);
    if (salidaMusica) {
        salidaMusica->setVolume(volumenMusica);
    }
}

void GestorSonidos::setVolumenEfectos(float volumen) {
    volumenEfectos = qBound(0.0f, volumen, 1.0f);
    for (auto* salida : salidasEfectos) {
        if (salida) {
            salida->setVolume(volumenEfectos);
        }
    }
}

void GestorSonidos::toggleMusica() {
    musicaHabilitada = !musicaHabilitada;
    if (!musicaHabilitada) {
        detenerMusica();
    }
}

void GestorSonidos::toggleEfectos() {
    efectosHabilitados = !efectosHabilitados;
}
