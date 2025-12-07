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
    zonaRescate(700, 500),
    tiempoSpawnObjeto(0.0f),
    intervaloSpawnObjeto(5.0f),
    npcsRescatados(0),
    npcsTotales(0),
    objetivoNPCs(10) {

    // Configurar nivel
    anchoNivel = 800;
    altoNivel = 600;
    tiempoLimite = tiempoLimiteHundimiento;

    // Actualizar limites de camara
    camara->setLimites(Vector2D(0, 0), Vector2D(anchoNivel, altoNivel));
    camara->setTieneScroll(false);

    // Definir cuartos del barco
    cuartos.push_back(Vector2D(200, 300));
    cuartos.push_back(Vector2D(400, 250));
    cuartos.push_back(Vector2D(600, 300));

    // Inicializar
    inicializar();
}

Nivel2Barco::~Nivel2Barco() {
    npcs.clear();
    objetos.clear();
}

// ========== INICIALIZACION ==========

void Nivel2Barco::inicializar() {
    // Crear jugador (en el barco)
    jugador = new Jugador(Vector2D(100, 400));
    jugador->setDimensiones(32, 48);
    motorFisica->agregarEntidad(jugador);

    // Crear NPCs
    crearNPCs();

    // Crear objetos iniciales
    crearObjetos();

    // Activar gravedad
    motorFisica->setGravedadActiva(true);
    motorFisica->setGravedad(9.8f);
}

void Nivel2Barco::crearNPCs() {
    // Crear 15 NPCs en diferentes cuartos
    for (int i = 0; i < 15; i++) {
        // Elegir cuarto aleatorio
        Vector2D posicion = cuartos[rand() % cuartos.size()];

        // Offset aleatorio dentro del cuarto
        posicion.x += (rand() % 80) - 40;
        posicion.y += (rand() % 40) - 20;

        // Tipo aleatorio
        TipoNPC tipo;
        int tipoRand = rand() % 3;
        if (tipoRand == 0) tipo = TipoNPC::HOMBRE;
        else if (tipoRand == 1) tipo = TipoNPC::MUJER;
        else tipo = TipoNPC::NINO;

        NPC* npc = new NPC(posicion, tipo);
        npc->seguirJugador(jugador);

        motorFisica->agregarEntidad(npc);
        npcs.push_back(npc);
        npcsTotales++;
    }
}

void Nivel2Barco::crearObjetos() {
    // Crear objetos iniciales

    // Maletas
    for (int i = 0; i < 4; i++) {
        Vector2D pos(150 + i * 120, 250);
        ObjetoJuego* maleta = new ObjetoJuego(pos, TipoObjeto::MALETA);
        motorFisica->agregarEntidad(maleta);
        objetos.push_back(maleta);
    }

    // Sillas
    for (int i = 0; i < 3; i++) {
        Vector2D pos(200 + i * 150, 300);
        ObjetoJuego* silla = new ObjetoJuego(pos, TipoObjeto::SILLA);
        motorFisica->agregarEntidad(silla);
        objetos.push_back(silla);
    }

    // Lamparas colgantes (MCU)
    for (int i = 0; i < 2; i++) {
        Vector2D anclaje(300 + i * 200, 150);
        ObjetoJuego* lampara = new ObjetoJuego(Vector2D(0, 0), TipoObjeto::LAMPARA);
        lampara->colgar(anclaje);
        motorFisica->agregarEntidad(lampara);
        objetos.push_back(lampara);
    }
}

void Nivel2Barco::spawnearObjetoAleatorio() {
    // Spawnear objeto aleatorio que cae desde arriba

    TipoObjeto tipo;
    int tipoRand = rand() % 3;
    if (tipoRand == 0) tipo = TipoObjeto::ESCOMBRO_PEQUENO;
    else if (tipoRand == 1) tipo = TipoObjeto::MALETA;
    else tipo = TipoObjeto::ESCOMBRO_GRANDE;

    Vector2D pos(150 + (rand() % 500), 50);

    ObjetoJuego* objeto = new ObjetoJuego(pos, tipo);
    motorFisica->agregarEntidad(objeto);
    objetos.push_back(objeto);
}

// ========== ACTUALIZACION ==========

void Nivel2Barco::actualizar(float dt) {
    if (completado || fallado) return;

    // Actualizar tiempo
    tiempoTranscurrido += dt;
    tiempoHundimiento += dt;

    // Actualizar inclinacion del barco
    actualizarInclinacion(dt);

    // APLICAR LIMITES AL JUGADOR
    if (jugador) {
        Vector2D pos = jugador->getPosicion();
        float ancho = jugador->getAncho();
        float alto = jugador->getAlto();

        // Limites horizontales
        if (pos.x < 0) pos.x = 0;
        if (pos.x + ancho > anchoNivel) pos.x = anchoNivel - ancho;

        // Limites verticales
        if (pos.y < 0) pos.y = 0;
        if (pos.y + alto > altoNivel) pos.y = altoNivel - alto;

        jugador->setPosicion(pos);
    }

    // Actualizar fisica
    motorFisica->actualizar(dt);

    // Actualizar NPCs
    actualizarNPCs(dt);

    // Actualizar objetos
    actualizarObjetos(dt);

    // Verificar rescates
    verificarRescates();

    // Spawning de objetos
    tiempoSpawnObjeto += dt;
    if (tiempoSpawnObjeto >= intervaloSpawnObjeto) {
        spawnearObjetoAleatorio();
        tiempoSpawnObjeto = 0.0f;
    }

    // Verificar condiciones
    if (verificarVictoria()) {
        completado = true;
    }

    if (verificarDerrota()) {
        fallado = true;
    }
}

