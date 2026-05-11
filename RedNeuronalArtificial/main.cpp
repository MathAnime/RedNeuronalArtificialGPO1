/*
* PROYECTO DE UNA RED NEURONAL ARTIFICIAL
* 8VO SEMESTRE GPO 1
*/

#include <iostream>
#include <limits>
#include "RedNeuronal.h"

using namespace std;

int main() {
    RedNeuronal rn;
    string configFile;

    cout << "=== SISTEMA DE RED NEURONAL (OOP MODULAR) ===\n";
    cout << "Archivo de configuracion inicial: ";
    cin >> configFile;

    if (!rn.construirRedNeuronal(configFile)) {
        cout << "[ERROR] Configuracion invalida o archivo no encontrado.\n";
        return 1;
    }

    int opcion;
    while (true) {
        cout << "\n=== MENU PRINCIPAL ===\n"
            << "1. Estimacion del valor de salida\n"
            << "2. Cargar pesos y bias\n"
            << "3. Guardar estructura de la red neuronal\n"
            << "4. Salir\n"
            << "Opcion: ";

        if (!(cin >> opcion)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        string filename;
        switch (opcion) {
        case 1:
            cout << "Archivo de entrada de datos: ";
            cin >> filename;
            rn.obtenerSalidaRedNeuronal(filename);
            break;
        case 2:
            cout << "Archivo de pesos y bias: ";
            cin >> filename;
            rn.cargarPesosDesdeArchivo(filename);
            break;
        case 3:
            cout << "Archivo para guardar la red: ";
            cin >> filename;
            rn.guardarEstructuraRedNeuronal(filename);
            break;
        case 4:
            cout << "Saliendo...\n";
            return 0;
        default:
            cout << "[ADVERTENCIA] Opcion invalida.\n";
        }
    }
}
