#include "nivel2barco.h"
#include "gestorsprites.h"
#include <QColor>
#include <QPolygonF>
#include <cstdlib>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// ========== CONSTRUCTOR ==========

Nivel2Barco::Nivel2Barco()
    : Nivel(),
    anguloBarco(0.0f),
    velocidadInclinacion(2.0f),
    tiempoHundimiento(0.0f),
    tiempoLimiteHundimiento(120.0f),
    zonaRescate(400, 80),
    tiempoSpawnObjeto(0.0f),
    intervaloSpawnObjeto(5.0f),
    npcsRescatados(0),
    npcsMuertos(0),
    npcsTotales(0),
    objetivoNPCs(15) {

    anchoNivel = 800;
    altoNivel = 600;
    tiempoLimite = tiempoLimiteHundimiento;

    camara->setLimites(Vector2D(0, 0), Vector2D(anchoNivel, altoNivel));
    camara->setTieneScroll(false);

    cuartos.push_back(Vector2D(200, 350));
    cuartos.push_back(Vector2D(350, 320));
    cuartos.push_back(Vector2D(500, 350));
    cuartos.push_back(Vector2D(300, 280));
    cuartos.push_back(Vector2D(450, 280));

    inicializar();
}

Nivel2Barco::~Nivel2Barco() {
    npcs.clear();
    objetos.clear();
}

// ========== INICIALIZACION ==========

void Nivel2Barco::inicializar() {
    jugador = new Jugador(Vector2D(200, 400));
    jugador->setDimensiones(40, 56);
    motorFisica->agregarEntidad(jugador);

    crearNPCs();
    crearObjetos();

    motorFisica->setGravedadActiva(true);
    motorFisica->setGravedad(9.8f);
}

void Nivel2Barco::crearNPCs() {
    // Crear 20 NPCs
    for (int i = 0; i < 20; i++) {
        Vector2D posicion = cuartos[rand() % cuartos.size()];

        posicion.x += (rand() % 60) - 30;
        posicion.y += (rand() % 30) - 15;

        TipoNPC tipo;
        int tipoRand = rand() % 3;
        if (tipoRand == 0) tipo = TipoNPC::HOMBRE;
        else if (tipoRand == 1) tipo = TipoNPC::MUJER;
        else tipo = TipoNPC::NINO;

        NPC* npc = new NPC(posicion, tipo);
        npc->setDimensiones(36, 50);
        npc->seguirJugador(jugador);

        motorFisica->agregarEntidad(npc);
        npcs.push_back(npc);
        npcsTotales++;
    }
}

void Nivel2Barco::crearObjetos() {
    for (int i = 0; i < 5; i++) {
        Vector2D pos(180 + i * 100, 280 + (rand() % 40));
        ObjetoJuego* maleta = new ObjetoJuego(pos, TipoObjeto::MALETA);
        motorFisica->agregarEntidad(maleta);
        objetos.push_back(maleta);
    }

    for (int i = 0; i < 4; i++) {
        Vector2D pos(220 + i * 120, 330);
        ObjetoJuego* silla = new ObjetoJuego(pos, TipoObjeto::SILLA);
        motorFisica->agregarEntidad(silla);
        objetos.push_back(silla);
    }
}

void Nivel2Barco::spawnearObjetoAleatorio() {
    TipoObjeto tipo;
    int tipoRand = rand() % 3;
    if (tipoRand == 0) tipo = TipoObjeto::ESCOMBRO_PEQUENO;
    else if (tipoRand == 1) tipo = TipoObjeto::MALETA;
    else tipo = TipoObjeto::ESCOMBRO_GRANDE;

    Vector2D pos(200 + (rand() % 400), 50);

    ObjetoJuego* objeto = new ObjetoJuego(pos, tipo);
    motorFisica->agregarEntidad(objeto);
    objetos.push_back(objeto);
}

void Nivel2Barco::respawnearNPC() {
    Vector2D posicion = cuartos[rand() % cuartos.size()];
    posicion.x += (rand() % 60) - 30;
    posicion.y += (rand() % 30) - 15;

    TipoNPC tipo;
    int tipoRand = rand() % 3;
    if (tipoRand == 0) tipo = TipoNPC::HOMBRE;
    else if (tipoRand == 1) tipo = TipoNPC::MUJER;
    else tipo = TipoNPC::NINO;

    NPC* npc = new NPC(posicion, tipo);
    npc->setDimensiones(36, 50);
    npc->seguirJugador(jugador);

    motorFisica->agregarEntidad(npc);
    npcs.push_back(npc);
}

