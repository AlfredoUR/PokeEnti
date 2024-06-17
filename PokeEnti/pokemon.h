
#pragma once

#include "liderGimnasio.h"


class Pokemons {
public:
	Pokemons(int posX, int posY, char simbolo, int vida);
	int ObtenerPosX() const;
	int ObtenerPosY() const;
	char ObtenerSimbolo() const;
	int ObtenerVida() const;
	void DisminucionVida(int cantidad);

private:
	int posX;
	int posY;
	char simbolo;
	int vida;
};