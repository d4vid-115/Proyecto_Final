#ifndef FISICAFLOTACION_H
#define FISICAFLOTACION_H

#include "componentefisica.h"
#include "entidad.h"

// Fisica de Flotacion (Principio de Arquimedes)
// Usado para: Escombros flotando/hundiendose (Nivel 3)
// Ecuacion: a = (ρ_agua - ρ_objeto) × g / ρ_objeto
//
// Densidades:
//   - Agua: ρ = 1.0 (referencia)
//   - Madera: ρ = 0.6 (flota rapido)
//   - Metal: ρ = 7.8 (se hunde rapido)
//   - Salvavidas: ρ = 0.3 (flota muy rapido)
//   - Neutro: ρ = 1.0 (flotacion neutra)
class FisicaFlotacion : public ComponenteFisica {
private:
    float densidadObjeto;        // ρ_objeto (rho)
    float densidadAgua;          // ρ_agua = 1.0
    float gravedad;              // g = 9.8 (simulada)
    float velocidadTerminal;     // Velocidad maxima de hundimiento/flotacion
    float friccion;              // Resistencia del agua (0.95)

    // Calcula la aceleracion de flotacion
    float calcularAceleracion() const;

public:
    // ========== CONSTRUCTORES ==========
    FisicaFlotacion();
    FisicaFlotacion(float densidad);
    ~FisicaFlotacion();

    // ========== METODOS HEREDADOS ==========
    Vector2D calcular(Entidad* e, float dt) override;
    void aplicar(Entidad* e) override;

    // ========== GETTERS Y SETTERS ==========
    float getDensidadObjeto() const;
    float getVelocidadTerminal() const;
    void setDensidadObjeto(float densidad);
    void setVelocidadTerminal(float vt);

    // ========== UTILIDADES ==========
    bool estaFlotando() const;   // true si densidad < 1.0
    bool estaHundiendo() const;  // true si densidad > 1.0
};

#endif // FISICAFLOTACION_H