// ========== ACTUALIZACION ==========

void Nivel2Barco::actualizar(float dt) {
    if (completado || fallado) return;

    tiempoTranscurrido += dt;
    tiempoHundimiento += dt;

    actualizarInclinacion(dt);

    if (jugador) {
        Vector2D pos = jugador->getPosicion();
        float ancho = jugador->getAncho();
        float alto = jugador->getAlto();

        if (pos.x < 100) pos.x = 100;
        if (pos.x + ancho > 700) pos.x = 700 - ancho;

        if (pos.y < 50) pos.y = 50;
        if (pos.y + alto > 550) pos.y = 550 - alto;

        jugador->setPosicion(pos);
    }

    motorFisica->actualizar(dt);

    actualizarNPCs(dt);
    actualizarObjetos(dt);
    verificarRescates();
    verificarColisionesObjetos();

    tiempoSpawnObjeto += dt;
    if (tiempoSpawnObjeto >= intervaloSpawnObjeto) {
        spawnearObjetoAleatorio();
        tiempoSpawnObjeto = 0.0f;
    }

    if (verificarVictoria()) {
        completado = true;
    }

    if (verificarDerrota()) {
        fallado = true;
    }
}

void Nivel2Barco::actualizarInclinacion(float dt) {
    anguloBarco += velocidadInclinacion * dt;

    if (anguloBarco > 30.0f) {
        anguloBarco = 30.0f;
    }

    for (auto* objeto : objetos) {
        if (objeto && objeto->estaActivo()) {
            objeto->setAnguloBarco(anguloBarco);
        }
    }
}

void Nivel2Barco::actualizarNPCs(float dt) {
    static std::map<NPC*, float> tiemposMuerte;

    auto it = npcs.begin();

    while (it != npcs.end()) {
        NPC* npc = *it;

        if (!npc || !npc->estaActivo()) {
            it = npcs.erase(it);
        } else {
            // ===== VERIFICAR SI MURIO =====
            if (!npc->estaConsciente() && npc->getResistencia() <= 0.0f) {
                if (tiemposMuerte.find(npc) == tiemposMuerte.end()) {
                    // Primera vez que muere
                    tiemposMuerte[npc] = 0.0f;
                    npcsMuertos++;
                    agregarPuntos(-50);
                }

                tiemposMuerte[npc] += dt;

                if (tiemposMuerte[npc] >= 5.0f) {
                    npc->destruir();
                    tiemposMuerte.erase(npc);
                    respawnearNPC();
                }
            }

            if (npc->getEstado() == EstadoNPC::PANICO) {
                float distancia = npc->getPosicion().distanciaA(jugador->getPosicion());
                if (distancia < 100.0f) {
                    npc->seguirJugador(jugador);
                }
            }
            ++it;
        }
    }
}

void Nivel2Barco::actualizarObjetos(float /*dt*/) {
    auto it = objetos.begin();

    while (it != objetos.end()) {
        ObjetoJuego* objeto = *it;

        if (!objeto || !objeto->estaActivo()) {
            it = objetos.erase(it);
        } else {
            ++it;
        }
    }
}

void Nivel2Barco::verificarColisionesObjetos() {
    if (!jugador) return;

    for (auto* objeto : objetos) {
        if (!objeto || !objeto->estaActivo()) continue;

        TipoObjeto tipoObj = objeto->getTipo();
        if (tipoObj == TipoObjeto::LAMPARA) continue;

        Vector2D velObjeto = objeto->getVelocidad();
        float velocidad = velObjeto.magnitud();

        if (velocidad < 5.0f) continue;

        float peso = objeto->getMasa();
        float danio = (peso * velocidad) / 10.0f;

        if (objeto->colisionaCon(jugador)) {
            jugador->recibirDanio(danio);
            objeto->setVelocidad(velObjeto * 0.3f);
        }

        for (auto* npc : npcs) {
            if (npc && npc->estaActivo() && objeto->colisionaCon(npc)) {
                npc->caer(danio);
                objeto->setVelocidad(velObjeto * 0.3f);
            }
        }
    }
}

