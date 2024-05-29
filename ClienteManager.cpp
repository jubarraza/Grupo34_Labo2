#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "ClienteManager.h"
using namespace std;

ClienteManager::ClienteManager() : _archivo("Clientes.dat")
{
}

void ClienteManager::menu()
{
    int opcion;
    do {
        system("cls");
        cout << "MENU CLIENTES " << endl;
        cout << "********************** " << endl;
        cout << "1. Cargar Cliente " << endl;
        cout << "2. Mostrar Clientes " << endl;
        cout << "3. Crear backup " << endl;
        cout << "4. Restaurar backup " << endl;
        cout << "5. Editar Cliente " << endl;
        cout << "6. Borrar Cliente " << endl;

        cout << endl;
        cout << "0. SALIR DEL PROGRAMA " << endl;
        cout << "********************** " << endl;
        cout << "OPCION: " << endl;
        cin >> opcion;
        system("cls");

        switch (opcion) {
        case 1:
            agregarCliente();
            system("pause");
            break;

        case 2:
            listarClientes();
            system("pause");
            break;

        case 3:
            backupArchivo();
            system("pause");
            break;

        case 4:
            restaurarBackup();
            system("pause");
            break;

        case 5:
            editarCliente();
            system("pause");
            break;

        case 6:
            borrarCliente();
            system("pause");
            break;


        case 0:

            break;
        }
    } while (opcion != 0);
}

Cliente ClienteManager::crearCliente()
{
    string email;
    int tel;
    Direccion d;
    Cliente reg;

    cout << "------------- Ingreso de nuevo Cliente -------------" << endl;
    reg.Cargar();
    cin.ignore();
    cout << "EMAIL: ";
    getline(cin, email);
    reg.setEmail(email);
    cout << "TELEFONO: ";
    cin >> tel;
    reg.setTelefono(tel);
    cin.ignore();
    cout << "DIRECCION: " << endl;
    d.Cargar();
    reg.setDireccion(d);
    reg.setEliminado(false);

    return reg;
}

void ClienteManager::mostrarCliente(Cliente reg)
{
    reg.Mostrar();
    cout << "EMAIL: " << reg.getEmail() << endl;
    cout << "TELEFONO: " << reg.getTelefono() << endl;
    cout << "DIRECCION: ";
    reg.getDireccion().MostrarEnLinea();
}

void ClienteManager::agregarCliente()
{
    if (_archivo.guardarCliente(crearCliente())) {
        cout << "El cliente se guard� correctamente." << endl;
    }
    else {
        cout << "No se pudo guardar el cliente." << endl;
    }
}

void ClienteManager::listarClientes()
{
    int i, cantidad = _archivo.contarClientes();
    Cliente reg;

    cout << "**** Listado de Clientes ****" << endl;

    for (i = 0; i < cantidad; i++) {
        cout << "------------------------------" << endl;
        reg = _archivo.leerCliente(i);
        if (reg.getEliminado() == false) {
            mostrarCliente(reg);
        }
        cout << endl << "------------------------------" << endl << endl;
    }
}

int ClienteManager::buscarCliente(int dni)
{
    Cliente reg;
    FILE* p;
    int pos = 0;
    p = fopen(_archivo.getNombreArchivo().c_str(), "rb");
    if (p == nullptr) {
        return -2; //no se abrio el archivo
    }
    while (fread(&reg, sizeof reg, 1, p) == 1) {
        if (reg.getDni() == dni) {
            fclose(p); //cierro el archivo porque ya encontre la empresa
            return pos;
        }
        pos++;
    }
    fclose(p);

    return -1; //se recorrio el archivo y no existe el codigo
}

void ClienteManager::editarCliente()
{
    int dni, opcion;

    cout << "Ingrese DNI del Cliente a editar: ";
    cin >> dni;
    cin.ignore();
    cout << endl;

    int pos = buscarCliente(dni);

    if (pos >= 0) {
        Cliente reg;
        reg = _archivo.leerCliente(pos);

        if (reg.getEliminado() == false) {
            cout << endl << "Cliente a Editar: " << endl;
            mostrarCliente(reg);

            cout << endl;
            cout << "�Que dato desea editar?" << endl;
            cout << "1 - Email" << endl;
            cout << "2 - Telefono" << endl;
            cout << "3 - Direccion" << endl;
            cin >> opcion;
            cin.ignore();

            switch (opcion) {
            case 1:
            {
                string e;
                cout << "Ingrese nuevo email:";
                getline(cin, e);
                reg.setEmail(e);
                break;
            }
            case 2:
            {
                int tel;
                cout << "Ingrese nuevo telefono: ";
                cin >> tel;
                cin.ignore();
                reg.setTelefono(tel);
                break;
            }
            case 3:
            {
                Direccion d;
                cout << "Ingrese nueva direcci�n: ";
                d.Cargar();
                reg.setDireccion(d);
                break;
            }
            default:
                cout << "Opcion invalida.";
                break;
            }

            cout << endl;
            bool result = _archivo.sobreescribirCliente(reg, pos);

            if (result == true) {
                cout << "Se edit� correctamente el cliente." << endl;
            }
            else {
                cout << "No se pudo editar el cliente." << endl;
            }


        }
        else {
            cout << "El cliente buscado se encuentra eliminado" << endl;
        }

    }
    else {
        cout << "Error al buscar el cliente. Codigo error " << pos << endl;
    }
}

void ClienteManager::backupArchivo()
{
    string origen = "Clientes.dat";
    string copia = "Clientes.bak";

    string comando = "copy " + origen + " " + copia; 

    int resultado = system(comando.c_str()); 

    if (resultado == 0) {
        cout << endl << "Backup realizado con exito." << endl; 
    }
    else {
        cout << "Hubo un error al copiar el archivo." << endl; 
    }
}

void ClienteManager::restaurarBackup()
{
    string origen = "Clientes.bak";
    string copia = "Clientes.dat";

    string comando = "copy " + origen + " " + copia; 

    int resultado = system(comando.c_str()); 

    if (resultado == 0) {
        cout << endl << "Backup restaurado con exito." << endl; 
    }
    else {
        cout << "Hubo un error al restaurar el archivo." << endl; 
    }
}

void ClienteManager::borrarCliente()
{
    int dni;
    char opc;

    cout << "Ingrese DNI del Cliente a borrar: ";
    cin >> dni; 
    cin.ignore();
    cout << endl; 

    int pos = buscarCliente(dni);

    if (pos >= 0) {
        Cliente reg;
        reg = _archivo.leerCliente(pos);

        cout << endl << "Cliente a Borrar: " << endl;
        mostrarCliente(reg);
        cout << endl << "Confirma que desea borrar este Cliente? S/N" << endl;
        cin >> opc;

        if (opc == 's' || opc == 'S') {
            reg.setEliminado(true);
            bool result = _archivo.sobreescribirCliente(reg, pos);
        }
        else {
            cout << endl << "Se cancelo el borrado del cliente." << endl;
        }

    }
    else {
        cout << "El cliente buscado no existe" << endl;
    }
}