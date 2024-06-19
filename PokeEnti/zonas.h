// zonas.h

#pragma once

#include "pokemon.h"

class Zonas {
public:
    Zonas(int anchura, int altura, int numeroSala, int cantidadPokemons, int vidaPokemons, bool zonaMewtwo = false, int vidaMewtwo = 0);
    ~Zonas();
    void Muestra(int personajePosX, int personajePosY, char personajeSimbolo) const;
    bool DentroLimites(int x, int y) const;
    bool EstaEnLaPuertaArriba(int x, int y) const;
    bool EstaEnLaPuertaAbajo(int x, int y) const;
    bool EstaEnLaPokeball(int x, int y) const;
    void RecogerPokeball(int x, int y);
    int ObtenerContadorPokeballs() const;
    int ObtenerAnchura() const;
    int ObtenerAltura() const;
    Pokemons** ObtenerPokemons() const;
    int ObtenerContadorPokemons() const;
    void RetirarPokemon(int x, int y);
    void ActualizarAnyadirPokemon();
    Pokemons* ObtenerMewtwo();
    void RetirarMewtwo();
    void AnyadirLiderGimnasio(LiderGimnasio* liderGimnasio);
    std::vector<LiderGimnasio*> ObtenerLideresGimnasio() const;

private:
    char** cuadricula;
    int anchura;
    int altura;
    int numeroSala;
    Pokemons** pokemons;
    int cantidadPokemons;
    bool mewtwoPresencia;
    Pokemons* mewtwo;
    std::vector<LiderGimnasio*> lideresGimnasio;
};
