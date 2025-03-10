#include "Cidade.h"
#include <cmath>

//Construtor da cidade
Cidade::Cidade(int id, int posX, int posY, int demand)
	: id(id), posX(posX), posY(posY), demand(demand)
{
}

//Retorna o vertice da cidade
int Cidade::getVertice() const {
	return id;
}

//Retorna a posição X da cidade
double Cidade::getPosX() const {
	return posX;
}

//Retorna a posição Y da cidade
double Cidade::getPosY() const {
	return posY;
}

//Retorna a demanda da cidade
int Cidade::getDemand() const {
	return demand;
}

//Calcula distancias entre as cidades
int CalculaDistancia(const Cidade& c1, const Cidade& c2) {
	double distX = c1.getPosX() - c2.getPosX();
	double distY = c1.getPosY() - c2.getPosY();
	return int(std::round(std::sqrt(distX * distX + distY * distY)));
}
