#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>   // para abs()
#include "listas.hpp"
using namespace std;

// ======================================================================
//                       REGISTRO LEÍDO DEL ARCHIVO
// ======================================================================
struct Registro {
    string accion;
    int plazo;
    string bolsa;
    float precio;
    int cantidad;
};

// ======================================================================
//                           NIVEL 3 – OPERACIÓN
// ======================================================================
struct Oper {
    string tipo;     // Cpra o Vta
    string accion;
    int cantidad;    // absoluta
};

// ======================================================================
//                           NIVEL 2 – BOLSA
// ======================================================================
struct Bolsa {
    string nombre;
    double monto = 0;      // total absoluto
    double resultado = 0;  // total con signo
    Nodo<Oper>* ops = nullptr;
};

int critBolsa(Bolsa a, Bolsa b) {
    return a.nombre.compare(b.nombre);
}

// ======================================================================
//                           NIVEL 1 – PLAZO
// ======================================================================
struct Plazo {
    int numero;         // 0,1,2,3
    int compras = 0;
    int ventas = 0;
    Nodo<Bolsa>* bolsas = nullptr;
};

int critPlazo(Plazo a, Plazo b) {
    return a.numero - b.numero;
}

// ======================================================================
//                   PARSEAR UNA LÍNEA DEL ARCHIVO
// ======================================================================
Registro parsearLinea(const string& linea) {
    string accion, token, bolsa;
    int plazo, cantidad;
    float precio;

    stringstream ss(linea);
    ss >> accion;
    ss >> token;

    // Acción con espacios
    while (!(isdigit(token[0]) || token[0]=='-')) {
        accion += " " + token;
        ss >> token;
    }

    plazo = stoi(token);
    bolsa = "";

    while (ss >> token) {
        bool esPrecio = token.find('.') != string::npos;

        if (esPrecio) {
            precio = stof(token);
            ss >> cantidad;
            break;
        } else {
            if (!bolsa.empty()) bolsa += " ";
            bolsa += token;
        }
    }

    return Registro{accion, plazo, bolsa, precio, cantidad};
}

// ======================================================================
//                           IMPRIMIR LISTADO FINAL
// ======================================================================
void mostrarListado(Nodo<Plazo>* lp) {

    while (lp != nullptr) {

        // Nombre del plazo
        string nombrePlazo;
        switch(lp->dato.numero) {
            case 0: nombrePlazo = "CI"; break;
            case 1: nombrePlazo = "24Hs"; break;
            case 2: nombrePlazo = "48Hs"; break;
            case 3: nombrePlazo = "72Hs"; break;
        }

        cout << endl;
        cout << "Plazo: " << nombrePlazo
             << ", Compras: " << lp->dato.compras
             << ", Ventas: " << lp->dato.ventas << endl << endl;

        // NIVEL 2 —bolsas
        Nodo<Bolsa>* pb = lp->dato.bolsas;
        while (pb != nullptr) {
            cout << "Bolsa           Monto        Resultado" << endl;
            cout << pb->dato.nombre << "   "
                 << pb->dato.monto << "   "
                 << pb->dato.resultado << endl << endl;

            cout << "Oper  Acción      Cant." << endl;
            cout << "-----------------------" << endl;

            // NIVEL 3 —operaciones
            Nodo<Oper>* po = pb->dato.ops;
            while (po != nullptr) {
                cout << po->dato.tipo << "   "
                     << po->dato.accion << "   "
                     << po->dato.cantidad << endl;
                po = po->sig;
            }

            cout << endl;
            pb = pb->sig;
        }

        lp = lp->sig;
    }
}

//MAIN
int main() {

    ifstream fs("salida.txt");
    if (!fs.is_open()) {
        cout << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    string linea;

    // Saltar encabezado
    getline(fs, linea);
    getline(fs, linea);

    Nodo<Plazo>* listaPlazos = nullptr;

    cout << "=== REGISTROS LEÍDOS ===" << endl << endl;

    // =====================================================
    //          PUNTO 1 + PUNTO 2 COMPLETOS
    // =====================================================
    while (getline(fs, linea)) {
        if (linea.empty()) continue;

        Registro reg = parsearLinea(linea);

        // Mostrar registro (Punto 1)
        cout << reg.accion << " | Plazo: " << reg.plazo
             << " | Bolsa: " << reg.bolsa
             << " | Precio: " << reg.precio
             << " | Cant: " << reg.cantidad << endl;

        //ARMADO DE ESTRUCTURAS

        // NIVEL 1— PLAZO
        Plazo p;
        p.numero = reg.plazo;
        Nodo<Plazo>* nodoPlazo = insertar_unico(p, listaPlazos, critPlazo);

        if (reg.cantidad > 0) nodoPlazo->dato.ventas++;
        else nodoPlazo->dato.compras++;

        // NIVEL 2— BOLSA
        Bolsa b;
        b.nombre = reg.bolsa;
        Nodo<Bolsa>* nodoBolsa = insertar_unico(b, nodoPlazo->dato.bolsas, critBolsa);

        double absMonto = reg.precio * abs(reg.cantidad);
        double signedMonto = reg.precio * reg.cantidad;

        nodoBolsa->dato.monto += absMonto;
        nodoBolsa->dato.resultado += signedMonto;

        // NIVEL 3— OPERACIÓN
        Oper op;
        op.tipo = (reg.cantidad > 0 ? "Vta" : "Cpra");
        op.accion = reg.accion;
        op.cantidad = abs(reg.cantidad);

        agregar(nodoBolsa->dato.ops, op); // mantiene el orden del archivo
    }

    fs.close();

    // =====================================================
    //                   EMITIR LISTADO FINAL
    // =====================================================
    cout << endl << "=== LISTADO FINAL (3 NIVELES) ===" << endl;
    mostrarListado(listaPlazos);

    return 0;
}
