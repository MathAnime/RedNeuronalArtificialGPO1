#pragma once
#include <vector>
#include "Enums.h"

class Neurona
{
private:
	std::vector<double> pesos;
	double bias;
	Activacion funcionTransferencia;

	// NUEVO: Variables de estado para Backpropagation
	std::vector<double> ultimasEntradas;
	double ultimaSalida;
	double delta; // Error propagado


public:
	Neurona(int numEntradas, Activacion func);
	void inicializarAleatoriamenteNeurona();
	void establecerPesosNeurona(const std::vector<double> nuevos_pesos, double nuevo_bias);
	double obtenerSalida(std::vector<double> entradas);
	std::vector<double> obtenerPesosNeurona() const;
	double obtenerBiasNeurona() const;

	// Modificado para guardar el estado
	double calcularSalida(const std::vector<double>& entradas);

	// NUEVO: Métodos de entrenamiento
	double getDelta() const { return delta; }
	void calcularDeltaSalida(double valorEsperado);
	void calcularDeltaOculta(double sumaDeltasSiguienteCapa);
	void actualizarPesos(double tasaAprendizaje);

};