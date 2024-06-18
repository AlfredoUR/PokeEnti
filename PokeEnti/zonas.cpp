
#include "zonas.h"

Zonas::Zonas(int anchura, int altura, int numeroSala, int cantidadPokemons, int vidaPokemons, bool zonaMewtwo, int vidaMewtwo) : anchura(anchura), altura(altura), numeroSala(numeroSala), mewtwoPresencia(zonaMewtwo), mewtwo(nullptr), cantidadPokemons(cantidadPokemons) {
    cuadricula = new char* [altura];
    for (int contador = 0; contador < altura; ++contador) {
        cuadricula[contador] = new char[anchura];
        for (int contador2 = 0; contador2 < anchura; ++contador2) {
            if (contador == 0 || contador == altura - 1 || contador2 == 0 || contador == anchura - 1) {
                cuadricula[contador][contador2] = 'X';
            }
            else {
                cuadricula[contador][contador2] = ' ';
            }
        }
    }

    if (numeroSala == 0) {
        cuadricula[altura - 1][anchura / 2] = 'D';
    }
    else if (numeroSala == 1) {
        cuadricula[0][anchura / 2] = 'U';
        cuadricula[altura - 1][anchura / 2] = 'D';
    }
    else if (numeroSala == 2) {
        cuadricula[0][anchura / 2] = 'U';
        cuadricula[altura - 1][anchura / 2] = 'D';
    }
    else if (numeroSala == 3) {
        cuadricula[0][anchura / 2] = 'U';
    }

    if (zonaMewtwo) {
        int abscisas = altura / 2;
        int ordenadas = anchura / 2;
        mewtwo = new Pokemons(abscisas, ordenadas, 'M', vidaMewtwo);
        cuadricula[abscisas][ordenadas] = 'M';
    }
    else {
        pokemons = new Pokemons * [cantidadPokemons];
        for (int i = 0; i < cantidadPokemons; ++i) {
            int pokemonPosX = std::rand() % (altura - 2) + 1;
            int pokemonPosY = std::rand() % (anchura - 2) + 1;
            pokemons[i] = new Pokemons(pokemonPosX, pokemonPosY, 'P', vidaPokemons);
        }
    }
}

Zonas::~Zonas() {
    for (int contador = 0; contador < altura; ++contador) {
        delete[] cuadricula[contador];
    }
    delete[] cuadricula;

    for (int contador = 0; contador < cantidadPokemons; ++contador) {
        delete pokemons[contador];
    }
    delete[] pokemons;

    delete mewtwo;

    for (LiderGimnasio* liderGimnasio : lideresGimnasio) {
        delete liderGimnasio;
    }
}

void Zonas::AnyadirLiderGimnasio(LiderGimnasio* liderGimnasio) {
    lideresGimnasio.push_back(liderGimnasio);
    cuadricula[liderGimnasio->ObtenerPosX()][liderGimnasio->ObtenerPosY()] = liderGimnasio->ObtenerSimbolo();
}

std::vector<LiderGimnasio*> Zonas::ObtenerLideresGimnasio() const {
    return lideresGimnasio;
}

void Zonas::RecogerPokeball(int x, int y) {
    if (cuadricula[x][y] == 'B') {
        cuadricula[x][y] = ' ';
    }
}

Pokemons* Zonas::ObtenerMewtwo() {
    return mewtwo;
}

void Zonas::RetirarMewtwo() {
    if (mewtwo) {
        cuadricula[mewtwo->ObtenerPosX()][mewtwo->ObtenerPosY()] = ' ';
        delete mewtwo;
        mewtwo = nullptr;
    }
}

void Zonas::RetirarPokemon(int x, int y) {
    for (int contador = 0; contador < cantidadPokemons; ++contador) {
        if (pokemons[contador] && pokemons[contador]->ObtenerPosX() == x && pokemons[contador]->ObtenerPosY() == y) {
            delete pokemons[contador];
            pokemons[contador] = nullptr;
            cuadricula[x][y] = ' '; // Eliminar del mapa
            break;
        }
    }
}

void Zonas::Muestra(int personajePosX, int personajePosY, char personajeSimbolo) const {
    const int mostrarAnchura = 35;
    const int mostrarAltura = 35;

    int comienzoX = (personajePosX - mostrarAnchura / 2 >= 0) ? personajePosX - mostrarAnchura / 2 : 0;
    int comienzoY = (personajePosY - mostrarAltura / 2 >= 0) ? personajePosY - mostrarAltura / 2 : 0;
    int finalX = (personajePosX + mostrarAnchura / 2 < altura) ? personajePosX + mostrarAnchura / 2 : altura - 1;
    int finalY = (personajePosY + mostrarAltura / 2 < anchura) ? personajePosY + mostrarAltura / 2 : anchura - 1;

    for (int contador = comienzoX; contador <= finalX; ++contador) {
        for (int contador2 = comienzoY; contador2 <= finalY; ++contador2) {
            bool personaje = (contador == personajePosX && contador2 == personajePosY);
            bool pokemon = false;

            for (int contador3 = 0; contador3 < cantidadPokemons; ++contador3) {
                if (pokemons[contador3] && pokemons[contador3]->ObtenerPosX() == contador && pokemons[contador3]->ObtenerPosY() == contador2) {
                    pokemon = true;
                    break;
                }
            }

            if (personaje) {
                std::cout << personajeSimbolo;
            }
            else if (pokemon) {
                std::cout << 'P'; // Pokemon
            }
            else if (mewtwo && mewtwo->ObtenerPosX() == contador && mewtwo->ObtenerPosY() == contador2) {
                std::cout << 'M'; // Mewtwo
            }
            else {
                std::cout << cuadricula[contador][contador2];
            }
        }
        std::cout << std::endl;
    }
}

bool Zonas::DentroLimites(int x, int y) const {
    return x >= 0 && x < altura && y >= 0 && y < anchura && cuadricula[x][y] != 'X';
}

bool Zonas::EstaEnLaPuertaArriba(int x, int y) const {
    return cuadricula[x][y] == 'U';
}

bool Zonas::EstaEnLaPuertaAbajo(int x, int y) const {
    return cuadricula[x][y] == 'D';
}

bool Zonas::EstaEnLaPokeball(int x, int y) const {
    return cuadricula[x][y] == 'B';
}

int Zonas::ObtenerContadorPokeballs() const {
    int contadorPokeballs = 0;
    for (int contador = 0; contador < altura; ++contador) {
        for (int contador2 = 0; contador2 < anchura; ++contador2) {
            if (cuadricula[contador][contador2] == 'B') {
                contadorPokeballs++;
            }
        }
    }
    return contadorPokeballs;
}

int Zonas::ObtenerAnchura() const {
    return anchura;
}

int Zonas::ObtenerAltura() const {
    return altura;
}

Pokemons** Zonas::ObtenerPokemons() const {
    return pokemons;
}

int Zonas::ObtenerContadorPokemons() const {
    return cantidadPokemons;
}

void Zonas::ActualizarAnyadirPokemon() {
    int pokeballPosX = std::rand() % (altura - 2) + 1;
    int pokeballPosY = std::rand() % (anchura - 2) + 1;
    cuadricula[pokeballPosX][pokeballPosY] = 'B';
}
