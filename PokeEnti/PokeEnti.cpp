#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>

#include <iostream>
#include <fstream>
#include <windows.h>
using namespace std;


struct Configuration {
    int mapWidth;
    int mapHeight;
    std::vector<std::pair<int, int>> pokemonCounts; // Parejas de Pokémon y requisitos para desbloqueo
};

Configuration loadConfiguration(const char* filename) {
    ifstream config(filename);
    Configuration conf;

    if (config.is_open()) {
        // Leer dimensiones del mapa
        char delimiter;
        config >> conf.mapWidth >> delimiter >> conf.mapHeight;

        // Leer el número de Pokémon en cada zona y requisitos para desbloqueo
        int pokemonInitial, unlockRequirement;
        while (config >> pokemonInitial >> delimiter >> unlockRequirement) {
            conf.pokemonCounts.push_back({pokemonInitial, unlockRequirement});
        }
    }

    return conf;
}


struct Pokemon {
    int x;
    int y;
};

void placePokemons(Map& map, const Configuration& config) {
    // Usar una semilla para la generación aleatoria
    std::srand(std::time(nullptr));

    for (int i = 0; i < config.pokemonCounts.size(); i++) {
        int pokemonCount = config.pokemonCounts[i].first;

        // Añadir Pokémon aleatoriamente en el mapa
        for (int j = 0; j < pokemonCount; j++) {
            int x, y;
            do {
                x = std::rand() % map.width;
                y = std::rand() % map.height;
            } while (map.data[y][x] != ' '); // Verificar que no se ponga en una pared

            map.data[y][x] = 'P';
        }
    }
}

struct Player {
    int x;
    int y;
    char direction; // '^', 'v', '>', '<'
    int pokemonCaptured;
};

void capturePokemon(Player& player, Map& map) {
    int dx = 0;
    int dy = 0;

    // Determinar la dirección del jugador para ver qué celda chequear
    switch (player.direction) {
        case '^': dy = -1; break;
        case 'v': dy = 1; break;
        case '>': dx = 1; break;
        case '<': dx = -1; break;
    }

    int newX = player.x + dx;
    int newY = player.y + dy;

    if (newX >= 0 && newX < map.width && newY >= 0 && newY < map.height) {
        if (map.data[newY][newX] == 'P') {
            // Capturar el Pokémon
            map.data[newY][newX] = ' ';
            player.pokemonCaptured++;
        }
    }
}

void unlockZones(Player& player, Map& map, const Configuration& config) {
    int totalPokemonRequired = 0;
    int zoneWidth = map.width / 2;
    int zoneHeight = map.height / 2;

    // Sumar los requisitos para cada zona hasta la actual para desbloquearlas
    for (int i = 0; i < config.pokemonCounts.size(); i++) {
        totalPokemonRequired += config.pokemonCounts[i].second;
        if (player.pokemonCaptured >= totalPokemonRequired) {
            // Desbloquear la zona
            int zoneStartX = (i % 2) * zoneWidth;
            int zoneStartY = (i / 2) * zoneHeight;

            // Remover las paredes del mapa que separan zonas
            if (i % 2 == 0) {
                // Desbloquea a la derecha
                for (int y = zoneStartY; y < zoneStartY + zoneHeight; y++) {
                    map.data[y][zoneStartX + zoneWidth - 1] = ' ';
                }
            }

            if (i / 2 == 0) {
                // Desbloquea hacia abajo
                for (int x = zoneStartX; x < zoneStartX + zoneWidth; x++) {
                    map.data[zoneStartY + zoneHeight - 1][x] = ' ';
                }
            }
        }
    }
}

void gameLoop(Map& map, Player& player, const Configuration& config) {
    bool running = true;
    while (running) {
        if (GetAsyncKeyState(VK_ESCAPE)) {
            running = false;
        } else if (GetAsyncKeyState(VK_LEFT)) {
            player.direction = '<';
            movePlayer(player, -1, 0, map);
        } else if (GetAsyncKeyState(VK_RIGHT)) {
            player.direction = '>';
            movePlayer(player, 1, 0, map);
        } else if (GetAsyncKeyState(VK_UP)) {
            player.direction = '^';
            movePlayer(player, 0, -1, map);
        } else if (GetAsyncKeyState(VK_DOWN)) {
            player.direction = 'v';
            movePlayer(player, 0, 1, map);
        } else if (GetAsyncKeyState(VK_SPACE)) {
            capturePokemon(player, map);
            unlockZones(player, map, config);
        }

        displayMap(map, player);
        Sleep(100); // Controlar la velocidad de actualización
    }
}




// Definiciones del mapa y jugador
struct Map {
    char** data;
    int width;
    int height;
};

struct Configuration {
    int mapWidth;
    int mapHeight;
    std::vector<std::pair<int, int>> pokemonCounts; // Parejas de Pokémon y requisitos para desbloqueo
};

