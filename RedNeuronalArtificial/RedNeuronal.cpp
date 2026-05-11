#include "RedNeuronal.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <filesystem>

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

void RedNeuronal::cargarPesosDesdeArchivo(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        std::getline(file, line);
        cargarPesosDesdeCadena(line);
        std::cout << "[INFO] Pesos cargados.\n";
    }
}

void RedNeuronal::obtenerSalidaRedNeuronal(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::vector<double> datosActuales;
    double val;
    while (file >> val)
        datosActuales.push_back(val);

    for (auto& capa : capas)
    {
        datosActuales = capa.salidaCapa(datosActuales);
    }

    std::cout << "\n--- SALIDA ESTIMADA ---\n";
    for (size_t i = 0; i < datosActuales.size(); ++i)
    {
        std::cout << "Y" << i + 1 << ": " << datosActuales[i] << "\n";
    }
}

void RedNeuronal::guardarEstructuraRedNeuronal(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) return;

    file << arquitectura.size() << "\n";
    for (int n : arquitectura)
        file << n << " ";
    file << "\n";
    for (Activacion act : funcionesConfiguradas)
        file << activacionToString(act) << " ";
    file << "\n";

    // Guardado separado por ";"
    for (size_t i = 0; i < capas.size(); ++i)
    {
        file << capas[i].exportarPesosYBias();
        if (i < capas.size() - 1) file << "; ";
    }
    file << "\n";
    std::cout << "[INFO] Red guardada en " << filename << "\n";
}

// Lee un archivo binario plano llenando un vector bidimensional
bool RedNeuronal::cargarDatosBinarios(const std::string& ruta, std::vector<std::vector<double>>& datos, int tamañoVector)
{
    std::ifstream archivo(ruta, std::ios::binary);
    if (!archivo.is_open()) return false;

    datos.clear();
    while (archivo.peek() != EOF)
    {
        std::vector<double> registro(tamañoVector);
        archivo.read(reinterpret_cast<char*>(registro.data()), tamañoVector * sizeof(double));
        if (archivo.gcount() > 0) datos.push_back(registro);
    }
    return true;
}

void RedNeuronal::entrenar(const std::string& carpetaDatos, double tasaAprendizaje, int epocas)
{
    std::vector<std::vector<double>> x_train, y_train;
    std::string rutaX = carpetaDatos + "/train_images.bin";
    std::string rutaY = carpetaDatos + "/train_labels.bin";

    if (!cargarDatosBinarios(rutaX, x_train, arquitectura[0]) ||
        !cargarDatosBinarios(rutaY, y_train, arquitectura.back())) {
        std::cerr << "[ERROR] No se encontraron los archivos binarios en la carpeta especificada.\n";
        return;
    }

    // Buscar numero consecutivo para el archivo de logs
    int numArchivo = 1;
    while (std::filesystem::exists("val_error_epoc_" + std::to_string(numArchivo) + ".txt"))
    {
        numArchivo++;
    }
    std::string nombreLog = "val_error_epoc_" + std::to_string(numArchivo) + ".txt";
    std::ofstream logFile(nombreLog);

    std::cout << "[INFO] Iniciando entrenamiento...\n";
    auto inicio = std::chrono::high_resolution_clock::now();

    for (int epoca = 1; epoca <= epocas; ++epoca)
    {
        double errorCuadraticoMedio = 0.0;

        for (size_t i = 0; i < x_train.size(); ++i)
        {
            // 1. FORWARD PROPAGATION
            std::vector<double> activaciones = x_train[i];
            for (auto& capa : capas) {
                activaciones = capa.salidaCapa(activaciones);
            }

            // Calculo del error de la muestra
            for (size_t j = 0; j < activaciones.size(); ++j)
            {
                double diff = y_train[i][j] - activaciones[j];
                errorCuadraticoMedio += diff * diff;
            }

            // 2. BACKWARD PROPAGATION (Reversa)
            // Capa de Salida
            capas.back().calcularDeltasCapaSalida(y_train[i]);

            // Capas Ocultas
            for (int c = capas.size() - 2; c >= 0; --c)
            {
                capas[c].calcularDeltasCapaOculta(capas[c + 1]);
            }

            // 3. ACTUALIZACIÓN DE PESOS
            for (auto& capa : capas)
            {
                capa.actualizarPesosCapa(tasaAprendizaje);
            }
        }

        errorCuadraticoMedio /= x_train.size();
        logFile << "Epoca " << epoca << " Error: " << errorCuadraticoMedio << "\n";
        std::cout << "Epoca " << epoca << "/" << epocas << " - MSE: " << errorCuadraticoMedio << "\n";
    }

    auto fin = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion = fin - inicio;

    logFile.close();
    std::cout << "\n[EXITO] Red neuronal entrenada con exito.\n";
    std::cout << "Tiempo de entrenamiento: " << duracion.count() << " segundos.\n";
    std::cout << "Log de errores guardado en: " << nombreLog << "\n";
}

void RedNeuronal::medirEficiencia(const std::string& carpetaTest) {
    std::vector<std::vector<double>> x_test, y_test;
    std::string rutaX = carpetaTest + "/test_images.bin";
    std::string rutaY = carpetaTest + "/test_labels.bin";

    if (!cargarDatosBinarios(rutaX, x_test, arquitectura[0]) ||
        !cargarDatosBinarios(rutaY, y_test, arquitectura.back()))
    {
        std::cerr << "[ERROR] No se encontraron los datos de prueba.\n";
        return;
    }

    int aciertos = 0;
    for (size_t i = 0; i < x_test.size(); ++i)
    {
        std::vector<double> activaciones = x_test[i];
        for (auto& capa : capas) {
            activaciones = capa.salidaCapa(activaciones);
        }

        // Obtener el índice con mayor probabilidad (clasificación MNIST)
        int idxPredicho = std::distance(activaciones.begin(), std::max_element(activaciones.begin(), activaciones.end()));
        int idxReal = std::distance(y_test[i].begin(), std::max_element(y_test[i].begin(), y_test[i].end()));

        if (idxPredicho == idxReal)
            aciertos++;
    }

    double porcentaje = (double)aciertos / x_test.size() * 100.0;
    std::cout << "\n[EFICIENCIA] Evaluacion en conjunto de prueba terminada.\n";
    std::cout << "Precision de la red: " << porcentaje << "% (" << aciertos << "/" << x_test.size() << ")\n";
}
