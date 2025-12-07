QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    agenteia.cpp \
    camara.cpp \
    componentefisica.cpp \
    configuracionsprites.cpp \
    datosia.cpp \
    enemigo.cpp \
    entidad.cpp \
    escombro.cpp \
    fisicaflotacion.cpp \
    fisicavortice.cpp \
    gamewidget.cpp \
    gestorsonidos.cpp \
    gestorsprites.cpp \
    hud.cpp \
    jugador.cpp \
    main.cpp \
    mainwindow.cpp \
    motorfisica.cpp \
    motorjuego.cpp \
    movimientocircular.cpp \
    nivel.cpp \
    nivel1oceano.cpp \
    nivel2barco.cpp \
    nivel3submarino.cpp \
    npc.cpp \
    objetojuego.cpp \
    osciladorarmonico.cpp \
    pantalladerrota.cpp \
    pantallainicio.cpp \
    pantallavictoria.cpp \
    sistemaaccion.cpp \
    sistemaaprendizaje.cpp \
    sistemapercepcion.cpp \
    sistemarazonamiento.cpp \
    submarino.cpp \
    torpedo.cpp \
    trayectoriaparabolica.cpp \
    vector2d.cpp \
    vortice.cpp

HEADERS += \
    agenteia.h \
    camara.h \
    componentefisica.h \
    configuracionsprites.h \
    datosia.h \
    enemigo.h \
    entidad.h \
    escombro.h \
    fisicaflotacion.h \
    fisicavortice.h \
    gamewidget.h \
    gestorsonidos.h \
    gestorsprites.h \
    hud.h \
    jugador.h \
    mainwindow.h \
    motorfisica.h \
    motorjuego.h \
    movimientocircular.h \
    nivel.h \
    nivel1oceano.h \
    nivel2barco.h \
    nivel3submarino.h \
    npc.h \
    objetojuego.h \
    osciladorarmonico.h \
    pantalladerrota.h \
    pantallainicio.h \
    pantallavictoria.h \
    sistemaaccion.h \
    sistemaaprendizaje.h \
    sistemapercepcion.h \
    sistemarazonamiento.h \
    submarino.h \
    tipos.h \
    torpedo.h \
    trayectoriaparabolica.h \
    vector2d.h \
    vortice.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
