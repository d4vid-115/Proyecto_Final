#ifndef TIPOS_H
#define TIPOS_H

// ============================================
// ENUMERACIONES DEL JUEGO
// Agrupan constantes relacionadas y evitan "magic numbers"
// ============================================

// Estado general del juego
enum class EstadoJuego {
    MENU_PRINCIPAL,      // Pantalla de inicio
    JUGANDO,             // En medio del juego
    PAUSADO,             // Juego en pausa
    GAME_OVER,           // Perdiste
    VICTORIA,            // Ganaste
    TRANSICION_NIVEL     // Cambiando de nivel
};

// Tipo de cada entidad (para identificar en colisiones)
enum class TipoEntidad {
    JUGADOR,
    ENEMIGO_SUBMARINO,
    TORPEDO,
    NPC,
    OBJETO,
    ESCOMBRO,
    VORTICE
};

// Estado del submarino enemigo
enum class EstadoSubmarino {
    PATRULLANDO,    // Movimiento normal
    DETECTANDO,     // Ha visto al jugador
    ATACANDO,       // Disparando torpedos
    EVADIENDO       // Muy cerca, alejandose
};

// Estado de los NPCs (Nivel 2)
enum class EstadoNPC {
    PANICO,         // Movimiento erratico
    SIGUIENDO,      // Siguiendo al jugador
    RESCATADO,      // Ya llego a la salida
    CAIDO           // Fue golpeado por objeto
};

// Tipo de NPC (afecta sprite y velocidad)
enum class TipoNPC {
    HOMBRE,
    MUJER,
    NINO
};

// Tipo de objeto dinamico (Nivel 2)
enum class TipoObjeto {
    MALETA,
    SILLA,
    LAMPARA,
    ESCOMBRO_PEQUENO,
    ESCOMBRO_GRANDE
};

// Tipo de escombro (Nivel 3 - afecta densidad)
enum class TipoEscombro {
    MADERA,          // ρ = 0.6 (flota rapido)
    METAL_PESADO,    // ρ = 7.8 (se hunde rapido)
    SALVAVIDAS,      // ρ = 0.3 (flota muy rapido)
    NEUTRO           // ρ = 1.0 (flotacion neutra)
};

// Estado de la IA
enum class EstadoIA {
    PATRULLANDO,
    ACERCANDOSE,
    ATACANDO,
    EVADIENDO
};

// ============================================
// ESTRUCTURAS DE DATOS
// ============================================

// Caja de colision rectangular (AABB - Axis-Aligned Bounding Box)
struct CajaColision {
    float x, y;          // Posicion
    float ancho, alto;   // Dimensiones

    CajaColision() : x(0), y(0), ancho(0), alto(0) {}

    CajaColision(float _x, float _y, float _ancho, float _alto)
        : x(_x), y(_y), ancho(_ancho), alto(_alto) {}

    // Detecta colision con otraa caja
    bool colisionaCon(const CajaColision& otra) const {
        // No hay colision si:
        // - Este esta completamente a la izquierda del otro
        // - Este esta completamente a la derecha del otro
        // - Este esta completamente arriba del otro
        // - Este esta completamente abajo del otro
        return !(x + ancho < otra.x ||
                 x > otra.x + otra.ancho ||
                 y + alto < otra.y ||
                 y > otra.y + otra.alto);
    }
};

// Estructura para sprites (simplificada por ahora)
struct Sprite {
    // Path de la imagen
    QString rutaImagen;

    // Dimensiones
    int ancho, alto;

    // Animacion
    int frameActual;
    int totalFrames;

    Sprite() : ancho(0), alto(0), frameActual(0), totalFrames(1) {}
};

#endif // TIPOS_H
