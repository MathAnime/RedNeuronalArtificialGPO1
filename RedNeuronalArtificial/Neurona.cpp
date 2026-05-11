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

double Neurona::obtenerSalida(std::vector<double> entradas)
{
	ultimasEntradas = entradas;		//Se almacena para el Backpropagation
	double z = bias;
	for (size_t i = 0; i < pesos.size(); i++)
	{
		z += pesos[i] * entradas[i];
	}

	switch (funcionTransferencia)
	{
	case Activacion::ESCALONADA:
		ultimaSalida = (z < 0) ? 0.0 : 1.0;
		break;
	case Activacion::IDENTIDAD:
		ultimaSalida = z;
		break;
	case Activacion::RELU:
		ultimaSalida = (z < 0) ? 0.0 : z;
		break;
	case Activacion::SIGMOIDE:
		ultimaSalida = 1.0 / (1.0 + exp(-z));
		break;
	case Activacion::TANH:
		ultimaSalida = tanh(z);
		break;
	default:
		ultimaSalida = 0.0;
		break;

	}
	return ultimaSalida;
}

std::vector<double> Neurona::obtenerPesosNeurona() const
{
	return pesos;
}

double Neurona::obtenerBiasNeurona() const
{
	return bias;
}

// Calcula el error si esta neurona está en la última capa
void Neurona::calcularDeltaSalida(double valorEsperado) {
	double error = valorEsperado - ultimaSalida; // MSE derivado
	delta = error * calcularDerivada(funcionTransferencia, ultimaSalida);
}

// Calcula el error si esta neurona está en una capa oculta
void Neurona::calcularDeltaOculta(double sumaDeltasSiguienteCapa) {
	delta = sumaDeltasSiguienteCapa * calcularDerivada(funcionTransferencia, ultimaSalida);
}

// Aplica el Descenso del Gradiente
void Neurona::actualizarPesos(double tasaAprendizaje) {
	for (size_t i = 0; i < pesos.size(); ++i) {
		pesos[i] += tasaAprendizaje * delta * ultimasEntradas[i];
	}
	bias += tasaAprendizaje * delta; // La entrada del bias siempre es 1
}
