#pragma once
#include "Neurona.h"
#include <string>
#include <vector>

class Capa
{
private:
	std::vector<Neurona> neuronas;
	Activacion activacionCapa;
public:
	Capa(int numNeuronas, int cantidadEntradasNeurona, Activacion func);
	
	void inicializarAleatoriamente();
	void establecerPesosYBias(const std::vector<double>& valores);
	std::string exportarPesosYBias() const;
	std::vector<double> salidaCapa(const std::vector<double>& entradas) const;
};
