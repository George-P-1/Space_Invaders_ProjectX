TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        enemy.cpp \
        main.cpp \
        player.cpp

# ----------CHANGE THE PATH TO SFML LIBRARY ACCORDINGLY-----------------------

INCLUDEPATH += "C:/Users/Ashish/Documents/SFML-2.5.1/include"

LIBS += -L"C:/Users/Ashish/Documents//SFML-2.5.1/lib"
CONFIG(debug, debug|release){
    LIBS += -lsfml-audio-d -lsfml-graphics-d -lsfml-network-d -lsfml-system-d -lsfml-window-d
} else {
    LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window
}

HEADERS += \
    enemy.h \
    player.h