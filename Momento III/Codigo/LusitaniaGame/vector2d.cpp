#include "vector2d.h"

// ========== CONSTRUCTORES ==========

Vector2D::Vector2D() : x(0.0f), y(0.0f) {
    // Constructor vacio: crea vector (0, 0)
}

Vector2D::Vector2D(float _x, float _y) : x(_x), y(_y) {
    // Constructor con valores especificos
}

// ========== OPERADORES ARITMETICOS ==========

Vector2D Vector2D::operator+(const Vector2D& otro) const {
    // Suma vectorial: (x1, y1) + (x2, y2) = (x1+x2, y1+y2)
    return Vector2D(x + otro.x, y + otro.y);
}

Vector2D Vector2D::operator-(const Vector2D& otro) const {
    // Resta vectorial: util para calcular direccion entre dos puntos
    return Vector2D(x - otro.x, y - otro.y);
}

Vector2D Vector2D::operator*(float escalar) const {
    // Multiplicacion por escalar: cambia la magnitud del vector
    return Vector2D(x * escalar, y * escalar);
}

Vector2D Vector2D::operator/(float escalar) const {
    // Division por escalar (con proteccion contra division por cero)
    if (escalar != 0.0f)
        return Vector2D(x / escalar, y / escalar);
    return Vector2D(0, 0);
}

// ========== OPERADORES DE ASIGNACION ==========

Vector2D& Vector2D::operator+=(const Vector2D& otro) {
    // Permite hacer: posicion += velocidad
    x += otro.x;
    y += otro.y;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& otro) {
    x -= otro.x;
    y -= otro.y;
    return *this;
}

Vector2D& Vector2D::operator*=(float escalar) {
    x *= escalar;
    y *= escalar;
    return *this;
}

// ========== METODOS UTILES ==========

float Vector2D::magnitud() const {
    // Teorema de Pitagoras: longitud = (x^2 + y^2)^1/2
    // Util para distancias, velocidades
    return std::sqrt(x * x + y * y);
}

float Vector2D::magnitudCuadrada() const {
    // x^2 + y^2 (sin raiz cuadrada, mas rapido)
    // Util para comparar distancias sin el costo de sqrt()
    return x * x + y * y;
}

Vector2D Vector2D::normalizado() const {
    // Devuelve vector con la misma direccion pero longitud = 1
    // Util para direcciones de movimiento
    float mag = magnitud();
    if (mag > 0.0f)
        return Vector2D(x / mag, y / mag);
    return Vector2D(0, 0);
}

void Vector2D::normalizar() {
    // Normaliza este vector (lo modifica)
    float mag = magnitud();
    if (mag > 0.0f) {
        x /= mag;
        y /= mag;
    }
}

float Vector2D::distanciaA(const Vector2D& otro) const {
    // Distancia euclidiana entre dos puntos
    // dist = |A - B|
    return (*this - otro).magnitud();
}

float Vector2D::productoEscalar(const Vector2D& otro) const {
    // Producto escalar: A·B = Ax*Bx + Ay*By
    // Util para calcular angulos, proyecciones
    return x * otro.x + y * otro.y;
}

float Vector2D::angulo() const {
    // Devuelve el angulo del vector en radianes
    return std::atan2(y, x);
}

Vector2D Vector2D::desdeAngulo(float angulo, float magnitud) {
    // Crea un vector desde un angulo (util para disparos)
    // x = magnitud * cos(angulo)
    // y = magnitud * sin(angulo)
    return Vector2D(std::cos(angulo) * magnitud, std::sin(angulo) * magnitud);
}

Vector2D Vector2D::rotado(float angulo) const {
    // Rota el vector por un angulo dado
    // Usa matriz de rotación 2D:
    // [cos θ  -sin θ] [x]
    // [sin θ   cos θ] [y]
    float cosA = std::cos(angulo);
    float sinA = std::sin(angulo);
    return Vector2D(x * cosA - y * sinA, x * sinA + y * cosA);
}
