#ifndef SISTEMAAPRENDIZAJE_H
#define SISTEMAAPRENDIZAJE_H

#include "datosia.h"
#include <vector>

// Sistema de Aprendizaje
// Aprende de los disparos pasados para mejorar la precision
// Ajusta el perfil de IA segun el desempeño
class SistemaAprendizaje {
private:
    std::vector<DatosDisparo> historialDisparos;  // Ultimos N disparos
    int maxHistorial;                              // Maximo 20 disparos guardados

    // Estadisticas
    int totalDisparos;
    int totalAciertos;
    float tasaAcierto;                             // % de aciertos

    // Aprendizaje
    float factorAprendizaje;                       // 0.1 (10% de ajuste por iteracion)

    // Analisis de disparos
    void analizarDisparo(const DatosDisparo& disparo);
    float calcularErrorPromedio();
    void ajustarPerfil(PerfilIA& perfil);

public:
    // ========== CONSTRUCTOR ==========
    SistemaAprendizaje();
    SistemaAprendizaje(int maxHist);
    ~SistemaAprendizaje();

    // ========== METODOS PRINCIPALES ==========

    // Registra un disparo y aprende de el
    void registrarDisparo(const DatosDisparo& disparo);

    // Actualiza el perfil de IA basandose en el historial
    void aprenderYMejorar(PerfilIA& perfil);

    // ========== ESTADISTICAS ==========
    float getTasaAcierto() const;
    int getTotalDisparos() const;
    int getTotalAciertos() const;

    // ========== UTILIDADES ==========
    void reiniciar();
    void limpiarHistorial();
};

#endif // SISTEMAAPRENDIZAJE_H