struct Player {
    int x;
    int y;
    char direction; // '^', 'v', '>', '<'
    int pokemonCaptured;
};

// Leer el archivo de configuración
Configuration loadConfiguration(const char* filename) {
    ifstream config(filename);
    Configuration conf;

    if (config.is_open()) {
        char delimiter;
        config >> conf.mapWidth >> delimiter >> conf.mapHeight;

        int pokemonInitial, unlockRequirement;
        while (config >> pokemonInitial >> delimiter >> unlockRequirement) {
            conf.pokemonCounts.push_back({pokemonInitial, unlockRequirement});
        }
    }

    return conf;
}

// Crear la matriz del mapa
Map createMap(int width, int height) {
    Map map;
    map.width = width;
    map.height = height;

    map.data = new char*[height];
    for (int i = 0; i < height; i++) {
        map.data[i] = new char[width];
        for (int j = 0; j < width; j++) {
            map.data[i][j] = ' '; // Inicializar el mapa con espacio vacío
        }
    }

    return map;
}

// Función para mostrar la vista
void displayMap(Map& map, Player& player) {
    system("cls");
    for (int i = 0; i < map.height; i++) {
        for (int j = 0; j < map.width; j++) {
            if (i == player.y && j == player.x) {
                cout << player.direction;
            } else {
                cout << map.data[i][j];
            }
        }
        cout << endl;
    }
}

// Función para mover al jugador
void movePlayer(Player& player, int dx, int dy, Map& map) {
    int newX = player.x + dx;
    int newY = player.y + dy;

    if (newX >= 0 && newX < map.width && newY >= 0 && newY < map.height && map.data[newY][newX] != 'X') {
        player.x = newX;
        player.y = newY;
    }
}

// Función para capturar Pokémon
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

    if (newX >= 0 && newX < map.width && newY >= 0 && newY < map.height) {
        if (map.data[newY][newX] == 'P') {
            map.data[newY][newX] = ' ';
            player.pokemonCaptured++;
        }
    }
}

// Desbloqueo de zonas
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
                for (int x = zoneStartX; x < zoneStartX + zoneWidth; x++) {
                    map.data[zoneStartY + zoneHeight - 1][x] = ' ';
                }
            }
        }
    }
}

// Controlar el juego
void gameLoop(Map& map, Player& player, const Configuration& config) {
    bool running = true;
    while (running) {
        if (GetAsyncKeyState(VK_ESCAPE)) {
            running = false;
        } else if (GetAsyncKeyState(VK_LEFT)) {
            player.direction = '<';
            movePlayer(player, -1, 0, map);
        } else if (GetAsyncKeyState(VK_RIGHT)) {
            player.direction = '>';
            movePlayer(player, 1, 0, map);
        } else if (GetAsyncKeyState(VK_UP)) {
            player.direction = '^';
            movePlayer(player, 0, -1, map);
        } else if (GetAsyncKeyState(VK_DOWN)) {
            player.direction = 'v';
            movePlayer(player, 0, 1, map);
        } else if (GetAsyncKeyState(VK_SPACE)) {
            capturePokemon(player, map);
            unlockZones(player, map, config);
        }

        displayMap(map, player);
        Sleep(100); // Controlar la velocidad de actualización
    }
}

// Liberar memoria
void freeMap(Map& map) {
    for (int i = 0; i < map.height; i++) {
        delete[] map.data[i];
    }
    delete[] map.data;
}

// Programa principal
int main() {
    Configuration config = loadConfiguration("config.txt");

    Map map = createMap(config.mapWidth, config.mapHeight);

    // Colocar paredes para definir las zonas
    for (int i = 0; i < map.width; i++) {
        map.data[0][i] = 'X'; // Pared superior
        map.data[map.height - 1][i] = 'X'; // Pared inferior
    }
    for (int i = 0; i < map.height; i++) {
        map.data[i][0] = 'X'; // Pared izquierda
        map.data[i][map.width - 1] = 'X'; // Pared derecha
    }

    // Agregar paredes interiores para zonas
    int zoneWidth = map.width / 2;
    int zoneHeight = map.height / 2;
    for (int i = zoneWidth - 1; i < map.width; i += zoneWidth) {
        for (int j = 0; j < map.height; j++) {
            map.data[j][i] = 'X';
        }
    }
    for (int j = zoneHeight - 1; j < map.height; j += zoneHeight) {
        for (int i = 0; i < map.width; i++) {
            map.data[j][i] = 'X';
        }
    }

    // Colocar Pokémon en el mapa según la configuración
    placePokemons(map, config);

    // Iniciar al jugador
    Player ash = {0, 0, 'v', 0};

    // Iniciar el bucle del juego
    gameLoop(map, ash, config);

    // Liberar recursos
    freeMap(map);

    return 0;
}

