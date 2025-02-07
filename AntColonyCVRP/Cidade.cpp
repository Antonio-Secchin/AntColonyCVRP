#include "Cidade.h"
#include <cmath>



Cidade::Cidade(int vertice, int posX, int posY, int demand)
	: vertice(vertice), posX(posX), posY(posY), demand(demand)
{
}

int Cidade::getVertice() const {
	return vertice;
}

int Cidade::getPosX() const {
	return posX;
}

int Cidade::getPosY() const {
	return posY;
}

int Cidade::getDemand() const {
	return demand;
}

float CalculaDistancia(const Cidade& c1, const Cidade& c2) {
	int distX = c1.getPosX() - c2.getPosX();
	int distY = c1.getPosY() - c2.getPosY();
	return std::sqrt(distX * distX + distY * distY);
}
