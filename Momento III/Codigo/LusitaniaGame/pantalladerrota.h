#ifndef PANTALLADERROTA_H
#define PANTALLADERROTA_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class PantallaDerrota : public QWidget {
    Q_OBJECT

public:
    explicit PantallaDerrota(QWidget *parent = nullptr);

    void mostrarResultados(int puntuacionFinal, int nivelPerdido);

protected:
    void paintEvent(QPaintEvent* event) override;

signals:
    void volverAlMenu();
    void reintentar();

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

#endif // PANTALLADERROTA_H
