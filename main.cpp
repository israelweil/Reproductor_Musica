#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

struct Cancion {
    string Nombre, Artista;
    int Duracion;
    Cancion *sig;
    Cancion *ant;
    Cancion()
        : Nombre(), Artista(), Duracion(0), sig(nullptr), ant(nullptr) {}
    Cancion(string nombre, string artista, int duracion) {
        Nombre = nombre;
        Artista = artista;
        Duracion = duracion;
        sig = ant = NULL;
    }
};

Cancion* cabeza = NULL;

void inicializar() {
    cabeza = NULL;
}

bool empty() {
    return cabeza == NULL;
}

void insertarFinal(string nombre, string artista, int duracion) {
    Cancion* nueva = new Cancion(nombre, artista, duracion);
    if (empty()) {
        cabeza = nueva;
        cabeza->sig = cabeza;
        cabeza->ant = cabeza;
    } else {
        Cancion* aux = cabeza;
        while (aux->sig != cabeza) {
            aux = aux->sig;
        }
        aux->sig = nueva;
        nueva->sig = cabeza;
        nueva->ant = aux;
        cabeza->ant = nueva;
    }
    cout << "Cancion agregada: " << nombre << endl;
}

void eliminarFinal() {
    if (empty()) {
        cout << "La lista esta vacia" << endl;
        return;
    }
    if (cabeza->sig == cabeza) {
        delete cabeza;
        cabeza = NULL;
        cout << "Se elimino la unica cancion" << endl;
        return;
    }
    Cancion* aux = cabeza->ant;
    aux->ant->sig = cabeza;
    cabeza->ant = aux->ant;
    delete aux;
    cout << "Se elimino la ultima cancion" << endl;
}

int size() {
    if (empty()) return 0;
    Cancion* aux = cabeza;
    int cont = 1;
    while (aux->sig != cabeza) {
        aux = aux->sig;
        cont++;
    }
    return cont;
}

void insertarPosicion(string nombre, string artista, int duracion, int pos) {
    if (pos > size()) {
        insertarFinal(nombre, artista, duracion);
        return;
    }
    Cancion* nuevo = new Cancion(nombre, artista, duracion);
    if (pos == 1) {
        nuevo->sig = cabeza;
        cabeza->ant->sig = nuevo;
        nuevo->ant = cabeza->ant;
        cabeza->ant = nuevo;
        cabeza = nuevo;
        return;
    }
    Cancion* aux = cabeza;
    int cont = 1;
    while (cont < pos - 1) {
        aux = aux->sig;
        cont++;
    }
    nuevo->sig = aux->sig;
    aux->sig = nuevo;
    nuevo->sig->ant = nuevo;
    nuevo->ant = aux;
}

void eliminarPosicion(int pos) {
    if (empty()) return;
    if (pos > size()) {
        eliminarFinal();
        return;
    }
    if (pos == 1) {
        if (cabeza->sig == cabeza) {
            delete cabeza;
            cabeza = NULL;
            return;
        }
        Cancion* aux = cabeza;
        cabeza->ant->sig = cabeza->sig;
        cabeza = aux->sig;
        cabeza->ant = aux->ant;
        delete aux;
        return;
    }
    Cancion* aux = cabeza;
    int cont = 1;
    while (cont < pos) {
        aux = aux->sig;
        cont++;
    }
    aux->ant->sig = aux->sig;
    aux->sig->ant = aux->ant;
    delete aux;
}

void mostrar() {
    if (empty()) {
        cout << "Lista vacia" << endl;
        return;
    }
    Cancion* aux = cabeza;
    int i = 1;
    do {
        cout << i++ << ". " << aux->Nombre << " - " << aux->Artista
             << " (" << aux->Duracion << "s)" << endl;
        aux = aux->sig;
    } while (aux != cabeza);
}

void destruir() {
    if (empty()) return;
    Cancion* aux = cabeza;
    Cancion* siguiente;
    do {
        siguiente = aux->sig;
        delete aux;
        aux = siguiente;
    } while (aux != cabeza);
    cabeza = NULL;
}

vector<Cancion> exportarLista() {
    vector<Cancion> v;
    if (empty()) return v;
    Cancion* nodo = cabeza;
    do {
        Cancion copia{nodo->Nombre, nodo->Artista, nodo->Duracion};
        v.push_back(copia);
        nodo = nodo->sig;
    } while (nodo != cabeza);
    return v;
}

