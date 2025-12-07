#ifndef NIVEL2BARCO_H
#define NIVEL2BARCO_H

#include "nivel.h"
#include "npc.h"
#include "objetojuego.h"
#include <vector>

class Nivel2Barco : public Nivel {
private:
    // Hundimiento
    float anguloBarco;
    float velocidadInclinacion;
    float tiempoHundimiento;
    float tiempoLimiteHundimiento;

    // Zona de rescate
    Vector2D zonaRescate;

    // NPCs
    std::vector<NPC*> npcs;
    int npcsRescatados;
    int npcsMuertos;
    int npcsTotales;
    int objetivoNPCs;

    // Objetos
    std::vector<ObjetoJuego*> objetos;
    float tiempoSpawnObjeto;
    float intervaloSpawnObjeto;

    // Cuartos del barco
    std::vector<Vector2D> cuartos;

    void crearNPCs();
    void crearObjetos();
    void spawnearObjetoAleatorio();
    void respawnearNPC();

    void actualizarInclinacion(float dt);
    void actualizarNPCs(float dt);
    void actualizarObjetos(float dt);
    void verificarRescates();
    void verificarColisionesObjetos();

public:
    Nivel2Barco();
    ~Nivel2Barco();

    void inicializar() override;
    void actualizar(float dt) override;
    void renderizar(QPainter& painter) override;
    void reiniciar() override;

    bool verificarVictoria() override;
    bool verificarDerrota() override;

    void manejarInput(int tecla, bool presionada) override;

    float getAnguloBarco() const { return anguloBarco; }
    int getNPCsRescatados() const { return npcsRescatados; }
    int getNPCsMuertos() const { return npcsMuertos; }
    int getObjetivoNPCs() const { return objetivoNPCs; }
};

#endif // NIVEL2BARCO_H
