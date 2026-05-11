#pragma once
#include <string>
#include <algorithm>

enum class Activacion
{
	ESCALONADA,
	SIGMOIDE,
	TANH,
	RELU,
	IDENTIDAD,
	DESCONOCIDA
};

/*El siguiente metodo transforma una cadena de texto
al valor correspondiente de la clase enum (Activacion)*/
inline Activacion parseActivacion(const std::string& funcStr)
{
	std::string f = funcStr;
	transform(f.begin(), f.end(), f.begin(), ::tolower);

	if (f == "escalonada")
	{
		return Activacion::ESCALONADA;
	}

	if (f == "sigmoide")
	{
		return Activacion::SIGMOIDE;
	}

	if (f == "tangente_hiperbolica")
	{
		return Activacion::TANH;
	}

	if (f == "relu")
	{
		return Activacion::RELU;
	}

	if (f == "identidad")
	{
		return Activacion::IDENTIDAD;
	}

	return Activacion::DESCONOCIDA;

}

/*Metodo que transforma un tipo de dato Activacion a una cadena
de texto (Se usará cuando se desea almacenar la configuracion de la red
neuronal artificial)*/
inline std::string activacionToString(Activacion act)
{
	switch (act)
	{
	case Activacion::ESCALONADA:
		return "escalonada";
	case Activacion::SIGMOIDE:
		return "sigmoide";
	case Activacion::TANH:
		return "tangente_hiperbolica";
	case Activacion::RELU:
		return "relu";
	case Activacion::IDENTIDAD:
		return "identidad";
	default:
		return "desconocida";
	}
}