void importarLista(const vector<Cancion>& v) {
    destruir();
    inicializar();
    for (auto const& c : v) {
        insertarFinal(c.Nombre, c.Artista, c.Duracion);
    }
}

void to_json(json& j, const Cancion& c) {
    j = json{{"Nombre", c.Nombre}, {"Artista", c.Artista}, {"Duracion", c.Duracion}};
}

void from_json(const json& j, Cancion& c) {
    j.at("Nombre").get_to(c.Nombre);
    j.at("Artista").get_to(c.Artista);
    j.at("Duracion").get_to(c.Duracion);
}

void guardarJSON(const string& archivo = "Musica.json") {
    vector<Cancion> vec = exportarLista();
    json j = vec;
    ofstream ofs(archivo);
    if (!ofs) {
        cerr << "No se pudo abrir " << archivo << " para guardar\n";
        return;
    }
    ofs << j.dump(4);
}

void cargarJSON(const string& archivo = "Musica.json") {
    ifstream ifs(archivo);
    if (!ifs) {
        cerr << "No se encontro el archivo " << archivo << endl;
        return;
    }
    json j;
    ifs >> j;
    vector<Cancion> vec = j.get<vector<Cancion>>();
    importarLista(vec);
}

void pedirDatosCancion(string& nombre, string& artista, int& duracion) {
    cin.ignore();
    cout << "Nombre: "; getline(cin, nombre);
    cout << "Artista: "; getline(cin, artista);
    cout << "Duracion (segundos): ";
    while (!(cin >> duracion) || duracion <= 0) {
        cout << "Duracion invalida: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

Cancion* obtenerPosCancion(int pos) {
    if (empty() || pos < 1 || pos > size()) return nullptr;
    Cancion* aux = cabeza;
    for (int i = 1; i < pos; i++) aux = aux->sig;
    return aux;
}

void reproductor() {
    if (empty()) {
        cout << "La lista esta vacia." << endl;
        return;
    }
    int opcion, pos;
    Cancion* actual;
    mostrar();
    cout << "1. Elegir cancion\n2. Reproducir primera\n3. Salir\n";
    cout << "Seleccion: ";
    while (!(cin >> opcion) || opcion < 1 || opcion > 3) {
        cout << "Opcion invalida: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    switch (opcion) {
        case 1:
            cout << "Posicion: ";
            while (!(cin >> pos) || pos <= 0) {
                cout << "Posicion invalida: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }
            actual = obtenerPosCancion(pos);
            break;
        case 2:
            actual = cabeza;
            break;
        default:
            return;
    }
    do {
        cout << "\nReproduciendo: " << actual->Nombre
             << " - " << actual->Artista
             << " (" << actual->Duracion << "s)" << endl;
        cout << "1. Siguiente\n2. Anterior\n3. Salir\nSeleccion: ";
        while (!(cin >> opcion) || opcion < 1 || opcion > 3) {
            cout << "Opcion invalida: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        switch (opcion) {
            case 1: actual = actual->sig; break;
            case 2: actual = actual->ant; break;
        }
    } while (opcion != 3);
}

void menu() {
    int opc, duracion, pos;
    string nombre, artista;
    do {
        cout << "\nMenu:\n1. Agregar\n2. Eliminar\n3. Reproducir\n4. Mostrar\n5. Salir\n";
        cout << "Opcion: ";
        cin >> opc;
        int sub;
        switch (opc) {
            case 1:
                cout << "1. Al final\n2. En posicion\n";
                cin >> sub;
                pedirDatosCancion(nombre, artista, duracion);
                if (sub == 1) {
                    insertarFinal(nombre, artista, duracion);
                } else if (sub == 2) {
                    cout << "Posicion: "; cin >> pos;
                    insertarPosicion(nombre, artista, duracion, pos);
                }
                break;
            case 2:
                cout << "1. Al final\n2. En posicion\n";
                cin >> sub;
                if (sub == 1) {
                    eliminarFinal();
                } else if (sub == 2) {
                    cout << "Posicion: "; cin >> pos;
                    eliminarPosicion(pos);
                }
                break;
            case 3:
                reproductor();
                break;
            case 4:
                mostrar();
                break;
        }
    } while (opc != 5);
}

int main() {
    inicializar();
    cargarJSON();
    menu();
    guardarJSON();
    destruir();
    return 0;
}