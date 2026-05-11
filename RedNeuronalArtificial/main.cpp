/*
* PROYECTO DE UNA RED NEURONAL ARTIFICIAL
* 8VO SEMESTRE GPO 1
*/

#include <iostream>
#include <limits>
#include "RedNeuronal.h"

using namespace std;

void menuInferencia(RedNeuronal& rn) {
    int opcion;
    while (true) {
        cout << "\n--- SUBMENU: INFERENCIA DE LA RED ---\n"
            << "1. Entrenar la red neuronal\n"
            << "2. Medir la eficacia de la red neuronal\n"
            << "3. Regresar al menu principal\n"
            << "Seleccione una opcion: ";

        if (!(cin >> opcion)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }

        string carpeta;
        switch (opcion) {
        case 1: {
            cout << "Nombre de la carpeta de entrenamiento: ";
            cin >> carpeta;
            double lr;
            int epocas;
            cout << "Ingrese la tasa de aprendizaje (ej. 0.01): ";
            cin >> lr;
            cout << "Ingrese la cantidad de epocas: ";
            cin >> epocas;
            rn.entrenar(carpeta, lr, epocas);
            break;
        }
        case 2:
            cout << "Nombre de la carpeta de pruebas (Test): ";
            cin >> carpeta;
            rn.medirEficiencia(carpeta);
            break;
        case 3:
            return; // Regresa al main
        default:
            cout << "Opcion invalida.\n";
        }
    }
}


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
            menuInferencia(rn);
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
