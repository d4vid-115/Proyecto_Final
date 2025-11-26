#include "fisicaflotacion.h"
#include <algorithm>

// ========== CONSTRUCTORES ==========

FisicaFlotacion::FisicaFlotacion()
    : densidadObjeto(1.0f),    // Neutro por defecto
    densidadAgua(1.0f),       // Agua siempre 1.0
    gravedad(9.8f),           // g simulada
    velocidadTerminal(100.0f), // Maximo 100 px/s
    friccion(0.95f) {         // 5% de perdida por frame
    // Constructor por defecto
}

FisicaFlotacion::FisicaFlotacion(float densidad)
    : densidadObjeto(densidad),
    densidadAgua(1.0f),
    gravedad(9.8f),
    velocidadTerminal(100.0f),
    friccion(0.95f) {
    // Constructor con densidad especifica
}

FisicaFlotacion::~FisicaFlotacion() {
    // Destructor vacio
}

// ========== CALCULO DE LA FISICA ==========

float FisicaFlotacion::calcularAceleracion() const {
    // Principio de Arquimedes simplificado:
    //
    // Fuerza de empuje: F_empuje = ρ_agua × V × g
    // Peso del objeto: F_peso = ρ_objeto × V × g
    // Fuerza neta: F_neta = F_empuje - F_peso
    //           = (ρ_agua - ρ_objeto) × V × g
    //
    // Aceleracion: a = F_neta / masa
    //            = F_neta / (ρ_objeto × V)
    //            = (ρ_agua - ρ_objeto) × g / ρ_objeto
    //
    // Si ρ_objeto < ρ_agua → a > 0 → FLOTA (sube)
    // Si ρ_objeto > ρ_agua → a < 0 → SE HUNDE (baja)
    // Si ρ_objeto = ρ_agua → a = 0 → NEUTRO (quieto)

    return (densidadAgua - densidadObjeto) * gravedad / densidadObjeto;
}

Vector2D FisicaFlotacion::calcular(Entidad* e, float dt) {
    if (!e) return Vector2D(0, 0);

    // Obtener velocidad actual
    Vector2D velocidad = e->getVelocidad();

    // Calcular aceleracion de flotacion
    float aceleracion = calcularAceleracion();

    // Aplicar aceleracion solo en eje Y (vertical)
    // El agua no afecta el movimiento horizontal (simplificacion)
    velocidad.y += aceleracion * dt;

    // Aplicar friccion del agua (ralentiza el movimiento)
    velocidad *= friccion;

    // Limitar a velocidad terminal
    // Esto evita que objetos caigan o suban infinitamente rapido
    if (velocidad.y > velocidadTerminal) {
        velocidad.y = velocidadTerminal;
    } else if (velocidad.y < -velocidadTerminal) {
        velocidad.y = -velocidadTerminal;
    }

    // Calcular nueva posicion: pos = pos + vel × dt
    Vector2D posicionActual = e->getPosicion();
    Vector2D nuevaPosicion = posicionActual + velocidad * dt;

    return nuevaPosicion;
}

void FisicaFlotacion::aplicar(Entidad* e) {
    if (!e) return;

    // Calcular aceleracion
    float aceleracion = calcularAceleracion();

    // Obtener y actualizar velocidad
    Vector2D velocidad = e->getVelocidad();
    velocidad.y += aceleracion * 0.016f; // Asumiendo ~60 FPS
    velocidad *= friccion;

    // Limitar velocidad terminal
    if (velocidad.y > velocidadTerminal) {
        velocidad.y = velocidadTerminal;
    } else if (velocidad.y < -velocidadTerminal) {
        velocidad.y = -velocidadTerminal;
    }

    // Aplicar velocidad
    e->setVelocidad(velocidad);

    // Actualizar posicion
    Vector2D posicion = e->getPosicion();
    posicion += velocidad * 0.016f;
    e->setPosicion(posicion);
}

// ========== GETTERS Y SETTERS ==========

float FisicaFlotacion::getDensidadObjeto() const {
    return densidadObjeto;
}

float FisicaFlotacion::getVelocidadTerminal() const {
    return velocidadTerminal;
}

void FisicaFlotacion::setDensidadObjeto(float densidad) {
    densidadObjeto = densidad;

    // Ajustar velocidad terminal segun densidad
    // Objetos mas densos caen mas rapido
    // Objetos menos densos flotan mas rapido
    if (densidad < densidadAgua) {
        // Flotante: menor densidad = mas rapido
        velocidadTerminal = 50.0f + (densidadAgua - densidad) * 100.0f;
    } else {
        // Hundible: mayor densidad = mas rapido
        velocidadTerminal = 50.0f + (densidad - densidadAgua) * 50.0f;
    }
}

void FisicaFlotacion::setVelocidadTerminal(float vt) {
    velocidadTerminal = vt;
}

// ========== UTILIDADES ==========

bool FisicaFlotacion::estaFlotando() const {
    return densidadObjeto < densidadAgua;
}

bool FisicaFlotacion::estaHundiendo() const {
    return densidadObjeto > densidadAgua;
}
