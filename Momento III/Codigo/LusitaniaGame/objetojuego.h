#ifndef OBJETOJUEGO_H
#define OBJETOJUEGO_H

#include "entidad.h"
#include "tipos.h"
#include "movimientocircular.h"
#include <QPainter>

class ObjetoJuego : public Entidad {
private:
    TipoObjeto tipoObjeto;

    // Fisica segun tipo
    float masa;
    float friccion;
    bool enSuelo;
    bool suspendido;

    // Movimiento por inclinacion
    float anguloBarco;
    float aceleracionDeslizamiento;

    // Fisica de lampara (MCU)
    MovimientoCircular* movCircular;
    Vector2D puntoAnclaje;
    float longitudCuerda;

    // Daño
    float danioContacto;

    void configurarSegunTipo();
    void actualizarPorInclinacion(float dt);

public:
    // ========== CONSTRUCTORES ==========
    ObjetoJuego();
    ObjetoJuego(const Vector2D& pos, TipoObjeto tipo);
    ~ObjetoJuego();

    // ========== METODOS HEREDADOS ==========
    void actualizar(float dt) override;
    void renderizar(QPainter& painter) override;
    void onColision(Entidad* otra) override;

    // ========== METODOS ESPECIFICOS ==========
    void setAnguloBarco(float angulo);
    void setLongitudCuerda(float longitud) { longitudCuerda = longitud; }
    void colgar(const Vector2D& anclaje);
    void soltar();

    // ========== GETTERS ==========
    TipoObjeto getTipoObjeto() const;
    TipoObjeto getTipo() const { return tipoObjeto; }
    float getMasa() const;
    bool estaSuspendido() const;
    bool estaEnSuelo() const;
    float getDanioContacto() const;
};

#endif // OBJETOJUEGO_H
