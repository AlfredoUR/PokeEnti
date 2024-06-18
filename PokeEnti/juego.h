
#pragma once

#include "personaje.h"
#include "zonas.h"
#include "liderGimnasio.h"
#include <vector>

enum EstadoJuego {
    PANTALLA_DE_BIENVENIDA,
    MENU_PRINCIPAL,
    JUGANDO,
    FINAL_DEL_JUEGO,
    INVALID
};

enum ZoneType {
    PUEBLO_PALETA,
    BOSQUE,
    CUEVA_CELESTE,
    LIGA_POKENTI,
    UNKNOWN
};

class Juego {
public:
    Juego();
    ~Juego();
    void Comienzo();
    static int contadorPokeballs;
    int numZonas = 4;
    std::string ObtenerNombreZonaActual() const;

private:
    Personaje* personaje;
    Zonas** zonas;
    void MuestraPantallaDeBienvenida() const;
    void MuestraMenuPrincipal() const;
    bool PersonajePerdido() const;
    void JugarAlJuego(EstadoJuego& estado);
    void ManejoInput(bool& corriendo, EstadoJuego& estado);
    void CambiarSala(int direccion);
    void MostrarMenuDeBatallaLideresGimnasio(LiderGimnasio& liderGimnasio, bool& corriendo, EstadoJuego& estado);
    void MuestraMenuDeBatalla(Pokemons& pokemon, bool& corriendo, EstadoJuego& estado);
    bool LideresDeGimnasioVencidos() const;
    float CalcularPosibilidadDeCaptura(int vidaActual, int vidaMax) const;
    void ArchivoConfiguracion(const std::string& nombreArch);
    int anchuraMapa, alturaMapa;
    int puebloPaletaPokemon, puebloPaletaDesbloquear;
    int bosquePokemon, bosqueDesbloquear;
    int pokemonDanyo;
    int pokemonVida, mewtwoVida;
    int minTiempoEspera, maxTiempoEspera;
};

void borrarInputDeEntrada();