void Nivel2Barco::actualizarInclinacion(float dt) {
    // Aumentar inclinacion gradualmente
    anguloBarco += velocidadInclinacion * dt;

    // Limitar a 30 grados
    if (anguloBarco > 30.0f) {
        anguloBarco = 30.0f;
    }

    // Aplicar angulo a todos los objetos
    for (auto* objeto : objetos) {
        if (objeto && objeto->estaActivo()) {
            objeto->setAnguloBarco(anguloBarco);
        }
    }
}

void Nivel2Barco::actualizarNPCs(float /*dt*/) {
    // Actualizar lista de NPCs activos
    auto it = npcs.begin();

    while (it != npcs.end()) {
        NPC* npc = *it;

        if (!npc || !npc->estaActivo()) {
            it = npcs.erase(it);
        } else {
            // Asegurar que sigue al jugador
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
    // Limpiar objetos inactivos
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

void Nivel2Barco::verificarRescates() {
    // Verificar si NPCs llegaron a la zona de rescate
    for (auto* npc : npcs) {
        if (npc && npc->estaActivo() && npc->getEstado() == EstadoNPC::SIGUIENDO) {
            float distancia = npc->getPosicion().distanciaA(zonaRescate);

            if (distancia < 50.0f) {
                // ¡Rescatado!
                npc->rescatar();
                npcsRescatados++;
                agregarPuntos(50); // 50 puntos por rescate
            }
        }
    }
}

// ========== RENDERIZADO ==========

void Nivel2Barco::renderizar(QPainter& painter) {
    if (!camara) return;

    painter.save();

    GestorSprites* gestor = GestorSprites::obtenerInstancia();

    // ===== FONDO =====
    QPixmap fondoEstructura = gestor->getSprite("estructura1");

    if (!fondoEstructura.isNull()) {
        // Dibujar fondo escalado
        QPixmap fondoEscalado = fondoEstructura.scaled(800, 600,
                                                       Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap(0, 0, fondoEscalado);
    } else {
        // Fallback: degradado
        painter.fillRect(0, 0, 800, 400, QColor(100, 150, 200));
        painter.fillRect(0, 400, 800, 200, QColor(0, 50, 100));
    }

    // ===== BARCO INCLINADO =====
    painter.save();

    QPointF centro(400, 400);
    painter.translate(centro);
    painter.rotate(anguloBarco);
    painter.translate(-centro);

    // Casco del barco
    painter.setBrush(QColor(80, 40, 0));
    painter.setPen(Qt::black);
    QPolygonF casco;
    casco << QPointF(100, 400) << QPointF(700, 400)
          << QPointF(650, 450) << QPointF(150, 450);
    painter.drawPolygon(casco);

    // Cubiertas
    painter.setBrush(QColor(120, 80, 40));
    painter.drawRect(150, 300, 500, 100);
    painter.drawRect(200, 200, 400, 100);

    // Chimeneas
    painter.setBrush(QColor(50, 25, 0));
    painter.drawRect(300, 150, 30, 50);
    painter.drawRect(450, 150, 30, 50);

    painter.restore();

    // ===== ZONA DE RESCATE =====
    painter.setBrush(QColor(255, 200, 0));
    painter.setPen(Qt::black);
    painter.drawEllipse(zonaRescate.x - 30, zonaRescate.y - 15, 60, 30);

    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.setPen(Qt::black);
    painter.drawText(zonaRescate.x - 25, zonaRescate.y + 40, "RESCATE");

    // ===== RENDERIZAR ENTIDADES =====
    for (auto* entidad : motorFisica->getEntidades()) {
        if (entidad && entidad->estaActivo()) {
            entidad->renderizar(painter);
        }
    }

    painter.restore();
}

// ========== REINICIAR ==========

void Nivel2Barco::reiniciar() {
    // Limpiar todo
    limpiar();

    // Reiniciar variables
    completado = false;
    fallado = false;
    tiempoTranscurrido = 0.0f;
    tiempoHundimiento = 0.0f;
    puntuacion = 0;
    anguloBarco = 0.0f;
    npcsRescatados = 0;
    npcsTotales = 0;
    tiempoSpawnObjeto = 0.0f;

    npcs.clear();
    objetos.clear();

    // Reinicializar
    motorFisica = new MotorFisica();
    camara = new Camara(800, 600);
    camara->setLimites(Vector2D(0, 0), Vector2D(anchoNivel, altoNivel));

    inicializar();
}

// ========== CONDICIONES ==========

bool Nivel2Barco::verificarVictoria() {
    // Victoria: rescatar 10 NPCs
    return npcsRescatados >= objetivoNPCs;
}

bool Nivel2Barco::verificarDerrota() {
    // Derrota: quedarse sin vidas o se acaba el tiempo
    bool sinVidas = jugador && jugador->estaMuerto();
    bool tiempoAgotado = tiempoHundimiento >= tiempoLimiteHundimiento;

    return sinVidas || tiempoAgotado;
}

void Nivel2Barco::manejarInput(int /*tecla*/, bool /*presionada*/) {
    // Metodo reservado para input adicional
}
