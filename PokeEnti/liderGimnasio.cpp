
#include "liderGimnasio.h"

LiderGimnasio::LiderGimnasio(int posX, int posY, char simbolo, const std::string& nombre, int vida) : posX(posX), posY(posY), simbolo(simbolo), nombre(nombre), vida(vida) {}

int LiderGimnasio::ObtenerPosX() const {
    return posX;
}

int LiderGimnasio::ObtenerPosY() const {
    return posY;
}

char LiderGimnasio::ObtenerSimbolo() const {
    return simbolo;
}

std::string LiderGimnasio::ObtenerNombre() const {
    return nombre;
}

int LiderGimnasio::ObtenerVida() const {
    return vida;
}

void LiderGimnasio::DisminucionVida(int amount) {
    vida -= amount;
    if (vida < 0) {
        vida = 0;
    }
}
