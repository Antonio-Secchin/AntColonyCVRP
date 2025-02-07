#pragma once
class Cidade
{
	int vertice;
	int posX;
	int posY;
	int demand;
public:

	Cidade(int vertice, int posX, int posY, int demand);

	int getVertice() const;

	int getPosX() const;

	int getPosY() const;

	int getDemand() const;
};

float CalculaDistancia(const Cidade& c1, const Cidade& c2);

