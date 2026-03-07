#include <iostream>
#include <string>

using namespace std;

void ConvertirBinario()
{
    string Binario;
    int Resultado = 0;

    cout << "\nConvertir Binario a Decimal\n\n";
    cout << "Ingrese un numero binario: ";
    cin >> Binario;

    // Validacion
    for (int i = 0; i < Binario.length(); i++)
    {
        if (Binario[i] != '0' && Binario[i] != '1')
        {
            cout << "Numero binario invalido.\n";
            return;
        }
    }

    for (int i = 0; i < Binario.length(); i++)
    {
        Resultado = Resultado * 2 + (Binario[i] - '0');
    }

    cout << "\nDecimal: " << Resultado << "\n";
}

void Arreglo()
{
    const int Max = 100;
    int Numero;

    cout << "\nArreglo Descendente\n\n";
    cout << "Ingrese un numero: ";
    cin >> Numero;

    // Validad errores al leer la entrada.
    if (cin.fail())
    {
        cout << "Entrada invalida.\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    // Validacion
    if (Numero <= 0)
    {
        cout << "Debe ingresar un numero positivo.\n";
        return;
    }

    if (Numero > Max)
    {
        cout << "Numero demasiado grande. Maximo permitido: " << Max << "\n";
        return;
    }

    int Arreglo[Max];

    cout << "\nResultado: ";

    for (int i = 0; i < Numero; i++)
    {
        Arreglo[i] = Numero - i;
        cout << Arreglo[i] << " ";
    }

    cout << "\n";
}

void InversoAditivo()
{
    const int Max = 100;
    int Numeros;

    cout << "\nInverso Aditivo\n\n";
    cout << "Ingrese la cantidad de numeros: ";
    cin >> Numeros;

    // Validad errores al leer la entrada.
    if (cin.fail())
    {
        cout << "Entrada invalida.\n";
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    // Validacion
    if (Numeros <= 0 || Numeros > Max)
    {
        cout << "Cantidad invalida.\n";
        return;
    }

    int Original[Max];
    int Inverso[Max];

    cout << "\nIngrese los numeros:\n";

    for (int i = 0; i < Numeros; i++)
    {
        cin >> Original[i];

        if (cin.fail())
        {
            cout << "Entrada invalida.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            return;
        }
    }

    for (int i = 0; i < Numeros; i++)
    {
        Inverso[i] = -Original[i];
    }

    cout << "\nArreglo inverso aditivo: ";

    for (int i = 0; i < Numeros; i++)
    {
        cout << Inverso[i] << " ";
    }

    cout << "\n";
}

void InvertirCadena()
{
    string Texto;
    int PuntoA, PuntoB;

    cout << "\nInvertir una cadena\n\n";

    cout << "Ingrese la cadena sin espacios: ";
    cin >> Texto;

    cout << "\nIngrese posicion A: ";
    cin >> PuntoA;
    cout << "\nIngrese posicion B: ";
    cin >> PuntoB;

    int Longitud = Texto.length();

    if (PuntoA < 0)
        PuntoA = 0;

    if (PuntoB >= Longitud)
        PuntoB = Longitud - 1;

    while (PuntoA < PuntoB)
    {
        char Temporal = Texto[PuntoA];
        Texto[PuntoA] = Texto[PuntoB];
        Texto[PuntoB] = Temporal;

        PuntoA++;
        PuntoB--;
    }

    cout << "\nResultado: " << Texto << endl;
}

int AddFive(int Numero)
{
    return Numero + 5;
}

int main()
{
    int Opcion;

    // Menu de opciones
    do
    {
        cout << "\nMenu\n\n";
        cout << "1. Convertir Binario\n";
        cout << "2. Arreglo Descendente\n";
        cout << "3. Inverso Aditivo\n";
        cout << "4. Invertir cadena\n";
        cout << "5. Sumar 5\n";
        cout << "0. Salir\n\n";

        cout << "Seleccione una opcion: ";
        cin >> Opcion;

        // Validacion
        if (cin.fail())
        {
            cout << "Opcion invalida.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        switch (Opcion)
        {
        case 1:
            ConvertirBinario();
            break;
        case 2:
            Arreglo();
            break;
        case 3:
            InversoAditivo();
            break;
        case 4:
            InvertirCadena();
            break;
        case 5:
            cout << "Resultado: " << AddFive(1) << "\n";
            break;
        case 0:
            cout << "\nSaliendo...\n";
            break;
        default:
            cout << "Opcion no valida.\n";
        }

    } while (Opcion != 0);

    cout << "Autor: Cesia Devian" << endl;
    cout << "Programacion I" << endl;

    return 0;
}