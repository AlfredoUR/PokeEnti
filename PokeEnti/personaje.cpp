// personaje.cpp

#include "personaje.h"

Personaje::Personaje(int posX, int posY, char simbolo, int pokemonDanyo) : posX(posX), posY(posY), simboloPersonaje(simbolo), pokemonDanyo(pokemonDanyo) {}

int Personaje::ObtenerPosX() const {
    return posX;
}

int Personaje::ObtenerPosY() const {
    return posY;
}

char Personaje::ObtenerSimbolo() const {
    return simboloPersonaje;
}

void Personaje::Movimiento(int dx, int dy) {
    posX += dx;
    posY += dy;
}

void Personaje::Posicion(int newX, int newY) {
    posX = newX;
    posY = newY;
}

void Personaje::ActualizarSimbolo(int newX, int newY) {
    if (newX < posX) {
        simboloPersonaje = '^';
    }
    else if (newX > posX) {
        simboloPersonaje = 'v';
    }
    else if (newY < posY) {
        simboloPersonaje = '<';
    }
    else if (newY > posY) {
        simboloPersonaje = '>';
    }

    posX = newX;
    posY = newY;
}

int Personaje::ObtenerPokeballsRecogidas() const {
    return pokeballs;
}

void Personaje::PokeballsRecogidas() {
    pokeballs++;
}

int Personaje::ObtenerPokemonsCapturados() const {
    return pokemonsCapturados;
}

void Personaje::AumentarCuentaPokemons() {
    pokemonsCapturados++;
    pokeballs--;
}

int Personaje::ObtenerPokemonDanyo() const {
    return pokemonDanyo;
}