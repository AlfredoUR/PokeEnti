
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

void Personaje::Movimiento(int x, int y) {
    posX += x;
    posY += y;
}

void Personaje::Posicion(int x, int y) {
    posX = x;
    posY = y;
}

void Personaje::ActualizarSimbolo(int x, int y) {
    if (x < posX) {
        simboloPersonaje = '^';
    }
    else if (x > posX) {
        simboloPersonaje = 'v';
    }
    else if (y < posY) {
        simboloPersonaje = '<';
    }
    else if (y > posY) {
        simboloPersonaje = '>';
    }

    posX = x;
    posY = y;
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