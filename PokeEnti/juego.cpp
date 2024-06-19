// juego.cpp

#include "juego.h"

int Juego::contadorPokeballs = 0;
int salaActual = PUEBLO_PALETA;
int cantidadPokemons;

Juego::Juego() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    ArchivoConfiguracion("config.txt");

    LiderGimnasio* jefeFinal1 = new LiderGimnasio(10, 10, 'r', "Jefe Final 1", 200);
    LiderGimnasio* jefeFinal2 = new LiderGimnasio(15, 15, 'R', "Jefe Final 2", 200);
    zonas[3]->AnyadirLiderGimnasio(jefeFinal1);
    zonas[3]->AnyadirLiderGimnasio(jefeFinal2);
}

Juego::~Juego() {
    delete personaje;
    for (int contador = 0; contador < numZonas; ++contador) {
        delete zonas[contador];
    }
    delete[] zonas;
}

void Juego::ArchivoConfiguracion(const std::string& nombreArch) {
    std::ifstream archivo(nombreArch);
    std::string linea;
    int contador = 0;

    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            std::istringstream obtenerNum(linea);
            int num1, num2;

            switch (contador) {
            case 0: 
                if (obtenerNum >> num1 && obtenerNum.ignore() && obtenerNum >> num2) {
                    anchuraMapa = num1;
                    alturaMapa = num2;
                }
                break;
            case 1: 
                if (obtenerNum >> num1 && obtenerNum.ignore() && obtenerNum >> num2) {
                    puebloPaletaPokemon = num1;
                    puebloPaletaDesbloquear = num2;
                }
                break;
            case 2: 
                if (obtenerNum >> num1 && obtenerNum.ignore() && obtenerNum >> num2) {
                    bosquePokemon = num1;
                    bosqueDesbloquear = num2;
                }
                break;
            case 3:
                if (obtenerNum >> num1) {
                    pokemonDanyo = num1;
                }
                break;
            case 4:
                if (obtenerNum >> num1 && obtenerNum.ignore() && obtenerNum >> num2) {
                    pokemonVida = num1;
                    mewtwoVida = num2;
                }
                break;
            case 5: 
                if (obtenerNum >> num1 && obtenerNum.ignore() && obtenerNum >> num2) {
                    minTiempoEspera = num1;
                    maxTiempoEspera = num2;
                }
                break;
            default:
                break;
            }
            contador++;
        }
        archivo.close();
    }
    else {
        std::cout << "No se puedo abrir el archivo." << std::endl;
    }

    zonas = new Zonas * [numZonas];
    zonas[0] = new Zonas(anchuraMapa, alturaMapa, 0, puebloPaletaPokemon, pokemonVida);
    zonas[1] = new Zonas(anchuraMapa, alturaMapa, 1, bosquePokemon, pokemonVida);
    zonas[2] = new Zonas(anchuraMapa, alturaMapa, 2, 0, pokemonVida, true, mewtwoVida); 
    zonas[3] = new Zonas(anchuraMapa, alturaMapa, 3, 0, pokemonVida);

    personaje = new Personaje(1, 7, 'v', pokemonDanyo);
}

void Juego::MuestraPantallaDeBienvenida() const {
    std::system("cls");
    std::cout << "                        ----------------------" << std::endl;
    std::cout << " " << std::endl;
    std::cout << "                                POKEENTI        " << std::endl;
    std::cout << " " << std::endl;
    std::cout << "                        ----------------------" << std::endl;
    std::cout << std::endl << std::endl << "CARGANDO..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(0));
}

void Juego::MuestraMenuPrincipal() const {
    std::system("cls");
    std::cout << "                        ----------------------" << std::endl;
    std::cout << " " << std::endl;
    std::cout << "                                POKEENTI        " << std::endl;
    std::cout << " " << std::endl;
    std::cout << "                        ----------------------" << std::endl;
    std::cout << " " << std::endl;
    std::cout << " " << std::endl;
    std::cout << "PULSE 1 PARA COMENZAR A JUGAR" << std::endl;
    std::cout << " " << std::endl;
    std::cout << "PULSE 2 PARA SALIR DEL JUEGO" << std::endl;
    std::cout << " " << std::endl;
}

std::string Juego::ObtenerNombreZonaActual() const {
    switch (salaActual) {
    case PUEBLO_PALETA: return "PUEBLO PALETA";
    case BOSQUE: return "BOSQUE";
    case CUEVA_CELESTE: return "CUEVA CELESTE";
    case LIGA_POKENTI: return "LIGA POKENTI";
    default: return "DESCONOCIDO";
    }
}

