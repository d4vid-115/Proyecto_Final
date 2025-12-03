#ifndef NIVEL1OCEANO_H
#define NIVEL1OCEANO_H

#include "nivel.h"
#include "submarino.h"
#include "torpedo.h"
#include "osciladorarmonico.h"
#include <vector>

// Nivel 1: El Naufragio del Lusitania
// Vista: Cenital (top-down)
// Objetivo: Sobrevivir 3 minutos evitando submarinos
// Fisicas: MAS (barco oscilando), Parabolica (torpedos)
class Nivel1Oceano : public Nivel {
private:
    // ===== ENTIDADES DEL NIVEL =====
    std::vector<Submarino*> submarinos;
    std::vector<Torpedo*> torpedos;

    // ===== BARCO LUSITANIA =====
    Vector2D posicionBarco;
    OsciladorArmonico* oscilacionBarco;

    // ===== SPAWNING DE ENEMIGOS =====
    float tiempoSpawn;
    float intervaloSpawn;              // Cada 20 segundos
    int maxSubmarinos;                 // Maximo 3 submarinos simultaneos

    // ===== CONDICIONES DE VICTORIA =====
    int submarinosDestruidos;
    int torpedosEvadidos;

    // Metodos auxiliares
    void spawnearSubmarino();
    void actualizarSubmarinos(float dt);
    void actualizarTorpedos(float dt);
    void verificarColisionesTorpedos();

public:
    // ========== CONSTRUCTOR ==========
    Nivel1Oceano();
    ~Nivel1Oceano();

    // ========== METODOS HEREDADOS ==========
    void inicializar() override;
    void actualizar(float dt) override;
    void renderizar(QPainter& painter) override;
    void reiniciar() override;

    bool verificarVictoria() override;
    bool verificarDerrota() override;

    void manejarInput(int tecla, bool presionada) override;
};

#endif // NIVEL1OCEANO_H
