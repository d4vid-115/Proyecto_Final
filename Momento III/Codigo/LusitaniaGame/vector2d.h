#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>

// Clase para manejar vectores 2D (posición, velocidad, fuerzas)
// Usada en TODAS las entidades del juego
class Vector2D {
public:
    float x, y;  // Componentes del vector

    // ========== CONSTRUCTORES ==========
    Vector2D();                        // Constructor por defecto (0, 0)
    Vector2D(float _x, float _y);      // Constructor con valores

    // ========== OPERADORES ARITMETICOS ==========
    // Estos permiten hacer: posicion = posicion + velocidad
    Vector2D operator+(const Vector2D& otro) const;  // Suma
    Vector2D operator-(const Vector2D& otro) const;  // Resta
    Vector2D operator*(float escalar) const;          // Multiplicacion por escalar
    Vector2D operator/(float escalar) const;          // Division por escalar

    // Operadores de asignacion compuesta (+=, -=, *=)
    Vector2D& operator+=(const Vector2D& otro);
    Vector2D& operator-=(const Vector2D& otro);
    Vector2D& operator*=(float escalar);

    // ========== METODOS UTILES ==========
    float magnitud() const;              // Longitud del vector: (x^2 + y^2)^1/2
    float magnitudCuadrada() const;      // x^2 + y^2 (mas rapido, evita sqrt)
    Vector2D normalizado() const;        // Vector unitario (longitud = 1)
    void normalizar();                   // Normaliza este vector

    float distanciaA(const Vector2D& otro) const;     // Distancia a otro punto
    float productoEscalar(const Vector2D& otro) const; // Producto escalar
    float angulo() const;                             // Angulo en radianes

    // Crea vector desde angulo (util para disparos)
    static Vector2D desdeAngulo(float angulo, float magnitud = 1.0f);

    // Rota el vector
    Vector2D rotado(float angulo) const;
};

#endif // VECTOR2D_H
