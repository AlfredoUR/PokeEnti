
#pragma once

#include "personaje.h"

class LiderGimnasio {
public:
    LiderGimnasio(int posX, int posY, char simbolo, const std::string& nombre, int vida);
    int ObtenerPosX() const;
    int ObtenerPosY() const;
    char ObtenerSimbolo() const;
    std::string ObtenerNombre() const;
    int ObtenerVida() const;
    void DisminucionVida(int amount);

private:
    int posX;
    int posY;
    char simbolo;
    std::string nombre;
    int vida;
};
