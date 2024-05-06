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
void displayMap(Map& map, Player& player);
void movePlayer(Player& player, int dx, int dy, Map& map);
void capturePokemon(Player& player, Map& map);
void unlockZones(Player& player, Map& map, const Configuration& config);
void gameLoop(Map& map, Player& player, const Configuration& config);
void freeMap(Map& map);

// Definiciones de funciones

Configuration loadConfiguration(const char* filename) {
    ifstream config(filename);
    Configuration conf;

    if (config.is_open()) {
        char delimiter;
        config >> conf.mapWidth >> delimiter >> conf.mapHeight;

        int pokemonInitial, unlockRequirement;
        while (config >> pokemonInitial >> delimiter >> unlockRequirement) {
            conf.pokemonCounts.push_back({ pokemonInitial, unlockRequirement });
        }
    }

    return conf;
}

// Función para crear el mapa y definir paredes
Map createMap(int width, int height) {
    Map map;
    map.width = width;
    map.height = height;

    map.data = new char* [height];
    for (int i = 0; i < height; i++) {
        map.data[i] = new char[width];
        for (int j = 0; j < width; j++) {
            map.data[i][j] = ' '; // Inicializar con espacios vacíos
        }
    }

    // Agregar paredes exteriores
    for (int i = 0; i < width; i++) {
        map.data[0][i] = 'X'; // Pared superior
        map.data[height - 1][i] = 'X'; // Pared inferior
    }

    for (int i = 0; i < height; i++) {
        map.data[i][0] = 'X'; // Pared izquierda
        map.data[i][width - 1] = 'X'; // Pared derecha
    }

    // Agregar paredes internas para dividir zonas
    int zoneWidth = width / 2;
    int zoneHeight = height / 2;

    for (int i = zoneWidth - 1; i < width; i += zoneWidth) {
        for (int j = 0; j < height; j++) {
            map.data[j][i] = 'X'; // Pared vertical para dividir zonas
        }
    }

    for (int i = zoneHeight - 1; i < height; i += zoneHeight) {
        for (int j = 0; j < width; j++) {
            map.data[i][j] = 'X'; // Pared horizontal para dividir zonas
        }
    }

    return map;
}


void placePokemons(Map& map, const Configuration& config) {
    std::srand(std::time(nullptr)); // Semilla para generación aleatoria

    for (size_t i = 0; i < config.pokemonCounts.size(); i++) {
        int numPokemons = config.pokemonCounts[i].first;

        for (int j = 0; j < numPokemons; j++) {
            int x, y;
            bool validPosition = false;

            while (!validPosition) {
                x = std::rand() % map.width; // Generar posición x
                y = std::rand() % map.height; // Generar posición y

                if (map.data[y][x] == ' ') { // Asegurarse de que la celda esté vacía
                    map.data[y][x] = 'P'; // Colocar Pokémon
                    validPosition = true;
                }
            }
        }
    }
}



void displayMap(Map& map, Player& player) {
    system("cls"); // Limpiar la consola para redibujar el mapa
    for (int i = 0; i < map.height; i++) {
        for (int j = 0; j < map.width; j++) {
            if (i == player.y && j == player.x) { // Mostrar al jugador
                cout << player.direction;
            }
            else {
                cout << map.data[i][j]; // Mostrar el resto del mapa
            }
        }
        cout << endl;
    }
}


void movePlayer(Player& player, int dx, int dy, Map& map) {
    int newX = player.x + dx;
    int newY = player.y + dy;

    // Verificar que el movimiento esté dentro de los límites y no atraviese paredes
    if (newX >= 0 && newX < map.width && newY >= 0 && newY < map.height && map.data[newY][newX] != 'X') {
        player.x = newX; // Asignar la nueva posición
        player.y = newY;
    }
}



void capturePokemon(Player& player, Map& map) {
    int dx = 0;
    int dy = 0;

    switch (player.direction) {
    case '^': dy = -1; break;
    case 'v': dy = 1; break;
    case '>': dx = 1; break;
    case '<': dx = -1; break;
    }

    int newX = player.x + dx;
    int newY = player.y + dy;

    if (newX >= 0 && newX < map.width && newY >= 0 && newY < map.height && map.data[newY][newX] == 'P') {
        map.data[newY][newX] = ' ';
        player.pokemonCaptured++;
    }
}


void unlockZones(Player& player, Map& map, const Configuration& config) {
    int zoneWidth = map.width / 2;
    int zoneHeight = map.height / 2;
    int totalPokemonRequired = 0;

    for (int i = 0; i < config.pokemonCounts.size(); i++) {
        totalPokemonRequired += config.pokemonCounts[i].second;

        if (player.pokemonCaptured >= totalPokemonRequired) {
            int zoneStartX = (i % 2) * zoneWidth;
            int zoneStartY = (i / 2) * zoneHeight;

            if (i % 2 == 0) {
                for (int y = zoneStartY; y < zoneStartY + zoneHeight; y++) {
                    map.data[y][zoneWidth - 1] = ' ';
                }
            }

            if (i / 2 == 0) {
                for (int x = 0; x < zoneWidth; x++) {
                    map.data[zoneHeight - 1][x] = ' ';
                }
            }
        }
    }
}

void gameLoop(Map& map, Player& player, const Configuration& config) {
    bool running = true;

    while (running) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            running = false;
            continue; // Salir del bucle si se presiona ESC
        }

        bool moved = false;

        if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
            player.direction = '<';
            movePlayer(player, -1, 0, map);
            moved = true;
        }
        else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
            player.direction = '>';
            movePlayer(player, 1, 0, map);
            moved = true;
        }
        else if (GetAsyncKeyState(VK_UP) & 0x8000) {
            player.direction = '^';
            movePlayer(player, 0, -1, map);
            moved = true;
        }
        else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
            player.direction = 'v'; // ERROR: Corregir a "="
            movePlayer(player, 0, 1, map);
            moved = true; // ERROR: Corregir a "="
        }

        if (moved) {
            displayMap(map, player); // Redibujar el mapa
            capturePokemon(player, map); // Verificar si se capturan Pokémon
            unlockZones(player, map, config); // Desbloquear zonas si es necesario
        }

        Sleep(100); // Controlar la velocidad del bucle
    }
}




void freeMap(Map& map) {
    for (int i = 0; i < map.height; i++) {
        delete[] map.data[i];
    }
    delete[] map.data;
}

// Función principal

int main() {
    Configuration config = loadConfiguration("config.txt");

    Map map = createMap(config.mapWidth, config.mapHeight);

    // Colocar paredes para definir zonas
    int zoneWidth = map.width / 2;
    int zoneHeight = map.height / 2;

    for (int i = 0; i < map.width; i++) {
        map.data[0][i] = 'X';
        map.data[map.height - 1][i] = 'X';
    }

    for (int i = 0; i < map.height; i++) {
        map.data[i][0] = 'X';
        map.data[i][map.width - 1] = 'X';
    }



    placePokemons(map, config);

    // Iniciar al jugador
    Player ash = { 0, 0, 'v', 0 }; // Iniciar en la posición (0, 0)

    // Iniciar el bucle del juego
    gameLoop(map, ash, config);

    // Liberar recursos al final
    freeMap(map);

    return 0;
}
