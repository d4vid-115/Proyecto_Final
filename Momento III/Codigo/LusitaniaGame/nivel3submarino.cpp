#include "nivel3submarino.h"
#include "gestorsprites.h"
#include <QColor>
#include <cstdlib>

// ========== CONSTRUCTOR ==========

Nivel3Submarino::Nivel3Submarino()
    : Nivel(),
    profundidadInicial(3000.0f),
    profundidadObjetivo(0.0f),
    profundidadActual(3000.0f),
    tiempoSpawnVortice(0.0f),
    intervaloSpawnVortice(10.0f),
    tiempoSpawnEscombro(0.0f),
    intervaloSpawnEscombro(0.8f),
    maxVortices(5),
    escombrosEvitados(0),
    vorticesEvitados(0),
    vortices(),
    escombros(),
    objetoActivo(nullptr),
    tiempoObjetoActivo(0.0f),
    duracionEfectoObjeto(0.0f),
    controlsBloqueados(false),
    tiempoSinOxigeno(0.0f) {

    anchoNivel = 1600;
    altoNivel = 3500;
    tiempoLimite = 0.0f;

    camara->setLimites(Vector2D(0, 0), Vector2D(anchoNivel, altoNivel));
    camara->setTieneScroll(true);

    inicializar();
}

Nivel3Submarino::~Nivel3Submarino() {
    vortices.clear();
    escombros.clear();
    objetoActivo = nullptr;
}

// ========== INICIALIZACION ==========

void Nivel3Submarino::inicializar() {
    // Crear jugador en el FONDO
    jugador = new Jugador(Vector2D(800, profundidadInicial));
    jugador->setDimensiones(48, 48);
    jugador->setOxigeno(150.0f);
    jugador->setSalud(200.0f);
    jugador->setVidas(1);
    motorFisica->agregarEntidad(jugador);

    // Spawnear escombros distribuidos
    for (int i = 0; i < 60; i++) {
        spawnearEscombro();
    }

    // Spawnear vortices iniciales
    spawnearVortice();
    spawnearVortice();
    spawnearVortice();

    motorFisica->setGravedadActiva(true);
    motorFisica->setGravedad(1.2f);
}

// ========== ACTUALIZACION ==========

void Nivel3Submarino::actualizar(float dt) {
    if (completado || fallado) return;

    tiempoTranscurrido += dt;

    // ===== SISTEMA DE OXIGENO CON DAÑO CRECIENTE =====
    if (jugador) {
        float oxigenoActual = jugador->getOxigeno();
        oxigenoActual -= dt * 3.0f;

        if (oxigenoActual < 0.0f) {
            oxigenoActual = 0.0f;

            // ===== DAÑO EXPONENCIAL SIN OXIGENO =====
            tiempoSinOxigeno += dt;

            float danio = 0.0f;
            if (tiempoSinOxigeno < 2.0f) {
                danio = 8.0f;  // Primeros 2 segundos: 8 HP/s
            } else if (tiempoSinOxigeno < 5.0f) {
                danio = 20.0f;  // 2-5 segundos: 20 HP/s
            } else if (tiempoSinOxigeno < 10.0f) {
                danio = 40.0f;  // 5-10 segundos: 40 HP/s
            } else {
                danio = 80.0f;  // Mas de 10 segundos: CRITICO 80 HP/s
            }

            jugador->recibirDanio(danio * dt);
        } else {
            tiempoSinOxigeno = 0.0f;
        }

        jugador->setOxigeno(oxigenoActual);
    }

    if (objetoActivo) {
        tiempoObjetoActivo += dt;

        if (tiempoObjetoActivo >= duracionEfectoObjeto) {
            objetoActivo->destruir();
            objetoActivo = nullptr;
            controlsBloqueados = false;
            tiempoObjetoActivo = 0.0f;
        }
    }

    motorFisica->actualizar(dt);

    actualizarVortices(dt);
    actualizarEscombros(dt);

    // ===== APLICAR FUERZAS DE VORTICE DESPUES DEL MOVIMIENTO =====
    aplicarFuerzasVortice(dt);

    tiempoSpawnVortice += dt;
    if (tiempoSpawnVortice >= intervaloSpawnVortice) {
        if ((int)vortices.size() < maxVortices) {
            spawnearVortice();
        }
        tiempoSpawnVortice = 0.0f;
    }

    tiempoSpawnEscombro += dt;
    if (tiempoSpawnEscombro >= intervaloSpawnEscombro) {
        spawnearEscombro();
        tiempoSpawnEscombro = 0.0f;
    }

    if (jugador) {
        profundidadActual = jugador->getPosicion().y;
        Vector2D posJugador = jugador->getPosicion();
        camara->seguirObjetivo(posJugador, dt);
    }

    if (verificarVictoria()) {
        completado = true;
    }

    if (verificarDerrota()) {
        fallado = true;
    }
}

