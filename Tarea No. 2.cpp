//PROCESAMIENTO DE ARCHIVOS EN C++
//Permite trabajar con archivos de texto y binarios.

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <string>

using namespace std;

// Estructura para archivo binario
//Se utiliza en archivos binarios para almacenar datos de tamaño fijo.
struct Alumno {
    int ID;
    char Nombre[30];
    float Nota;
};

// ARCHIVO DE TEXTO

//Funcion que agrega un nuevo alumno
//Append para no sobrescribir datos.
void AgregarTexto() {
    ofstream archivo("Alumnos.txt", ios::app);

    if (!archivo.is_open()) {
        cout << "Error al abrir archivo\n";
        return;
    }

    int ID;
    string Nombre;
    float Nota;

    cout << "ID: "; cin >> ID;
    cin.ignore();

    cout << "Nombre: ";
    getline(cin, Nombre);

    cout << "Nota: "; cin >> Nota;

    archivo << ID << "|" << Nombre << "|" << Nota << endl;

    archivo.close();
}

//Funcion para leer el archivo linea por linea
//Permite mostrar nombres con espacios
void ListarTexto() {
    ifstream archivo("Alumnos.txt");

    if (!archivo.is_open()) {
        cout << "Error al abrir archivo\n";
        return;
    }

    string linea;

    while (getline(archivo, linea)) {
        int ID;
        string Nombre;
        float Nota;

        size_t p1 = linea.find("|");
        size_t p2 = linea.find("|", p1 + 1);

        if (p1 == string::npos || p2 == string::npos) continue;

        ID = stoi(linea.substr(0, p1));
        Nombre = linea.substr(p1 + 1, p2 - p1 - 1);
        Nota = stof(linea.substr(p2 + 1));

        cout << ID << " | " << Nombre << " | " << Nota << endl;
    }

    archivo.close();
}

//FUncion para buscar a un alumno por su ID
void BuscarTexto() {
    ifstream archivo("Alumnos.txt");

    if (!archivo.is_open()) {
        cout << "Error al abrir archivo\n";
        return;
    }

    int buscar;
    cout << "ID a buscar: ";
    cin >> buscar;

    string linea;
    bool encontrado = false;

    while (getline(archivo, linea)) {
        int ID;
        string Nombre;
        float Nota;

        size_t p1 = linea.find("|");
        size_t p2 = linea.find("|", p1 + 1);

        if (p1 == string::npos || p2 == string::npos) continue;

        ID = stoi(linea.substr(0, p1));
        Nombre = linea.substr(p1 + 1, p2 - p1 - 1);
        Nota = stof(linea.substr(p2 + 1));

        if (ID == buscar) {
            cout << "Encontrado: " << Nombre << " | " << Nota << endl;
            encontrado = true;
        }
    }

    if (!encontrado) cout << "No encontrado\n";

    archivo.close();
}

//Funcion para modificar un archivo de texto
//Se utiliza un archivo temporal
void ActualizarTexto() {
    ifstream archivo("Alumnos.txt");
    ofstream temp("Temp.txt");

    if (!archivo.is_open()) {
        cout << "Error al abrir archivo\n";
        return;
    }

    int buscar;
    cout << "ID a actualizar: ";
    cin >> buscar;

    string linea;

    while (getline(archivo, linea)) {
        int ID;
        string Nombre;
        float Nota;

        size_t p1 = linea.find("|");
        size_t p2 = linea.find("|", p1 + 1);

        if (p1 == string::npos || p2 == string::npos) continue;

        ID = stoi(linea.substr(0, p1));
        Nombre = linea.substr(p1 + 1, p2 - p1 - 1);
        Nota = stof(linea.substr(p2 + 1));

        if (ID == buscar) {
            cout << "Nueva Nota: ";
            cin >> Nota;
        }

        temp << ID << "|" << Nombre << "|" << Nota << endl;
    }

    archivo.close();
    temp.close();

    remove("Alumnos.txt");
    rename("Temp.txt", "Alumnos.txt");

    cout << "Actualizado correctamente\n";
}

// ARCHIVO BINARIO

//Funcion que guarda datos en formato binario
void AgregarBinario() {
    ofstream archivo("Alumnos.dat", ios::binary | ios::app);

    if (!archivo.is_open()) {
        cout << "Error al abrir archivo\n";
        return;
    }

    Alumno a;

    cout << "ID: "; cin >> a.ID;
    cin.ignore();

    cout << "Nombre: ";
    cin.getline(a.Nombre, 30);

    cout << "Nota: "; cin >> a.Nota;

    archivo.write((char*)&a, sizeof(a));

    archivo.close();
}

//Funcion para leer un registro en cierta posicion
//Se utiliza acceso aleatorio
void LeerBinario() {
    ifstream archivo("Alumnos.dat", ios::binary);

    if (!archivo.is_open()) {
        cout << "Error al abrir archivo\n";
        return;
    }

    int pos;
    cout << "Numero de registro (0,1,2...): ";
    cin >> pos;

    Alumno a;

    archivo.seekg(pos * sizeof(a));

    if (archivo.read((char*)&a, sizeof(a))) {
        cout << a.ID << " | " << a.Nombre << " | " << a.Nota << endl;
    } else {
        cout << "Registro no existe\n";
    }

    archivo.close();
}

//Recorre el archivo binario leyendo registro por registro
void ListarBinario() {
    ifstream archivo("Alumnos.dat", ios::binary);

    if (!archivo.is_open()) {
        cout << "Error al abrir archivo\n";
        return;
    }

    Alumno a;

    while (archivo.read((char*)&a, sizeof(a))) {
        cout << a.ID << " | " << a.Nombre << " | " << a.Nota << endl;
    }

    archivo.close();
}

// MENU
//Muestra las opciones segun clasificiacion: Texto y Binario

int main() {
    int Opcion;

    while (true) {
        cout <<"\nArchivo de Texto\n";
        cout << "1. Agregar TXT\n2. Listar\n3. Buscar\n4. Actualizar\n";
        cout <<"Archivo Binario\n";
        cout << "5. Agregar\n6. Leer\n7. Listar"<<endl;
        cout<<"\n0. Salir\n";
        cin >> Opcion;

        if (Opcion == 0) break;

        if (Opcion == 1) AgregarTexto();
        if (Opcion == 2) ListarTexto();
        if (Opcion == 3) BuscarTexto();
        if (Opcion == 4) ActualizarTexto();
        if (Opcion == 5) AgregarBinario();
        if (Opcion == 6) LeerBinario();
        if (Opcion == 7) ListarBinario();
    }

    return 0;
}