bool Juego::PersonajePerdido() const {
    return personaje->ObtenerPokeballsRecogidas() < 0;
}

void Juego::Comienzo() {
    EstadoJuego estado = PANTALLA_DE_BIENVENIDA;
    int eleccion;

    while (estado != FINAL_DEL_JUEGO) {
        switch (estado) {
        case PANTALLA_DE_BIENVENIDA:
            MuestraPantallaDeBienvenida();
            estado = MENU_PRINCIPAL;
            break;
        case MENU_PRINCIPAL:
            MuestraMenuPrincipal();
            std::cin >> eleccion;
            if (std::cin.fail()) {
                std::cin.clear(); 
                borrarInputDeEntrada(); 
                std::cout << "OPCION INVALIDA" << std::endl;
                continue;
            }
            if (eleccion == 1) {
                personaje->Reiniciar();
                contadorPokeballs = 0;
                estado = JUGANDO;
            }
            else if (eleccion == 2) {
                std::cout << "GRACIAS POR PROVAR EL JUEGO!" << std::endl;
                estado = FINAL_DEL_JUEGO;
            }
            else {
                std::cout << "OPCION INVALIDA" << std::endl;
            }
            break;
        case JUGANDO:
            JugarAlJuego(estado);
            if (personaje->ObtenerPokeballsRecogidas() < 0) {
                estado = FINAL_DEL_JUEGO;
                std::cout << "NO QUEDAN POKEBALLS EN EL INVENTARIO" << std::endl;
            }
            break;
        case FINAL_DEL_JUEGO:
            break;
        }

    }
}

void Juego::ManejoInput(bool& corriendo, EstadoJuego& estado) {
    int newPosX = personaje->ObtenerPosX();
    int newPosY = personaje->ObtenerPosY();

    if (GetAsyncKeyState(VK_ESCAPE)) {
        estado = MENU_PRINCIPAL;
        corriendo = false;
    }
    if (GetAsyncKeyState(VK_UP)) {
        newPosX -= 1;
    }
    if (GetAsyncKeyState(VK_DOWN)) {
        newPosX += 1;
    }
    if (GetAsyncKeyState(VK_LEFT)) {
        newPosY -= 1;
    }
    if (GetAsyncKeyState(VK_RIGHT)) {
        newPosY += 1;
    }

    if (zonas[salaActual]->DentroLimites(newPosX, newPosY)) {
        personaje->ActualizarSimbolo(newPosX, newPosY);
        personaje->Movimiento(newPosX - personaje->ObtenerPosX(), newPosY - personaje->ObtenerPosY());

        if (zonas[salaActual]->EstaEnLaPokeball(newPosX, newPosY)) {
            zonas[salaActual]->RecogerPokeball(newPosX, newPosY);
            contadorPokeballs++;
            personaje->PokeballsRecogidas();
            zonas[salaActual]->ActualizarAnyadirPokemon();
        }
        if (zonas[salaActual]->EstaEnLaPuertaArriba(newPosX, newPosY)) {
            CambiarSala(-1);
        }
        else if (zonas[salaActual]->EstaEnLaPuertaAbajo(newPosX, newPosY)) {
            CambiarSala(1);
        }
    }
    if (GetAsyncKeyState('E')) {
        bool batallaIniciada = false;
        for (LiderGimnasio* liderGimnasio : zonas[salaActual]->ObtenerLideresGimnasio()) {
            if (liderGimnasio && std::abs(personaje->ObtenerPosX() - liderGimnasio->ObtenerPosX()) <= 1 && std::abs(personaje->ObtenerPosY() - liderGimnasio->ObtenerPosY()) <= 1) {
                MostrarMenuDeBatallaLideresGimnasio(*liderGimnasio, corriendo, estado);
                batallaIniciada = true;
                break;
            }
        }

        if (!batallaIniciada) {
            for (int contador = 0; contador < zonas[salaActual]->ObtenerContadorPokemons(); ++contador) {
                Pokemons* pokemon = zonas[salaActual]->ObtenerPokemons()[contador];
                if (pokemon && std::abs(personaje->ObtenerPosX() - pokemon->ObtenerPosY()) <= 1 && std::abs(personaje->ObtenerPosY() - pokemon->ObtenerPosY()) <= 1) {
                    MuestraMenuDeBatalla(*pokemon, corriendo, estado);
                    batallaIniciada = true;
                    break;
                }
            }

            if (!batallaIniciada) {
                Pokemons* mewtwo = zonas[salaActual]->ObtenerMewtwo();
                if (mewtwo != nullptr) {
                    if (std::abs(personaje->ObtenerPosX() - mewtwo->ObtenerPosX()) <= 1 && std::abs(personaje->ObtenerPosY() - mewtwo->ObtenerPosY()) <= 1) {
                        MuestraMenuDeBatalla(*mewtwo, corriendo, estado);
                    }
                }
            }
        }
    }
}

