#include <iostream>
#include <limits>
#include <string>
#include <conio.h>
#include <cstdlib>

using namespace std;

// Limpia el buffer del teclado
void LimpiarBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Espera una tecla antes de regresar al menu
void PausarPantalla() {
    cout << "\nPresione una tecla para continuar...";
    _getch();
}

// Clase Cancion
class Cancion {
public:
    string Nombre;
    string Artista;
    string Duracion;
    string Lanzamiento;

    // Solicita los datos de la cancion
    void AgregarDatos() {
        cout << "\nNombre de la cancion: ";
        getline(cin, Nombre);

        cout << "Artista: ";
        getline(cin, Artista);

        cout << "Duracion: ";
        getline(cin, Duracion);

        cout << "Fecha de lanzamiento: ";
        getline(cin, Lanzamiento);
    }

    // Muestra la informacion de la cancion
    void MostrarDatos() const {
        cout << "Nombre: " << Nombre << "\n";
        cout << "Artista: " << Artista << "\n";
        cout << "Duracion: " << Duracion << "\n";
        cout << "Fecha de lanzamiento: " << Lanzamiento << "\n";
    }
};

// Estructura del nodo
struct Nodo {
    Cancion dato;
    Nodo* siguiente;
    Nodo* anterior;
};

// Clase que administra la lista de reproduccion
class ListaReproduccion {
private:
    Nodo* cabeza;
    Nodo* actual;

public:
    ListaReproduccion() {
        cabeza = nullptr;
        actual = nullptr;
    }

    ~ListaReproduccion() {
        if (cabeza == nullptr) {
            return;
        }

        Nodo* recorrido = cabeza->siguiente;

        while (recorrido != cabeza) {
            Nodo* auxiliar = recorrido;
            recorrido = recorrido->siguiente;
            delete auxiliar;
        }

        delete cabeza;
    }

    void AgregarCancion();
    void MostrarLista() const;
    void ReproducirPlaylist();
};

// Agrega una cancion a la lista circular
void ListaReproduccion::AgregarCancion() {
    Nodo* nuevo = new Nodo;

    nuevo->dato.AgregarDatos();

    if (cabeza == nullptr) {
        cabeza = nuevo;
        nuevo->siguiente = nuevo;
        nuevo->anterior = nuevo;
        actual = cabeza;
    }
    else {
        Nodo* ultimo = cabeza->anterior;

        nuevo->siguiente = cabeza;
        nuevo->anterior = ultimo;
        ultimo->siguiente = nuevo;
        cabeza->anterior = nuevo;
    }

    cout << "\nCancion agregada correctamente.\n";
    PausarPantalla();
}

// Muestra toda la lista de canciones
void ListaReproduccion::MostrarLista() const {
    system("cls");

    cout << "========================================\n";
    cout << "          Lista de reproduccion\n";
    cout << "========================================\n";

    if (cabeza == nullptr) {
        cout << "\nLa lista esta vacia.\n";
        PausarPantalla();
        return;
    }

    Nodo* recorrido = cabeza;
    int numero = 1;

    do {
        cout << "\nCancion " << numero << "\n";
        cout << "........................................\n";
        recorrido->dato.MostrarDatos();

        recorrido = recorrido->siguiente;
        numero++;

    } while (recorrido != cabeza);

    PausarPantalla();
}

// Permite navegar entre canciones usando las flechas
void ListaReproduccion::ReproducirPlaylist() {
    if (cabeza == nullptr) {
        system("cls");

        cout << "========================================\n";
        cout << "          Reproducir playlist\n";
        cout << "========================================\n";
        cout << "\nNo hay canciones para reproducir.\n";

        PausarPantalla();
        return;
    }

    // Cada vez que se inicia la reproduccion empieza desde la primera cancion
    actual = cabeza;

    int tecla;

    do {
        system("cls");

        cout << "========================================\n";
        cout << "             Reproduciendo ahora\n";
        cout << "========================================\n\n";

        actual->dato.MostrarDatos();

        cout << "\n........................................\n";
        cout << "Flecha izquierda: Cancion anterior\n";
        cout << "Flecha derecha:   Cancion siguiente\n";
        cout << "ESC:               Volver al menu\n";
        cout << "........................................\n";

        tecla = _getch();

        if (tecla == 0 || tecla == 224) {
            tecla = _getch();

            if (tecla == 77) {
                actual = actual->siguiente;
            }
            else if (tecla == 75) {
                actual = actual->anterior;
            }
        }

    } while (tecla != 27);
}

// Programa principal
int main() {
    ListaReproduccion lista;
    int opcion;

    do {
        system("cls");

        cout << "========================================\n";
        cout << "          Lista de reproduccion\n";
        cout << "========================================\n";
        cout << "\n1. Agregar cancion\n";
        cout << "2. Mostrar lista completa\n";
        cout << "3. Reproducir playlist\n";
        cout << "4. Salir\n";
        cout << "\nSeleccione una opcion: ";

        while (!(cin >> opcion)) {
            cin.clear();
            LimpiarBuffer();

            cout << "\nOpcion invalida. Ingrese un numero: ";
        }

        LimpiarBuffer();

        switch (opcion) {
        case 1:
            system("cls");

            cout << "========================================\n";
            cout << "             Agregar cancion\n";
            cout << "========================================\n";

            lista.AgregarCancion();
            break;

        case 2:
            lista.MostrarLista();
            break;

        case 3:
            lista.ReproducirPlaylist();
            break;

        case 4:
            system("cls");

            cout << "========================================\n";
            cout << "             Programa finalizado\n";
            cout << "========================================\n\n";
            break;

        default:
            cout << "\nOpcion no valida.\n";
            PausarPantalla();
            break;
        }

    } while (opcion != 4);

    return 0;
}