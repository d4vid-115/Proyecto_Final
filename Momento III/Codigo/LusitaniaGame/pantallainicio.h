#ifndef PANTALLAINICIO_H
#define PANTALLAINICIO_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPixmap>
#include <QRect>
#include <QPushButton>

// Estructura para botones del menu
struct Boton {
    QRect rect;
    QString texto;
    bool hover;

    Boton() : hover(false) {}
    Boton(int x, int y, int w, int h, const QString& t)
        : rect(x, y, w, h), texto(t), hover(false) {}
};

class PantallaInicio : public QWidget {
    Q_OBJECT

public:
    explicit PantallaInicio(QWidget *parent = nullptr);
    ~PantallaInicio();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void iniciarJuego(int nivel);
    void abrirOpciones();
    void salir();

private:
    QPixmap fondoMenu;
    bool menuPrincipal;

    // Botones del menu principal
    Boton botonJugar;
    Boton botonOpciones;
    Boton botonSalir;

    // Botones de seleccion de nivel
    Boton botonNivel1;
    Boton botonNivel2;
    Boton botonNivel3;

    void dibujarBoton(QPainter& painter, const Boton& boton);
    void verificarHover(const QPoint& pos);
    void mostrarComoJugar();
};

#endif // PANTALLAINICIO_H
