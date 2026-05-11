#include "Capa.h"
#include <sstream>

//CONSTRUCTOR
Capa::Capa(int numNeuronas, int cantidadEntradasNeurona, Activacion func) :
	activacionCapa(func)
{
	for (int i = 0; i < numNeuronas; i++)
	{
		neuronas.emplace_back(cantidadEntradasNeurona, func);
	}
}
/*
* Metodo que inicializa los pesos aleatoriamente de todas
las neuronas de la capa
*/
void Capa::inicializarAleatoriamente()
{
	for (auto& n : neuronas)
	{
		n.inicializarAleatoriamenteNeurona();
	}
}
/*
* Metodo que inicializa los pesos de manera manual de todas las neuronas
* de la capa
*/
void Capa::establecerPesosYBias(const std::vector<double>& valores)
{
	int numNeuronas = neuronas.size();
	
	if (numNeuronas == 0)
		return;

	int numPesosPorNeurona = neuronas[0].obtenerPesosNeurona().size();
	int idx = 0;

	std::vector<double> biases(numNeuronas);
	for (int i = 0; i < numNeuronas; ++i)
	{
		if (idx < valores.size())
		{
			biases[i] = valores[idx++];
		}
	}
	for (int i = 0; i < numNeuronas; ++i)
	{
		std::vector<double> pesosTemp(numPesosPorNeurona);
		for (int j = 0; j < numPesosPorNeurona; ++j)
		{
			if (idx < valores.size())
			{
				pesosTemp[j] = valores[idx++];
			}
		}
		neuronas[i].establecerPesosNeurona(pesosTemp, biases[i]);
	}
}
/*
* Metodo que permitira almacenar todos los pesos de las neuronas de la capa
*/
std::string Capa::exportarPesosYBias() const
{
	std::stringstream ss;
	for (const auto& n : neuronas)
	{
		ss << n.obtenerBiasNeurona() << " ";
	}

	for (const auto& n : neuronas)
	{
		for (double w : n.obtenerPesosNeurona())
			ss << w << " ";
	}
	return ss.str();
}

/*
* Metodo que permite obtener el valor de salida de la capa
*/
std::vector<double> Capa::salidaCapa(std::vector<double>& entradas)
{
	std::vector<double> salidas;
	for (auto& n : neuronas)
	{
		salidas.push_back(n.obtenerSalida(entradas));
	}
	return salidas;
}


std::vector<Neurona>& Capa::getNeuronas()
{
	return neuronas;
}
void Capa::calcularDeltasCapaSalida(const std::vector<double>& valoresEsperados)
{
	for (size_t i = 0; i < neuronas.size(); ++i)
	{
		neuronas[i].calcularDeltaSalida(valoresEsperados[i]);
	}
}
void Capa::calcularDeltasCapaOculta(Capa& capaSiguiente)
{
	std::vector<Neurona>& neuronasSig = capaSiguiente.getNeuronas();
	for (size_t i = 0; i < neuronas.size(); ++i)
	{
		double sumaDeltas = 0.0;
		// Multiplicar el delta de la capa siguiente por los pesos que conectan con esta neurona
		for (size_t j = 0; j < neuronasSig.size(); ++j)
		{
			sumaDeltas += neuronasSig[j].obtenerPesosNeurona()[i] * neuronasSig[j].getDelta();
		}
		neuronas[i].calcularDeltaOculta(sumaDeltas);
	}

}
void Capa::actualizarPesosCapa(double tasaAprendizaje)
{
	for (auto& n : neuronas)
	{
		n.actualizarPesos(tasaAprendizaje);
	}

}