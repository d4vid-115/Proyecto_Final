#ifndef PANTALLAINICIO_H
#define PANTALLAINICIO_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QRect>
#include <QFont>

class PantallaInicio : public QWidget {
    Q_OBJECT

private:
    QPixmap fondoMenu;

    // Botones
    struct Boton {
        QRect rect;
        QString texto;
        bool hover;

        Boton() : hover(false) {}
        Boton(int x, int y, int w, int h, const QString& txt)
            : rect(x, y, w, h), texto(txt), hover(false) {}
    };

    Boton botonJugar;
    Boton botonNivel1;
    Boton botonNivel2;
    Boton botonNivel3;
    Boton botonSalir;
    Boton botonOpciones;

    // Estado
    bool menuPrincipal;

    // Metodos auxiliares
    void dibujarBoton(QPainter& painter, const Boton& boton);
    void verificarHover(const QPoint& pos);

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
};

#endif // PANTALLAINICIO_H