void Nivel3Submarino::actualizarVortices(float /*dt*/) {
    auto it = vortices.begin();

    while (it != vortices.end()) {
        Vortice* vortice = *it;

        if (!vortice || !vortice->estaActivo()) {
            it = vortices.erase(it);
            vorticesEvitados++;
            agregarPuntos(50);
        } else {
            ++it;
        }
    }
}

void Nivel3Submarino::actualizarEscombros(float /*dt*/) {
    auto it = escombros.begin();

    while (it != escombros.end()) {
        Escombro* escombro = *it;

        if (!escombro || !escombro->estaActivo()) {
            it = escombros.erase(it);
            escombrosEvitados++;
            agregarPuntos(10);
        } else {
            // ===== INTERACCION CON JUGADOR =====
            if (jugador && escombro->colisionaCon(jugador)) {
                aplicarEfectoEscombro(escombro);
            }
            ++it;
        }
    }
}

void Nivel3Submarino::aplicarEfectoEscombro(Escombro* escombro) {
    if (!jugador || !escombro) return;

    if (objetoActivo && objetoActivo != escombro) return;

    switch (escombro->getTipo()) {
    case TipoEscombro::SALVAVIDAS: {
        float oxigenoActual = jugador->getOxigeno();
        jugador->setOxigeno(oxigenoActual + 30.0f);
        agregarPuntos(50);
        escombro->destruir();
        tiempoSinOxigeno = 0.0f;  // Reset contador de daño
        break;
    }

    case TipoEscombro::MADERA: {
        if (!objetoActivo) {
            objetoActivo = escombro;
            duracionEfectoObjeto = 3.0f;
            tiempoObjetoActivo = 0.0f;
            controlsBloqueados = true;

            Vector2D impulso(0, -150.0f);
            jugador->setVelocidad(impulso);
        }
        break;
    }

    case TipoEscombro::METAL_PESADO: {
        if (!objetoActivo) {
            objetoActivo = escombro;
            duracionEfectoObjeto = 2.0f;
            tiempoObjetoActivo = 0.0f;
            controlsBloqueados = true;

            Vector2D impulso(0, 200.0f);
            jugador->setVelocidad(impulso);
            jugador->recibirDanio(5.0f);
        }
        break;
    }

    case TipoEscombro::NEUTRO: {
        break;
    }
    }
}

