#include "nivel3submarino.h"
#include <QColor>
#include <cstdlib>

// ========== CONSTRUCTOR ==========

Nivel3Submarino::Nivel3Submarino()
    : Nivel(),
    profundidadInicial(800.0f),
    profundidadObjetivo(0.0f),
    profundidadActual(800.0f),
    tiempoSpawnVortice(0.0f),
    intervaloSpawnVortice(15.0f),
    tiempoSpawnEscombro(0.0f),
    intervaloSpawnEscombro(3.0f),
    maxVortices(2),
    escombrosEvitados(0),
    vorticesEvitados(0) {

    // Configurar nivel
    anchoNivel = 1200;
    altoNivel = 1000;
    tiempoLimite = 0.0f;  // Sin limite de tiempo

    // Actualizar limites de camara
    camara->setLimites(Vector2D(0, 0), Vector2D(anchoNivel, altoNivel));
    camara->setTieneScroll(true);

    // Inicializar
    inicializar();
}

Nivel3Submarino::~Nivel3Submarino() {
    vortices.clear();
    escombros.clear();
}

// ========== INICIALIZACION ==========

void Nivel3Submarino::inicializar() {
    // Crear jugador (pequeño)
    jugador = new Jugador(Vector2D(400, profundidadInicial));
    jugador->setDimensiones(48, 32);
    motorFisica->agregarEntidad(jugador);

    // Spawnear escombros iniciales
    for (int i = 0; i < 10; i++) {
        spawnearEscombro();
    }

    // Spawnear vortice inicial
    spawnearVortice();

    // Configurar gravedad (agua = menor gravedad)
    motorFisica->setGravedadActiva(true);
    motorFisica->setGravedad(2.0f); // Gravedad reducida bajo el agua
}

// ========== ACTUALIZACION ==========

void Nivel3Submarino::actualizar(float dt) {
    if (completado || fallado) return;

    // Actualizar tiempo
    tiempoTranscurrido += dt;

    // Actualizar fisica
    motorFisica->actualizar(dt);

    // Actualizar vortices
    actualizarVortices(dt);

    // Actualizar escombros
    actualizarEscombros(dt);

    // Aplicar fuerzas de vortices
    aplicarFuerzasVortice();

    // Spawning de vortices
    tiempoSpawnVortice += dt;
    if (tiempoSpawnVortice >= intervaloSpawnVortice) {
        if ((int)vortices.size() < maxVortices) {
            spawnearVortice();
        }
        tiempoSpawnVortice = 0.0f;
    }

    // Spawning de escombros
    tiempoSpawnEscombro += dt;
    if (tiempoSpawnEscombro >= intervaloSpawnEscombro) {
        spawnearEscombro();
        tiempoSpawnEscombro = 0.0f;
    }

    // Actualizar profundidad actual
    if (jugador) {
        profundidadActual = jugador->getPosicion().y;

        // Actualizar camara (seguir al jugador verticalmente)
        camara->seguirObjetivo(jugador->getPosicion(), dt);
    }

    // Verificar condiciones
    if (verificarVictoria()) {
        completado = true;
    }

    if (verificarDerrota()) {
        fallado = true;
    }
}

void Nivel3Submarino::actualizarVortices(float dt) {
    // Actualizar y limpiar vortices
    auto it = vortices.begin();

    while (it != vortices.end()) {
        Vortice* vortice = *it;

        if (!vortice || !vortice->estaActivo()) {
            // Vortice expirado
            it = vortices.erase(it);
            vorticesEvitados++;
            agregarPuntos(30); // 30 puntos por sobrevivir a un vortice
        } else {
            ++it;
        }
    }
}

void Nivel3Submarino::actualizarEscombros(float dt) {
    // Limpiar escombros inactivos
    auto it = escombros.begin();

    while (it != escombros.end()) {
        Escombro* escombro = *it;

        if (!escombro || !escombro->estaActivo()) {
            it = escombros.erase(it);
            escombrosEvitados++;
            agregarPuntos(5); // 5 puntos por evitar
        } else {
            ++it;
        }
    }
}

void Nivel3Submarino::aplicarFuerzasVortice() {
    // Aplicar fuerza de vortices al jugador y escombros
    if (!jugador) return;

    for (auto* vortice : vortices) {
        if (vortice && vortice->estaActivo()) {
            // Aplicar al jugador
            vortice->aplicarFuerzaA(jugador);

            // Aplicar a escombros
            for (auto* escombro : escombros) {
                if (escombro && escombro->estaActivo()) {
                    vortice->aplicarFuerzaA(escombro);
                }
            }
        }
    }
}

void Nivel3Submarino::spawnearVortice() {
    // Spawnear vortice en posicion aleatoria
    float x = 200.0f + (rand() % 800);
    float y = 200.0f + (rand() % 600);

    Vortice* vortice = new Vortice(Vector2D(x, y));

    motorFisica->agregarEntidad(vortice);
    vortices.push_back(vortice);
}

