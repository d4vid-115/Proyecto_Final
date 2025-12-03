#ifndef OBJETOJUEGO_H
#define OBJETOJUEGO_H

#include "entidad.h"
#include "tipos.h"
#include "movimientocircular.h"
#include <QPainter>

// ObjetoJuego - Objetos dinamicos en el Nivel 2
// Afectados por la inclinacion del barco: caen, ruedan, oscilan
// Tipos: maletas, sillas, lamparas, escombros, etc
class ObjetoJuego : public Entidad {
private:
    TipoObjeto tipoObjeto;        // MALETA, SILLA, LAMPARA, etc.

    // Fisica segun tipo
    float masa;                    // Masa del objeto
    float friccion;                // Friccion con el suelo
    bool enSuelo;                  // Si esta tocando el suelo
    bool suspendido;               // Si esta colgando (lampara)

    // Movimiento por inclinacion
    float anguloBarco;             // Angulo de inclinacion del barco (0-30°)
    float aceleracionDeslizamiento; // Gravedad × sin(angulo)

    // Fisica de lampara (MCU)
    MovimientoCircular* movCircular;
    Vector2D puntoAnclaje;         // Punto donde cuelga

    // Daño
    float danioContacto;           // Daño que causa al golpear

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
    void colgar(const Vector2D& anclaje); // Para lamparas
    void soltar();                         // Dejar de colgar

    // ========== GETTERS ==========
    TipoObjeto getTipoObjeto() const;
    float getMasa() const;
    bool estaSuspendido() const;
    bool estaEnSuelo() const;
    float getDanioContacto() const;
};

#endif // OBJETOJUEGO_H