void Nivel3Submarino::aplicarFuerzasVortice(float dt) {
    if (!jugador) return;

    bool bloqueadoPorVortice = false;
    Vector2D fuerzaTotalVortices(0, 0);

    for (auto* vortice : vortices) {
        if (vortice && vortice->estaActivo()) {
            Vector2D posJugador = jugador->getPosicion();
            Vector2D centroVortice = vortice->getCentro();
            float distancia = posJugador.distanciaA(centroVortice);
            float radio = vortice->getRadio();
            float rangoAtraccion = vortice->getRangoAtraccion();

            if (distancia < rangoAtraccion) {
                // ===== DIRECCION HACIA EL CENTRO =====
                Vector2D direccion = centroVortice - posJugador;
                direccion.normalizar();

                // ===== FACTOR DE FUERZA (MAS FUERTE CERCA DEL CENTRO) =====
                float factorDistancia = 1.0f - (distancia / rangoAtraccion);
                factorDistancia = factorDistancia * factorDistancia;  // Cuadratico

                // ===== FUERZA BASE =====
                float fuerzaBase = 800.0f;

                // ===== VERIFICAR SI EL JUGADOR ESTA INTENTANDO ESCAPAR =====
                Vector2D velJugador = jugador->getVelocidad();
                float velocidadContraVortice = velJugador.productoEscalar(direccion);

                // Si el jugador NO se mueve o se mueve hacia el vortice, aumentar fuerza
                float multiplicadorResistencia = 1.0f;
                if (velocidadContraVortice >= 0) {
                    // No se resiste o va hacia el vortice: fuerza x2
                    multiplicadorResistencia = 2.5f;
                } else {
                    // Se esta resistiendo: fuerza normal (puede escapar)
                    multiplicadorResistencia = 1.0f;
                }

                float fuerzaMagnitud = fuerzaBase * factorDistancia * multiplicadorResistencia;

                // ===== EN EL CENTRO: BLOQUEO TOTAL =====
                if (distancia < radio * 0.4f) {
                    fuerzaMagnitud *= 5.0f;  // Fuerza x5 en el centro
                    bloqueadoPorVortice = true;

                    // Reducir la velocidad del jugador drasticamente
                    jugador->setVelocidad(velJugador * 0.3f);
                }

                Vector2D fuerza = direccion * fuerzaMagnitud;
                fuerzaTotalVortices += fuerza;
            }

            // Aplicar a escombros
            for (auto* escombro : escombros) {
                if (escombro && escombro->estaActivo()) {
                    vortice->aplicarFuerzaA(escombro);
                }
            }
        }
    }

    // ===== APLICAR FUERZA ACUMULADA AL JUGADOR =====
    if (fuerzaTotalVortices.magnitud() > 0.01f) {
        Vector2D velActual = jugador->getVelocidad();
        jugador->setVelocidad(velActual + fuerzaTotalVortices * dt);
    }

    // ===== BLOQUEAR CONTROLES SI ESTA ATRAPADO EN EL CENTRO =====
    if (bloqueadoPorVortice && !objetoActivo) {
        controlsBloqueados = true;
    } else if (!objetoActivo) {
        controlsBloqueados = false;
    }
}

void Nivel3Submarino::spawnearVortice() {
    float x = 200.0f + (rand() % 1200);
    float y = profundidadActual + (rand() % 1000) - 500;

    if (y < 100) y = 100;
    if (y > altoNivel - 100) y = altoNivel - 100;

    Vortice* vortice = new Vortice(Vector2D(x, y), 120.0f, 15.0f);

    motorFisica->agregarEntidad(vortice);
    vortices.push_back(vortice);
}

void Nivel3Submarino::spawnearEscombro() {
    TipoEscombro tipo;
    int tipoRand = rand() % 10;

    if (tipoRand < 4) tipo = TipoEscombro::MADERA;
    else if (tipoRand < 7) tipo = TipoEscombro::METAL_PESADO;
    else if (tipoRand < 9) tipo = TipoEscombro::SALVAVIDAS;
    else tipo = TipoEscombro::NEUTRO;

    float x = 200.0f + (rand() % 1200);
    float y = profundidadActual - 400.0f;

    if (y < 50) y = 50;

    Escombro* escombro = new Escombro(Vector2D(x, y), tipo);

    motorFisica->agregarEntidad(escombro);
    escombros.push_back(escombro);
}

// ========== RENDERIZADO ==========

