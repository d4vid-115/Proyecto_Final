#ifndef NIVEL3SUBMARINO_H
#define NIVEL3SUBMARINO_H

#include "nivel.h"
#include "vortice.h"
#include "escombro.h"
#include <vector>

// Nivel 3: Escape del Fondo Marino
// Vista: Lateral submarina
// Objetivo: Llegar a la superficie evitando vortices
// Fisicas: Flotacion (Arquimedes), Vortice (fuerza radial)
class Nivel3Submarino : public Nivel {
private:
    // ===== ASCENSO =====
    float profundidadInicial;           // 800 px
    float profundidadObjetivo;          // 0 px (superficie)
    float profundidadActual;            // Profundidad del jugador

    // ===== ENTIDADES DEL NIVEL =====
    std::vector<Vortice*> vortices;
    std::vector<Escombro*> escombros;

    // ===== SPAWNING =====
    float tiempoSpawnVortice;
    float intervaloSpawnVortice;        // Cada 15 segundos
    float tiempoSpawnEscombro;
    float intervaloSpawnEscombro;       // Cada 3 segundos

    int maxVortices;                    // Maximo 2 vortices simultaneos

    // ===== CONDICIONES =====
    int escombrosEvitados;
    int vorticesEvitados;

    // Metodos auxiliares
    void spawnearVortice();
    void spawnearEscombro();
    void actualizarVortices(float dt);
    void actualizarEscombros(float dt);
    void aplicarFuerzasVortice();

public:
    // ========== CONSTRUCTOR ==========
    Nivel3Submarino();
    ~Nivel3Submarino();

    // ========== METODOS HEREDADOS ==========
    void inicializar() override;
    void actualizar(float dt) override;
    void renderizar(QPainter& painter) override;
    void reiniciar() override;

    bool verificarVictoria() override;
    bool verificarDerrota() override;

    void manejarInput(int tecla, bool presionada) override;
};

#endif // NIVEL3SUBMARINO_H