void Juego::JugarAlJuego(EstadoJuego& estado) {
    bool corrinedo = true;
    while (corrinedo) {
        std::system("cls");
        std::cout << "USTED TIENE " << personaje->ObtenerPokeballsRecogidas() << " POKEBALLS" << std::endl;
        std::cout << "NUMERO DE POKEMONS CAPTURADOS: " << personaje->ObtenerPokemonsCapturados() << std::endl;
        std::cout << "USTED SE ENCUENTRA EN: " << ObtenerNombreZonaActual() << std::endl << std::endl;

        zonas[salaActual]->Muestra(personaje->ObtenerPosX(), personaje->ObtenerPosY(), personaje->ObtenerSimbolo());

        ManejoInput(corrinedo, estado);

        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

void Juego::CambiarSala(int direccion) {
    int proximaSala = salaActual + direccion;
    if (proximaSala >= 0 && proximaSala < numZonas) {
        salaActual = proximaSala;
        if (direccion == -1) {
            personaje->Posicion(zonas[salaActual]->ObtenerAltura() - 2, zonas[salaActual]->ObtenerAnchura() / 2);
        }
        else {
            personaje->Posicion(1, zonas[salaActual]->ObtenerAnchura() / 2);
        }
    }
}

void Juego::MostrarMenuDeBatallaLideresGimnasio(LiderGimnasio& liderGimnasio, bool& corriendo, EstadoJuego& estado) {
    bool enBatalla = true;

    while (enBatalla && corriendo) {
        std::system("cls");
        std::cout << "NUMERO TOTAL DE POKEMONS CAPTURADOS: " << personaje->ObtenerPokemonsCapturados() << "   NUMERO DE POKEBALLS OBTENIDAS: " << personaje->ObtenerPokeballsRecogidas() << std::endl;
        std::cout << "USTED SE ENCUENTRA EN: " << ObtenerNombreZonaActual() << std::endl;
        std::cout << "-----------------------------------------------------------------------------" << std::endl;

        zonas[salaActual]->Muestra(personaje->ObtenerPosX(), personaje->ObtenerPosY(), personaje->ObtenerSimbolo());

        std::cout << "-----------------------------------------------------------------------------" << std::endl;
        std::cout << "¡SE HA ENCONTRADO CON EL LIDER DEL GIMNASIO " << liderGimnasio.ObtenerNombre() << "!" << std::endl;
        std::cout << "LIDER: " << liderGimnasio.ObtenerSimbolo() << "  VIDA: " << liderGimnasio.ObtenerVida() << std::endl;
        std::cout << "-----------------------------------------------------------------------------" << std::endl;
        std::cout << "[A] PARA ATACAR,  [H] PARA HUIR" << std::endl;

        if (GetAsyncKeyState('A')) {
            liderGimnasio.DisminucionVida(personaje->ObtenerPokemonDanyo());
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            if (liderGimnasio.ObtenerVida() <= 0) {
                std::cout << "¡HA DERROTADO A " << liderGimnasio.ObtenerNombre() << "!" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                enBatalla = false;

                if (LideresDeGimnasioVencidos()) {
                    std::cout << "USTED HA VENCIDO A LOS LIDERES DE GIMNASIO Y HA COMPLETADO ESTA AVNETURA" << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                    estado = MENU_PRINCIPAL;
                    corriendo = false;
                }
            }
        }
        else if (GetAsyncKeyState('H')) {
            std::cout << "USTED HA HUIDO DEL COMBATE" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            enBatalla = false;
        }
    }
}

void Juego::MuestraMenuDeBatalla(Pokemons& pokemon, bool& corriendo, EstadoJuego& estado) {
    bool enBatalla = true;
    bool esMewtwo = (pokemon.ObtenerSimbolo() == 'M');

    while (enBatalla && corriendo) {
        std::system("cls");
        std::cout << "NUMERO DE POKEMONS CAPTURADOS: " << personaje->ObtenerPokemonsCapturados() << "   NUMERO DE POKEBALLS OBTENIDAS: " << personaje->ObtenerPokeballsRecogidas() << std::endl;
        std::cout << "USTED SE ENCUENTRA EN: " << ObtenerNombreZonaActual() << std::endl;
        std::cout << "-----------------------------------------------------------------------------" << std::endl;

        zonas[salaActual]->Muestra(personaje->ObtenerPosX(), personaje->ObtenerPosY(), personaje->ObtenerSimbolo());

        std::cout << "-----------------------------------------------------------------------------" << std::endl;
        std::cout << "¡SE HA ENCONTRADO CON UN POKEMON SALVAJE!" << std::endl;
        std::cout << "POKEMON: " << pokemon.ObtenerSimbolo() << "  VIDA: " << pokemon.ObtenerVida() << std::endl;
        std::cout << "1. ATACAR" << std::endl;
        std::cout << "2. CAPTURAR" << std::endl;
        std::cout << "3. HUIR" << std::endl;

        int eleccion;
        borrarInputDeEntrada();
        std::cin >> eleccion;

        if (std::cin.fail()) {
            std::cout << "Entrada no valida. Intentalo de nuevo." << std::endl;
            borrarInputDeEntrada();
            continue;
        }

        switch (eleccion) {
        case 1: 
            pokemon.DisminucionVida(personaje->ObtenerPokemonDanyo());
            std::cout << "HA ATACADO AL POKEMON RIVAL. SALUD RESTANTE: " << pokemon.ObtenerVida() << std::endl;
            if (pokemon.ObtenerVida() == 0) {
                if (esMewtwo) {
                    zonas[salaActual]->RetirarMewtwo();
                    std::cout << "¡MEWTWO HA SIDO VENCIDO!" << std::endl;
                }
                else {
                    zonas[salaActual]->RetirarPokemon(pokemon.ObtenerPosX(), pokemon.ObtenerPosY());
                    std::cout << "EL POKEMON RIVAL HA SIDO VENCIDO!" << std::endl;
                }
                enBatalla = false;
            }
            break;
        case 2: 
            if (personaje->ObtenerPokeballsRecogidas() > 0) {
                float probabilidadDeCaptura = CalcularPosibilidadDeCaptura(pokemon.ObtenerVida(), 100);
                if (esMewtwo) {
                    probabilidadDeCaptura /= 2; 
                }
                float valorAleatorio = static_cast<float>(std::rand()) / RAND_MAX;
                if (valorAleatorio < probabilidadDeCaptura) {
                    personaje->AumentarCuentaPokemons();
                    if (esMewtwo) {
                        zonas[salaActual]->RetirarMewtwo(); 
                        std::cout << "HA CAPTURADO A MEWTWO!" << std::endl;
                    }
                    else {
                        zonas[salaActual]->RetirarPokemon(pokemon.ObtenerPosX(), pokemon.ObtenerPosY());
                        std::cout << "HA CAPTURADO AL POKEMON SALVAJE!" << std::endl;
                    }
                    enBatalla = false;
                }
                else {
                    std::cout << "EL POKEMON SALVAJE ESCAPÓ DE LA POKEBALL!" << std::endl;
                }
            }
            else {
                std::cout << "NO LE QUEDAN POKEBALLS" << std::endl;
            }
            break;
        case 3:
            std::cout << "HUISTE DEL COMBATE" << std::endl;
            if (esMewtwo) {
                zonas[salaActual]->RetirarMewtwo(); 
                std::cout << "MEWTWO HA DESAPARECIDO" << std::endl;
            }
            enBatalla = false;
            break;
        default:
            std::cout << "Opcion invalida." << std::endl;
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

bool Juego::LideresDeGimnasioVencidos() const {
    for (LiderGimnasio* liderGimnasio : zonas[3]->ObtenerLideresGimnasio()) {
        if (liderGimnasio->ObtenerVida() > 0) {
            return false;
        }
    }
    return true;
}

float Juego::CalcularPosibilidadDeCaptura(int vidaActual, int vidaMax) const {
    return 1.0f - static_cast<float>(vidaActual) / vidaMax;
}

void borrarInputDeEntrada() {
    std::cin.clear();
    std::cin.ignore(10000, '\n');
}
