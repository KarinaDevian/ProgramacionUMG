#include <iostream>
#include <limits>
using namespace std;

// Estructura del nodo
struct Nodo {
    int dato;
    Nodo* siguiente;
};

// Función: mostrar toda la lista
void mostrarLista(Nodo* cabeza) {
    cout << "\nContenido de la lista:\n\n";

    Nodo* actual = cabeza;

    while (actual != nullptr) {

        if (actual->siguiente != nullptr)
            cout << "[" << actual->dato << " | * ] --> ";
        else
            cout << "[" << actual->dato << " | NULL ]";

        actual = actual->siguiente;
    }

    cout << "\n";
}

// Función: liberar memoria
void liberarLista(Nodo* cabeza) {
    Nodo* actual = cabeza;

    while (actual != nullptr) {
        Nodo* siguiente = actual->siguiente;
        delete actual;
        actual = siguiente;
    }

    cout << "Memoria liberada correctamente.\n";
}

int main() {

    Nodo* cabeza = nullptr;
    Nodo* ultimo = nullptr;

    char respuesta;

    do {

        cout << "\nDesea ingresar un nuevo valor [S/N]: ";
        cin >> respuesta;

        respuesta = toupper(respuesta);

        if (respuesta == 'S') {

            Nodo* nuevo = new Nodo();

            // VALIDACIÓN
            while (true) {

                cout << "Ingrese el valor: ";
                cin >> nuevo->dato;

                if (cin.fail()) {

                    cout << "Error: Debe ingresar un numero entero.\n";

                    cin.clear(); // limpia error
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                }
                else {
                    break;
                }
            }

            nuevo->siguiente = nullptr;

            if (cabeza == nullptr) {
                cabeza = nuevo;
                ultimo = nuevo;
            }
            else {
                ultimo->siguiente = nuevo;
                ultimo = nuevo;
            }
        }

    } while (respuesta == 'S');

    if (cabeza != nullptr)
        mostrarLista(cabeza);
    else
        cout << "\nLa lista esta vacia.\n";

    liberarLista(cabeza);

    return 0;
}