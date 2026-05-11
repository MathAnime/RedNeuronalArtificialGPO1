#include "Neurona.h"
#include <cmath>
#include <random>

//Constructor
Neurona::Neurona(int numEntradas, Activacion func) :
	funcionTransferencia(func), bias(0.0)
{
	pesos.resize(numEntradas, 0.0);
}
/*
* Metodo que asigna valores aleatorios a los pesos de la neurona
*/
void Neurona::inicializarAleatoriamenteNeurona()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.0, 1.0);

	for (double& w : pesos)
	{
		w = dis(gen);
	}

	bias = dis(gen);
}
/*
* Metodo que asigna a partir de valores dados por el usuario los
* pesos de la neurona
*/
void Neurona::establecerPesosNeurona(const std::vector<double> nuevos_pesos, double nuevo_bias)
{
	pesos = nuevos_pesos;
	bias = nuevo_bias;
}

/*
* Obtener el valor de salida de la neurona
*/

double Neurona::obtenerSalida(const std::vector<double> entradas) const
{
	double z = 0.0;
	for (size_t i = 0; i < pesos.size(); i++)
	{
		z += pesos[i] * entradas[i];
	}

	z += bias;

	switch (funcionTransferencia)
	{
	case Activacion::ESCALONADA:
		return (z < 0) ? 0.0 : 1.0;
	case Activacion::IDENTIDAD:
		return z;
	case Activacion::RELU:
		return (z < 0) ? 0.0 : z;
	case Activacion::SIGMOIDE:
		return 1.0 / (1.0 + exp(-z));
	case Activacion::TANH:
		return tanh(z);
	default:
		return 0.0;

	}
}

std::vector<double> Neurona::obtenerPesosNeurona() const
{
	return pesos;
}

double Neurona::obtenerBiasNeurona() const
{
	return bias;
}