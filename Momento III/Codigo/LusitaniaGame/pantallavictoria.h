#ifndef PANTALLAVICTORIA_H
#define PANTALLAVICTORIA_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class PantallaVictoria : public QWidget {
    Q_OBJECT

public:
    explicit PantallaVictoria(QWidget *parent = nullptr);

    void mostrarResultados(int puntuacionFinal, int nivelCompletado);

protected:
    void paintEvent(QPaintEvent* event) override;

signals:
    void volverAlMenu();
    void jugarDeNuevo();

private:
    QLabel* lblTitulo;
    QLabel* lblPuntuacion;
    QLabel* lblNivel;
    QLabel* lblMensaje;
    QPushButton* btnMenu;
    QPushButton* btnReintentar;

    int puntuacion;
    int nivel;

    void configurarUI();
};

#endif // PANTALLAVICTORIA_H
