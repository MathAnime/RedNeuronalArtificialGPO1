#pragma once
#include "Capa.h"
#include <vector>
#include <string>

class RedNeuronal
{
private:
	std::vector<Capa> capas;	//Vector de capas
	std::vector<int> arquitectura;	//Vector de entero
	std::vector<Activacion> funcionesConfiguradas;

	std::vector<std::string> split(const std::string& s, char delimitador) const;

public:
	bool construirRedNeuronal(const std::string& nombre_Archivo);
	void cargarPesosDesdeCadena(const std::string& lineaPesos);
	void cargarPesosDesdeArchivo(const std::string& nombre_Archivo);
	void obtenerSalidaRedNeuronal(const std:: string& nombre_Archivo);
	void guardarEstructuraRedNeuronal(const std::string& nombre_Archivo);
};
