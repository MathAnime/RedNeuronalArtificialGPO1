#pragma once
#include <vector>
#include "Enums.h"

class Neurona
{
private:
	std::vector<double> pesos;
	double bias;
	Activacion funcionTransferencia;

public:
	Neurona(int numEntradas, Activacion func);
	void inicializarAleatoriamenteNeurona();
	void establecerPesosNeurona(const std::vector<double> nuevos_pesos, double nuevo_bias);
	double obtenerSalida(const std::vector<double> entradas);
	std::vector<double> obtenerPesosNeurona() const;
	double obtenerBiasNeurona() const;
};