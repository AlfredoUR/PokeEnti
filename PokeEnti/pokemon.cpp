
#include "pokemon.h"

Pokemons::Pokemons(int posX, int posY, char simbolo, int vida) : posX(posX), posY(posY), simbolo(simbolo), vida(vida) {}

int Pokemons::ObtenerPosX() const {
    return posX;
}

int Pokemons::ObtenerPosY() const {
    return posY;
}

char Pokemons::ObtenerSimbolo() const {
    return simbolo;
}

int Pokemons::ObtenerVida() const {
    return vida;
}

void Pokemons::DisminucionVida(int cantidad) {
    vida -= cantidad;
    if (vida < 0) {
        vida = 0;
    }
}