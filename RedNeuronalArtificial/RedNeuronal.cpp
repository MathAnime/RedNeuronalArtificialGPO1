#include "RedNeuronal.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<std::string> RedNeuronal::split(const std::string& s, char delimiter) const {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        if (!token.empty() && token.find_first_not_of(' ') != std::string::npos)
            tokens.push_back(token);
    }
    return tokens;
}

bool RedNeuronal::construirRedNeuronal(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line1, line2, line3, line4;
    std::getline(file, line1); // Num capas
    std::getline(file, line2); // Arquitectura
    std::getline(file, line3); // Funciones
    std::getline(file, line4); // Pesos

    int numCapas = std::stoi(line1);
    if (numCapas < 2) return false;

    std::stringstream ss2(line2);
    int n;
    arquitectura.clear();
    while (ss2 >> n) arquitectura.push_back(n);

    std::stringstream ss3(line3);
    std::string funcStr;
    funcionesConfiguradas.clear();
    while (ss3 >> funcStr) funcionesConfiguradas.push_back(parseActivacion(funcStr));

    capas.clear();
    for (int i = 1; i < numCapas; ++i) {
        int entradasAnteriores = arquitectura[i - 1];
        capas.emplace_back(arquitectura[i], entradasAnteriores, funcionesConfiguradas[i]);
    }

    if (line4.empty()) {
        for (auto& c : capas) c.inicializarAleatoriamente();
    }
    else {
        cargarPesosDesdeCadena(line4);
    }
    return true;
}

void RedNeuronal::cargarPesosDesdeCadena(const std::string& lineaPesos) {
    std::vector<std::string> capasStr = split(lineaPesos, ';');
    for (size_t i = 0; i < std::min(capas.size(), capasStr.size()); ++i) {
        std::stringstream ssW(capasStr[i]);
        double w;
        std::vector<double> valoresPlano;
        while (ssW >> w) valoresPlano.push_back(w);
        capas[i].establecerPesosYBias(valoresPlano);
    }
}

void RedNeuronal::cargarPesosDesdeCadena(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        std::getline(file, line);
        cargarPesosDesdeCadena(line);
        std::cout << "[INFO] Pesos cargados.\n";
    }
}

void RedNeuronal::obtenerSalidaRedNeuronal(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::vector<double> datosActuales;
    double val;
    while (file >> val) datosActuales.push_back(val);

    for (const auto& capa : capas) {
        datosActuales = capa.salidaCapa(datosActuales);
    }

    std::cout << "\n--- SALIDA ESTIMADA ---\n";
    for (size_t i = 0; i < datosActuales.size(); ++i) {
        std::cout << "Y" << i + 1 << ": " << datosActuales[i] << "\n";
    }
}

void RedNeuronal::guardarEstructuraRedNeuronal(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    file << arquitectura.size() << "\n";
    for (int n : arquitectura) file << n << " ";
    file << "\n";
    for (Activacion act : funcionesConfiguradas) file << activacionToString(act) << " ";
    file << "\n";

    // Guardado separado por ";"
    for (size_t i = 0; i < capas.size(); ++i) {
        file << capas[i].exportarPesosYBias();
        if (i < capas.size() - 1) file << "; ";
    }
    file << "\n";
    std::cout << "[INFO] Red guardada en " << filename << "\n";
}