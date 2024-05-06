#pragma once

#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;


// Estructuras y Funciones

struct Map {
    char** data;
    int width;
    int height;
};

struct Configuration {
    int mapWidth;
    int mapHeight;
    std::vector<std::pair<int, int>> pokemonCounts;
};

struct Player {
    int x;
    int y;
    char direction;
    int pokemonCaptured;
};


// Declaraciones de funciones
Configuration loadConfiguration(const char* filename);
Map createMap(int width, int height);
void placePokemons(Map& map, const Configuration& config);
void displayMap(Map& map, Player& player);
void movePlayer(Player& player, int dx, int dy, Map& map);
void capturePokemon(Player& player, Map& map);
void unlockZones(Player& player, Map& map, const Configuration& config);
void gameLoop(Map& map, Player& player, const Configuration& config);
void freeMap(Map& map);