void Nivel2Barco::verificarRescates() {
    for (auto* npc : npcs) {
        if (npc && npc->estaActivo() && npc->getEstado() == EstadoNPC::SIGUIENDO) {
            float distancia = npc->getPosicion().distanciaA(zonaRescate);

            if (distancia < 100.0f) {
                // ===== PUNTUACION SEGUN SALUD =====
                float resistencia = npc->getResistencia();
                int puntos = 0;

                if (resistencia >= 100.0f) {
                    puntos = 100;  // Salud perfecta
                } else if (resistencia >= 75.0f) {
                    puntos = 75;   // Buena salud
                } else if (resistencia >= 50.0f) {
                    puntos = 50;   // Salud media
                } else {
                    puntos = 25;   // Salud baja
                }

                agregarPuntos(puntos);
                npc->rescatar();
                npcsRescatados++;
            }
        }
    }
}

// ========== RENDERIZADO ==========

void Nivel2Barco::renderizar(QPainter& painter) {
    if (!camara) return;

    painter.save();

    GestorSprites* gestor = GestorSprites::obtenerInstancia();

    QPixmap fondoEstructura = gestor->getSprite("estructura1");

    if (!fondoEstructura.isNull()) {
        QPixmap fondoEscalado = fondoEstructura.scaled(800, 600,
                                                       Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap(0, 0, fondoEscalado);
    } else {
        painter.fillRect(0, 0, 800, 400, QColor(100, 150, 200));
        painter.fillRect(0, 400, 800, 200, QColor(0, 50, 100));
    }

    // Zona de rescate
    painter.setBrush(QColor(255, 215, 0, 200));
    painter.setPen(QPen(QColor(255, 255, 0), 4));
    painter.drawRect(zonaRescate.x - 80, zonaRescate.y - 30, 160, 60);

    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.setPen(Qt::black);
    painter.drawText(zonaRescate.x - 70, zonaRescate.y - 5, "ZONA DE");
    painter.drawText(zonaRescate.x - 70, zonaRescate.y + 15, "RESCATE");

    // Renderizar NPCs con barras de vida
    for (auto* npc : npcs) {
        if (!npc || !npc->estaActivo()) continue;
        npc->renderizar(painter);
    }

    // Renderizar jugador
    if (jugador) {
        Vector2D pos = jugador->getPosicion();
        float ancho = jugador->getAncho();
        float alto = jugador->getAlto();

        QPixmap spriteJugador = gestor->getSprite("npc_01");

        if (!spriteJugador.isNull()) {
            QPixmap jugadorEscalado = spriteJugador.scaled(ancho, alto,
                                                           Qt::KeepAspectRatio, Qt::SmoothTransformation);

            painter.setPen(QPen(Qt::yellow, 4));
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(pos.x - 2, pos.y - 2, ancho + 4, alto + 4);

            painter.drawPixmap(pos.x, pos.y, jugadorEscalado);
        } else {
            painter.setBrush(QColor(0, 200, 0));
            painter.setPen(QPen(Qt::yellow, 3));
            painter.drawRect(pos.x, pos.y, ancho, alto);
        }
    }

    // Renderizar objetos
    for (auto* objeto : objetos) {
        if (objeto && objeto->estaActivo()) {
            objeto->renderizar(painter);
        }
    }

    painter.restore();
}

// ========== REINICIAR ==========

void Nivel2Barco::reiniciar() {
    limpiar();

    completado = false;
    fallado = false;
    tiempoTranscurrido = 0.0f;
    tiempoHundimiento = 0.0f;
    puntuacion = 0;
    anguloBarco = 0.0f;
    npcsRescatados = 0;
    npcsMuertos = 0;
    npcsTotales = 0;
    tiempoSpawnObjeto = 0.0f;

    npcs.clear();
    objetos.clear();

    motorFisica = new MotorFisica();
    camara = new Camara(800, 600);
    camara->setLimites(Vector2D(0, 0), Vector2D(anchoNivel, altoNivel));

    inicializar();
}

// ========== CONDICIONES ==========

bool Nivel2Barco::verificarVictoria() {
    return npcsRescatados >= objetivoNPCs;
}

bool Nivel2Barco::verificarDerrota() {
    bool sinVidas = jugador && jugador->estaMuerto();
    bool tiempoAgotado = tiempoHundimiento >= tiempoLimiteHundimiento;

    return sinVidas || tiempoAgotado;
}

void Nivel2Barco::manejarInput(int /*tecla*/, bool /*presionada*/) {
}
