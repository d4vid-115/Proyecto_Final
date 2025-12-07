#ifndef ESCOMBRO_H
#define ESCOMBRO_H

#include "entidad.h"
#include "fisicaflotacion.h"
#include "tipos.h"
#include <QPainter>

// Escombro - Objetos flotantes/hundibles en el Nivel 3
// Usa fisica de Arquimedes para flotar o hundirse segun densidad

class Escombro : public Entidad {
private:
    TipoEscombro tipoEscombro;       // MADERA, METAL, SALVAVIDAS, NEUTRO
    float densidad;                   // Densidad del material
    FisicaFlotacion* flotacion;      // Fisica de flotacion
    bool enAgua;                      // Si esta en el agua
    float velocidadTerminal;          // Velocidad maxima de flotacion/hundimiento

    // Configurar densidad segun tipo
    void configurarDensidad();

public:
    // ========== CONSTRUCTORES ==========
    Escombro();
    Escombro(const Vector2D& pos, TipoEscombro tipo);
    ~Escombro();

    // ========== METODOS HEREDADOS ==========
    void actualizar(float dt) override;
    void renderizar(QPainter& painter) override;
    void onColision(Entidad* otra) override;

    // ========== METODOS ESPECIFICOS ==========
    void flotar();
    void hundir();
    bool estaFlotando() const;
    bool estaHundiendo() const;

    // ========== GETTERS ==========
    TipoEscombro getTipo() const { return tipoEscombro; }
    float getDensidad() const;
    bool estaEnAgua() const;

    // ========== SETTERS ==========
    void setTipoEscombro(TipoEscombro tipo);
    void setEnAgua(bool agua);
};

#endif // ESCOMBRO_H
