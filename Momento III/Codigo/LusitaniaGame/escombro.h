#ifndef ESCOMBRO_H
#define ESCOMBRO_H

#include "entidad.h"
#include "fisicaflotacion.h"
#include "tipos.h"
#include <QPainter>

class Escombro : public Entidad {
private:
    TipoEscombro tipoEscombro;
    float densidad;
    FisicaFlotacion* flotacion;
    bool enAgua;
    float velocidadTerminal;

    void configurarDensidad();

public:
    Escombro();
    Escombro(const Vector2D& pos, TipoEscombro tipo);
    ~Escombro();

    void actualizar(float dt) override;
    void renderizar(QPainter& painter) override;
    void onColision(Entidad* otra) override;

    void flotar();
    void hundir();
    bool estaFlotando() const;
    bool estaHundiendo() const;

    TipoEscombro getTipo() const { return tipoEscombro; }
    float getDensidad() const;
    bool estaEnAgua() const;

    void setTipoEscombro(TipoEscombro tipo);
    void setEnAgua(bool agua);
};

#endif // ESCOMBRO_H
