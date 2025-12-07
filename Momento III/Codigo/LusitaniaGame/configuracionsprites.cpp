#include "configuracionsprites.h"
#include <QString>
#include <QDebug>

ConfiguracionSprites::ConfiguracionSprites() {
    // ========== MENU/UI ==========
    mapaSprites["menu_fondo"] = "ui/menu_fondo.jpg";

    // ========== NIVEL 1: OCEANO ==========
    // Barco Lusitania
    mapaSprites["lusitania"] = "nivel1/barco/lusitania.png";

    // Submarino
    mapaSprites["submarino"] = "nivel1/submarino/submarino.png";

    // Torpedo
    mapaSprites["torpedo"] = "nivel1/torpedo/torpedo.png";

    // ========== NIVEL 2: BARCO ==========
    // Escalera
    mapaSprites["escalera"] = "nivel2/escalera/escalera.png";

    // Fondos/Estructuras
    mapaSprites["estructura1"] = "nivel2/fondo/estructura1.png";
    mapaSprites["estructura2"] = "nivel2/fondo/estructura2.png";

    // NPCs
    for (int i = 1; i <= 8; i++) {
        QString nombre = QString("npc_%1").arg(i, 2, 10, QChar('0'));
        QString ruta = QString("nivel2/npcs/npc_%1.png").arg(i, 2, 10, QChar('0'));
        mapaSprites[nombre] = ruta;
    }
    mapaSprites["npc_generico"] = "nivel2/npcs/npc_generico.png";

    // Objetos
    mapaSprites["cofres"] = "nivel2/objetos/cofres.png";
    mapaSprites["lamparas"] = "nivel2/objetos/lamparas.png";

    // ========== NIVEL 3: SUBMARINO ==========
    // Escombros
    mapaSprites["escombro_madera"] = "nivel3/escombros/madera.png";
    mapaSprites["escombro_metal"] = "nivel3/escombros/metal.png";
    mapaSprites["escombro_salvavidas"] = "nivel3/escombros/salvavidas.png";

    // Fondo
    mapaSprites["oceano_profundo"] = "nivel3/fondo/oceano_profundo.jpg";

    // Jugador nadador
    mapaSprites["jugador_idle"] = "nivel3/jugador/jugador_idle.png";
    mapaSprites["jugador_swim"] = "nivel3/jugador/jugador_swim.png";
    mapaSprites["jugador_hurt"] = "nivel3/jugador/jugador_hurt.png";
    mapaSprites["jugador_death"] = "nivel3/jugador/jugador_death.png";

    // Vortice
    mapaSprites["vortice"] = "nivel3/vortice/vortice.png";

    // === DEBUG: VERIFICAR QUE ARCHIVOS SE INTENTAN CARGAR ===
    qDebug() << "\n========== CONFIGURACIÓN DE SPRITES ==========";
    for (auto it = mapaSprites.constBegin(); it != mapaSprites.constEnd(); ++it) {
        qDebug() << "Sprite:" << it.key() << "-> Archivo:" << it.value();
    }
    qDebug() << "Total sprites configurados:" << mapaSprites.size();
    qDebug() << "==============================================\n";
}

ConfiguracionSprites::~ConfiguracionSprites() {
    mapaSprites.clear();
}

QString ConfiguracionSprites::obtenerRuta(const QString& nombre) const {
    if (mapaSprites.contains(nombre)) {
        return mapaSprites[nombre];
    }

    qWarning() << "[ConfiguracionSprites] Sprite no configurado:" << nombre;
    return "";
}

bool ConfiguracionSprites::existe(const QString& nombre) const {
    return mapaSprites.contains(nombre);
}