void Nivel3Submarino::spawnearEscombro() {
    // Spawnear escombro aleatorio

    // Tipo aleatorio
    TipoEscombro tipo;
    int tipoRand = rand() % 4;
    switch (tipoRand) {
    case 0: tipo = TipoEscombro::MADERA; break;
    case 1: tipo = TipoEscombro::METAL_PESADO; break;
    case 2: tipo = TipoEscombro::SALVAVIDAS; break;
    default: tipo = TipoEscombro::NEUTRO; break;
    }

    // Posicion aleatoria
    float x = 100.0f + (rand() % 1000);
    float y = 100.0f + (rand() % 800);

    Escombro* escombro = new Escombro(Vector2D(x, y), tipo);

    motorFisica->agregarEntidad(escombro);
    escombros.push_back(escombro);
}

// ========== RENDERIZADO ==========

void Nivel3Submarino::renderizar(QPainter& painter) {
    if (!camara) return;

    painter.save();

    // ===== FONDO: OCEANO PROFUNDO =====
    int profundidadRelativa = (int)((profundidadActual / profundidadInicial) * 255);
    int azul = 150 - profundidadRelativa / 2;
    painter.fillRect(0, 0, 800, 600, QColor(0, profundidadRelativa / 3, azul));

    // Burbujas decorativas
    painter.setPen(QPen(QColor(200, 200, 255, 100), 2));
    for (int i = 0; i < 10; i++) {
        int x = (rand() % 800);
        int y = ((int)tiempoTranscurrido * 50 + i * 60) % 600;
        painter.drawEllipse(x, y, 8, 8);
    }

    // ===== RENDERIZAR ENTIDADES (con camara) =====
    Vector2D offsetCamara = camara->getPosicion();
    painter.translate(-offsetCamara.x, -offsetCamara.y);

    // Linea de superficie (objetivo)
    painter.setPen(QPen(QColor(255, 255, 0, 180), 3, Qt::DashLine));
    painter.drawLine(0, profundidadObjetivo, anchoNivel, profundidadObjetivo);
    painter.drawText(10 + offsetCamara.x, 30 + profundidadObjetivo, "SUPERFICIE");

    // Renderizar todas las entidades
    for (auto* entidad : motorFisica->getEntidades()) {
        if (entidad && entidad->estaActivo()) {
            entidad->renderizar(painter);
        }
    }

    painter.restore();

    // ===== HUD (sin offset de camara) =====
    painter.setPen(Qt::white);
    painter.drawText(10, 20, QString("Tiempo: %1 s").arg((int)tiempoTranscurrido));
    painter.drawText(10, 40, QString("Profundidad: %1 / %2 m")
                                 .arg((int)profundidadActual)
                                 .arg((int)profundidadInicial));

    // Barra de profundidad visual
    int barraAltura = 200;
    int barraPosY = 150;

    painter.setBrush(QColor(50, 50, 50, 180));
    painter.drawRect(10, barraPosY, 20, barraAltura);

    // Progreso (invertido: arriba = superficie)
    float progreso = 1.0f - (profundidadActual / profundidadInicial);
    int alturaProgreso = (int)(barraAltura * progreso);

    painter.setBrush(QColor(0, 255, 0, 200));
    painter.drawRect(10, barraPosY + barraAltura - alturaProgreso, 20, alturaProgreso);

    painter.setPen(Qt::white);
    painter.drawText(10, 370, QString("Puntos: %1").arg(puntuacion));
    painter.drawText(10, 390, QString("Vidas: %1").arg(jugador ? jugador->getVidas() : 0));
}

// ========== REINICIAR ==========

void Nivel3Submarino::reiniciar() {
    // Limpiar todo
    limpiar();

    // Reiniciar variables
    completado = false;
    fallado = false;
    tiempoTranscurrido = 0.0f;
    puntuacion = 0;
    profundidadActual = profundidadInicial;
    escombrosEvitados = 0;
    vorticesEvitados = 0;
    tiempoSpawnVortice = 0.0f;
    tiempoSpawnEscombro = 0.0f;

    vortices.clear();
    escombros.clear();

    // Reinicializar
    motorFisica = new MotorFisica();
    camara = new Camara(800, 600);
    camara->setLimites(Vector2D(0, 0), Vector2D(anchoNivel, altoNivel));

    inicializar();
}

// ========== CONDICIONES ==========

bool Nivel3Submarino::verificarVictoria() {
    // Victoria: llegar a la superficie (y < 50)
    return jugador && jugador->getPosicion().y < 50.0f;
}

bool Nivel3Submarino::verificarDerrota() {
    // Derrota: quedarse sin vidas
    return jugador && jugador->estaMuerto();
}


void Nivel3Submarino::manejarInput(int tecla, bool presionada) {
    if (!jugador) return;

    (void)tecla;
    (void)presionada;
}
