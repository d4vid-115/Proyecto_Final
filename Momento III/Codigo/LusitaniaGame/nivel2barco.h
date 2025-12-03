#ifndef NIVEL2BARCO_H
#define NIVEL2BARCO_H

#include "nivel.h"
#include "npc.h"
#include "objetojuego.h"
#include <vector>

// Nivel 2: El Hundimiento del Barco
// Vista: Lateral (side-view)
// Objetivo: Rescatar 10 NPCs antes de que el barco se hunda
// Fisicas: Plano Inclinado (objetos), MCU (lamparas)
class Nivel2Barco : public Nivel {
private:
    // ===== BARCO Y HUNDIMIENTO =====
    float anguloBarco;                  // Angulo de inclinación (0-30°)
    float velocidadInclinacion;         // Grados/segundo (2°/s)
    float tiempoHundimiento;            // Tiempo hasta hundimiento total
    float tiempoLimiteHundimiento;      // 120 segundos (2 minutos)

    // ===== ENTIDADES DEL NIVEL =====
    std::vector<NPC*> npcs;
    std::vector<ObjetoJuego*> objetos;

    // ===== ZONAS DEL BARCO =====
    Vector2D zonaRescate;               // Donde llevar NPCs (bote salvavidas)
    std::vector<Vector2D> cuartos;      // Ubicaciones de cuartos

    // ===== SPAWNING =====
    float tiempoSpawnObjeto;
    float intervaloSpawnObjeto;         // Cada 5 segundos

    // ===== CONDICIONES DE VICTORIA =====
    int npcsRescatados;
    int npcsTotales;
    int objetivoNPCs;                   // 10 NPCs para ganar

    // Metodos auxiliares
    void crearNPCs();
    void crearObjetos();
    void spawnearObjetoAleatorio();
    void actualizarInclinacion(float dt);
    void actualizarNPCs(float dt);
    void actualizarObjetos(float dt);
    void verificarRescates();

public:
    // ========== CONSTRUCTOR ==========
    Nivel2Barco();
    ~Nivel2Barco();

    // ========== METODOS HEREDADOS ==========
    void inicializar() override;
    void actualizar(float dt) override;
    void renderizar(QPainter& painter) override;
    void reiniciar() override;

    bool verificarVictoria() override;
    bool verificarDerrota() override;

    void manejarInput(int tecla, bool presionada) override;
};

#endif // NIVEL2BARCO_H