void Nivel3Submarino::renderizar(QPainter& painter) {
    if (!camara) return;

    painter.save();

    // Fondo mas claro
    QLinearGradient gradiente(0, 0, 0, 600);
    gradiente.setColorAt(0, QColor(20, 60, 100));
    gradiente.setColorAt(0.5, QColor(10, 40, 80));
    gradiente.setColorAt(1, QColor(5, 20, 50));
    painter.fillRect(0, 0, 800, 600, gradiente);

    Vector2D offsetCamara = camara->getPosicion();
    painter.translate(-offsetCamara.x, -offsetCamara.y);

    // Superficie
    painter.setBrush(QColor(100, 200, 255, 180));
    painter.setPen(QPen(QColor(255, 255, 100), 4));
    painter.drawRect(0, -50, anchoNivel, 100);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 24, QFont::Bold));
    painter.drawText(600, 20, "¡SUPERFICIE!");

    // Renderizar escombros
    for (auto* escombro : escombros) {
        if (!escombro || !escombro->estaActivo()) continue;
        escombro->renderizar(painter);
    }

    // Renderizar vortices
    for (auto* vortice : vortices) {
        if (!vortice || !vortice->estaActivo()) continue;

        Vector2D pos = vortice->getCentro();
        float radio = vortice->getRadio();
        float rangoAtraccion = vortice->getRangoAtraccion();
        float rotacion = vortice->getRotacion();

        painter.save();
        painter.translate(pos.x, pos.y);

        for (int i = 0; i < 5; ++i) {
            int r = radio * (5 - i) / 5;
            int alpha = 255 - (i * 40);
            QColor color = QColor(0, 100 + i * 30, 200 - i * 30, alpha);
            painter.setBrush(color);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(-r, -r, r * 2, r * 2);
        }

        painter.setPen(QPen(QColor(100, 200, 255), 3));
        float anguloInicio = rotacion * 180.0f / 3.14159f;

        for (int i = 0; i < 3; ++i) {
            float angulo = anguloInicio + i * 120.0f;
            float x1 = radio * 0.3f * cos(angulo * 3.14159f / 180.0f);
            float y1 = radio * 0.3f * sin(angulo * 3.14159f / 180.0f);
            float x2 = radio * cos(angulo * 3.14159f / 180.0f);
            float y2 = radio * sin(angulo * 3.14159f / 180.0f);
            painter.drawLine(x1, y1, x2, y2);
        }

        painter.restore();

        painter.setPen(QPen(QColor(255, 200, 0, 100), 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(pos.x - rangoAtraccion,
                            pos.y - rangoAtraccion,
                            rangoAtraccion * 2,
                            rangoAtraccion * 2);
    }

    // ===== RENDERIZAR JUGADOR CON ANIMACION =====
    if (jugador) {
        jugador->renderizarNadador(painter);
    }

    painter.restore();
}

// ========== REINICIAR ==========

void Nivel3Submarino::reiniciar() {
    limpiar();

    completado = false;
    fallado = false;
    tiempoTranscurrido = 0.0f;
    puntuacion = 0;
    profundidadActual = profundidadInicial;
    escombrosEvitados = 0;
    vorticesEvitados = 0;
    tiempoSpawnVortice = 0.0f;
    tiempoSpawnEscombro = 0.0f;
    objetoActivo = nullptr;
    tiempoObjetoActivo = 0.0f;
    duracionEfectoObjeto = 0.0f;
    controlsBloqueados = false;
    tiempoSinOxigeno = 0.0f;

    vortices.clear();
    escombros.clear();

    motorFisica = new MotorFisica();
    camara = new Camara(800, 600);
    camara->setLimites(Vector2D(0, 0), Vector2D(anchoNivel, altoNivel));

    inicializar();
}

// ========== CONDICIONES ==========

bool Nivel3Submarino::verificarVictoria() {
    return jugador && jugador->getPosicion().y < 50.0f;
}

bool Nivel3Submarino::verificarDerrota() {
    return jugador && jugador->estaMuerto();
}

void Nivel3Submarino::manejarInput(int /*tecla*/, bool /*presionada*/) {
}

bool Nivel3Submarino::estanControlsBloqueados() const {
    return controlsBloqueados;
}
