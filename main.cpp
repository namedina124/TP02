#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>   // abs()
#include "listas.hpp"

using namespace std;

// =======================================================
// REGISTRO DEL ARCHIVO BINARIO
// =======================================================
struct Registro {
    char accion[12];
    int plazo;
    char bolsa[14];
    float precio;
    int cantidad;
};

// =======================================================
// NIVEL 3 – OPERACIÓN
// =======================================================
struct Operacion {
    char tipo[5];     // "Cpra" o "Vta"
    char accion[12];
    int cantidad;     // absoluta
};

// =======================================================
// NIVEL 2 – BOLSA
// =======================================================
struct Bolsa {
    char nombre[14];
    double monto;
    double resultado;
    Nodo<Operacion>* operaciones;
};

int critBolsa(Bolsa a, Bolsa b) {
    return strcmp(a.nombre, b.nombre);
}

// =======================================================
// NIVEL 1 – PLAZO
// =======================================================
struct Plazo {
    int codigo;
    int compras;
    int ventas;
    Nodo<Bolsa>* bolsas;
};

int critPlazo(Plazo a, Plazo b) {
    return a.codigo - b.codigo;
}

// =======================================================
// MOSTRAR LISTADO
// =======================================================
void mostrarListado(Nodo<Plazo>* lp) {
    while (lp) {

        const char* nombrePlazo;
        switch (lp->dato.codigo) {
            case 0: nombrePlazo = "CI"; break;
            case 1: nombrePlazo = "24Hs"; break;
            case 2: nombrePlazo = "48Hs"; break;
            case 3: nombrePlazo = "72Hs"; break;
        }

        cout << "\nPlazo: " << nombrePlazo
             << ", Compras: " << lp->dato.compras
             << ", Ventas: " << lp->dato.ventas << "\n\n";

        Nodo<Bolsa>* pb = lp->dato.bolsas;
        while (pb) {

            cout << "Bolsa           Monto      Resultado\n";
            cout << pb->dato.nombre << "   "
                 << pb->dato.monto << "   "
                 << pb->dato.resultado << "\n\n";

            cout << "Oper  Acción      Cant.\n";
            cout << "-----------------------\n";

            Nodo<Operacion>* po = pb->dato.operaciones;
            while (po) {
                cout << po->dato.tipo << "   "
                     << po->dato.accion << "   "
                     << po->dato.cantidad << "\n";
                po = po->sig;
            }

            cout << "\n";
            pb = pb->sig;
        }

        lp = lp->sig;
    }
}

// =======================================================
// MAIN
// =======================================================
int main() {

    ifstream arch("Datos.bin", ios::binary);
    if (!arch) {
        cout << "No se pudo abrir Datos.bin\n";
        return 1;
    }

    Nodo<Plazo>* listaPlazos = nullptr;
    Registro r;

    cout << "=== REGISTROS LEÍDOS ===\n\n";

    while (arch.read((char*)&r, sizeof(Registro))) {

        // PUNTO 1 – Mostrar registro
        cout << r.accion << " | Plazo: " << r.plazo
             << " | Bolsa: " << r.bolsa
             << " | Precio: " << r.precio
             << " | Cant: " << r.cantidad << endl;

        // NIVEL 1 – PLAZO
        Plazo p = { r.plazo, 0, 0, nullptr };
        Nodo<Plazo>* nodoPlazo = insertar_unico(p, listaPlazos, critPlazo);

        if (r.cantidad > 0) nodoPlazo->dato.ventas++;
        else nodoPlazo->dato.compras++;

        // NIVEL 2 – BOLSA
        Bolsa b;
        strcpy(b.nombre, r.bolsa);
        b.monto = 0;
        b.resultado = 0;
        b.operaciones = nullptr;

        Nodo<Bolsa>* nodoBolsa =
            insertar_unico(b, nodoPlazo->dato.bolsas, critBolsa);

        double absMonto = r.precio * abs(r.cantidad);
        nodoBolsa->dato.monto += absMonto;
        nodoBolsa->dato.resultado += r.precio * r.cantidad;

        // NIVEL 3 – OPERACIÓN
        Operacion op;
        strcpy(op.accion, r.accion);
        op.cantidad = abs(r.cantidad);
        strcpy(op.tipo, r.cantidad > 0 ? "Vta" : "Cpra");

        agregar(nodoBolsa->dato.operaciones, op); // mismo orden del archivo
    }

    arch.close();

    cout << "\n=== LISTADO FINAL ===\n";
    mostrarListado(listaPlazos);

    return 0;
}

