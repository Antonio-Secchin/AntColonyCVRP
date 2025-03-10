#pragma once
class Cidade
{
public:
	int id;
	double posX;
	double posY;
	int demand;

	Cidade(int id = 0, int posX = 0, int posY = 0, int demand = 0);

	int getVertice() const;

	double getPosX() const;

	double getPosY() const;

	int getDemand() const;
};

int CalculaDistancia(const Cidade& c1, const Cidade& c2);

