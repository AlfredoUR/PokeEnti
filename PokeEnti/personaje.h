
#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <chrono>
#include <thread>
#include <vector>
#include <windows.h>
#include <numeric>
#include <cstdlib>
#include <ctime>
#include <limits>

class Personaje {
public:
    Personaje(int posX, int posY, char simbolo, int pokemonDanyo);
    int ObtenerPosX() const;
    int ObtenerPosY() const;
    char ObtenerSimbolo() const;
    void Movimiento(int dx, int dy);
    void Posicion(int x, int y);
    void ActualizarSimbolo(int x, int y);
    int ObtenerPokeballsRecogidas() const;
    void PokeballsRecogidas();
    int ObtenerPokemonsCapturados() const;
    void AumentarCuentaPokemons();
    void Reiniciar() { pokeballs = 0; pokemonsCapturados = 0; }
    int ObtenerPokemonDanyo() const;
private:
    int posX;
    int posY;
    char simboloPersonaje;
    int pokeballs = 0;
    int pokemonsCapturados = 0;
    int pokemonDanyo;
};
