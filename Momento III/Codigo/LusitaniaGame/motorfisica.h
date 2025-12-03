#ifndef MOTORFISICA_H
#define MOTORFISICA_H

#include "entidad.h"
#include "tipos.h"
#include <vector>

// Motor de Fisica
// Gestiona todas las entidades del juego
// Aplica fisica y detecta colisiones
class MotorFisica {
private:
    std::vector<Entidad*> entidades;    // Todas las entidades activas
    float gravedad;                      // Gravedad global (9.8 px/s²)
    bool gravedadActiva;                 // Si la gravedad esta activada

    // Colisiones
    void verificarColisiones();
    void resolverColision(Entidad* e1, Entidad* e2);

    // Limpieza
    void eliminarEntidadesInactivas();

public:
    // ========== CONSTRUCTOR ==========
    MotorFisica();
    MotorFisica(float g);
    ~MotorFisica();

    // ========== CICLO PRINCIPAL ==========
    void actualizar(float dt);
    void aplicarFisica(float dt);
    void aplicarGravedad(float dt);

    // ========== GESTION DE ENTIDADES ==========
    void agregarEntidad(Entidad* e);
    void removerEntidad(Entidad* e);
    void limpiarEntidades();

    // Busqueda de entidades
    std::vector<Entidad*> obtenerEntidadesPorTipo(TipoEntidad tipo);
    Entidad* obtenerPrimeraEntidadPorTipo(TipoEntidad tipo);
    int contarEntidadesPorTipo(TipoEntidad tipo);

    // ========== GETTERS ==========
    std::vector<Entidad*> getEntidades() const;
    int getNumeroEntidades() const;
    float getGravedad() const;
    bool getGravedadActiva() const;

    // ========== SETTERS ==========
    void setGravedad(float g);
    void setGravedadActiva(bool activa);
};

#endif // MOTORFISICA_H
