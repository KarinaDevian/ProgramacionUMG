#include <iostream>
#include <limits>
#include <string>
#include <cctype>

using namespace std;

// Limpia el buffer del teclado
void limpiarBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Clase Alumno
class Alumno {
public:
    string nombres;
    string apellidos;
    string carnet;
    string email;
    string telefono;
    double notas[5];

    // Calcula el promedio de las 5 notas
    double calcularPromedio() const {
        double suma = 0;

        for (int i = 0; i < 5; i++) {
            suma += notas[i];
        }

        return suma / 5.0;
    }

    // Solicita los datos del alumno
    void ingresarDatos() {

        cout << "Nombres: ";
        getline(cin, nombres);

        cout << "Apellidos: ";
        getline(cin, apellidos);

        cout << "Carnet: ";
        getline(cin, carnet);

        cout << "Email: ";
        getline(cin, email);

        cout << "Telefono: ";
        getline(cin, telefono);

        // Solicitud y validacion de notas
        for (int i = 0; i < 5; i++) {

            while (true) {

                cout << "Nota " << (i + 1) << " (0 - 10): ";
                cin >> notas[i];

                // Validar errores o valores fuera de rango
                if (cin.fail() || notas[i] < 0 || notas[i] > 10) {

                    cout << "Error: ingrese una nota valida entre 0 y 10.\n";

                    cin.clear();
                    limpiarBuffer();
                }
                else {

                    limpiarBuffer();
                    break;
                }
            }
        }
    }

    // Muestra la informacion del alumno
    void mostrarDatos() const {

        cout << "Nombres: " << nombres << "\n";
        cout << "Apellidos: " << apellidos << "\n";
        cout << "Carnet: " << carnet << "\n";
        cout << "Email: " << email << "\n";
        cout << "Telefono: " << telefono << "\n";

        for (int i = 0; i < 5; i++) {
            cout << "Nota " << (i + 1) << ": " << notas[i] << "\n";
        }

        cout << "Promedio: " << calcularPromedio() << "\n";
    }
};

// Nodo de la lista enlazada
struct Nodo {

    Alumno dato;
    Nodo* siguiente;
};

// Mostrar todos los alumnos de la lista
void mostrarLista(Nodo* cabeza) {

    if (cabeza == nullptr) {

        cout << "\nLa lista esta vacia.\n";
        return;
    }

    cout << "\n=====================================\n";
    cout << "         LISTA DE ALUMNOS\n";
    cout << "=====================================\n";

    Nodo* actual = cabeza;
    int contador = 1;

    while (actual != nullptr) {

        cout << "\nAlumno No. " << contador << "\n";
        cout << "-------------------------------------\n";

        actual->dato.mostrarDatos();

        actual = actual->siguiente;
        contador++;
    }

    cout << "=====================================\n";
}

// Liberar memoria dinamica
void liberarLista(Nodo*& cabeza) {

    Nodo* actual = cabeza;

    while (actual != nullptr) {

        Nodo* siguiente = actual->siguiente;

        delete actual;

        actual = siguiente;
    }

    cabeza = nullptr;

    cout << "\nMemoria liberada correctamente.\n";
}

// Funcion principal
int main() {

    Nodo* cabeza = nullptr;
    Nodo* ultimo = nullptr;

    char respuesta;

    cout << "=====================================\n";
    cout << "      REGISTRO DE ALUMNOS\n";
    cout << "=====================================\n";

    do {

        cout << "\nDesea ingresar un nuevo alumno? [S/N]: ";
        cin >> respuesta;

        respuesta = toupper(respuesta);

        limpiarBuffer();

        if (respuesta == 'S') {

            // Crear nuevo nodo dinamicamente
            Nodo* nuevo = new Nodo();

            cout << "\nIngrese los datos del alumno\n";
            cout << "-------------------------------------\n";

            nuevo->dato.ingresarDatos();

            nuevo->siguiente = nullptr;

            // Insertar nodo al final de la lista
            if (cabeza == nullptr) {

                cabeza = nuevo;
                ultimo = nuevo;
            }
            else {

                ultimo->siguiente = nuevo;
                ultimo = nuevo;
            }

            cout << "\nAlumno agregado correctamente.\n";
        }

    } while (respuesta == 'S');

    // Mostrar lista completa
    mostrarLista(cabeza);

    // Liberar memoria
    liberarLista(cabeza);

    return 0;
}