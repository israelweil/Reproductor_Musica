#include<iostream>
using namespace std;
#include <nlohmann/json.hpp>
#include <fstream>
struct Cancion{
    string Nombre, Artista;
    int Duracion;
    Cancion * sig;
    Cancion * ant;
    Cancion()
      : Nombre(), Artista(), Duracion(0),
        sig(nullptr), ant(nullptr)
    {}

    Cancion(string nombre, string artista,int duracion){
        Nombre = nombre;
        Artista = artista;
        Duracion = duracion;
        sig = ant = NULL;
    }
};

Cancion * cabeza;
void inicializar(){
    cabeza = NULL;
}

bool empty(){
    return cabeza == NULL;
}

void insertarFinal(string nombre, string artista, int duracion){
    Cancion * nuevaCancion = new Cancion(nombre,artista,duracion);
    if(empty()){
        cabeza = nuevaCancion;
        cabeza -> sig = cabeza;
        cabeza -> ant = cabeza;
    }else{
        Cancion * aux = cabeza;
        //Nodo * aux = cabeza -> ant; Alternativa para el while
        while(aux -> sig != cabeza){
            aux = aux -> sig;
        }
        aux -> sig = nuevaCancion;
        nuevaCancion -> sig = cabeza;
        nuevaCancion -> ant = aux;
        cabeza -> ant = nuevaCancion;
    }
    cout<<"Se agregó la canción  "<<nombre<<endl;
}

void eliminarFinal(){
    if(empty()){
        cout<<"La playlist está vacía"<<endl;
        return;
    }
    if(cabeza -> sig == cabeza){ //o cabeza -> ant == cabeza, sirve para lo mismo
        delete cabeza;
        cabeza = NULL;
        cout<<"Se eliminó la primera canción"<<endl;
        return;
    }
    Cancion * aux = cabeza -> ant;
    string nombre = aux -> Nombre, artis = aux -> Artista;
    int duracion = aux -> Duracion;
    aux -> ant -> sig = cabeza;
    cabeza -> ant = aux -> ant;
    delete aux;
    cout<<"Se eliminó la canción  "<<nombre<<" de "<<artis<<" de duración "<<duracion<<"s"<<endl;
}

int size(){
    if(empty()){
        return 0;
    }else{
        Cancion * aux = cabeza;
        int cont = 1;
        while(aux -> sig != cabeza){
            aux = aux -> sig;
            ++cont;
        }
        return cont;
    }
}

void insertarPosicion(string nombre, string artista, int duracion, int pos){
    if(pos > size()){
        cout<<"Posición fuera de rango, agregando canción al final de la playlist..."<<endl;
        insertarFinal(nombre, artista, duracion);
        return;
    }
    Cancion * nuevo = new Cancion(nombre, artista, duracion);
    if(pos == 1){
        nuevo -> sig = cabeza;
        cabeza -> ant -> sig = nuevo;
        nuevo -> ant = cabeza -> ant;
        cabeza -> ant = nuevo;
        cabeza = nuevo;
        cout<<"Se agregó la canción "<<endl;
        return;
    }
    Cancion * aux = cabeza;
    int cont = 1;
    while(cont < pos-1){
        aux = aux -> sig;
        ++cont;
    }
    nuevo -> sig = aux -> sig;
    aux -> sig = nuevo;
    nuevo -> sig -> ant = nuevo;
    nuevo -> ant = aux;
    cout<<"Se agregó la canción "<<nombre<<endl;
}

void eliminarPosicion(int pos){
    if(empty()){
        cout<<"PlayList vacía"<<endl;
        return;
    }
    if(pos > size()){
        cout<<"Posición fuera de rango, agregando canción al final de la playlist..."<<endl;
        eliminarFinal();
        return;
    }
    if(pos == 1){
        if(cabeza -> sig == cabeza){
            delete cabeza;
            cabeza = NULL;
            cout<<"Se eliminó la cabeza"<<endl;
            return;
        }
        Cancion * aux = cabeza;
        cabeza -> ant -> sig = cabeza -> sig;
        cabeza = aux -> sig;
        cabeza -> ant = aux -> ant;
        delete aux;
        cout<<"Se eliminó la primera canción"<<endl;
        return;
    }
    int cont = 1;
    Cancion * aux = cabeza;
    while(cont < pos){
        aux = aux -> sig;
        ++ cont;
    }
    string nombre = aux -> Nombre, artis = aux -> Artista;
    int duracion = aux -> Duracion;
    aux -> ant -> sig = aux -> sig;
    aux -> sig -> ant = aux -> ant;
    delete aux;
    cout<<"Se eliminó la canción  "<<nombre<<" de "<<artis<<" de duración "<<duracion<<"s"<<endl;
}

void mostrar(){
    if(empty()){
        cout<<"Playlist vacia, agrega Canciones"<<endl;
        return;
    }
    Cancion * aux = cabeza;
    int numCancion=1;
    do{
        cout <<numCancion<< " > Canción: " << aux->Nombre
        << " | Artista: " << aux->Artista
        << " | Duración: " << aux->Duracion << "s" << endl;
        aux = aux -> sig;
        ++numCancion;
    } while(aux != cabeza);
    cout<<endl;
}

void destruir(){
    if(empty()){
        cout<<"Lista vacía"<<endl;
        return;
    }
    Cancion * aux = cabeza, * siguiente = NULL;
    do{
        siguiente = aux -> sig;
        delete aux;
        aux = siguiente;
    }while(aux != cabeza);
    cout<<"Se eliminó la PlayList"<<endl;
}


// Recorre la lista circular y llena un vector
vector<Cancion> exportarLista() {
    vector<Cancion> v;
    if (empty()) return v;
    Cancion* nodo = cabeza;
    do {
        // copia sin punteros (porque JSON no serializa punteros)
        Cancion copia{ nodo->Nombre, nodo->Artista, nodo->Duracion };
        v.push_back(copia);
        nodo = nodo->sig;
    } while (nodo != cabeza);
    return v;
}

// Borra la lista actual y reconstruye desde un vector
void importarLista(vector<Cancion> const& v) {
    destruir();
    inicializar();
    for (auto const& c : v) {
        insertarFinal(c.Nombre, c.Artista, c.Duracion);
    }
}
// convertir Cancion a JSON
void to_json(nlohmann::json& j, const Cancion& c) {
    j = nlohmann::json{
            {"Nombre", c.Nombre},
            {"Artista", c.Artista},
            {"Duracion", c.Duracion}
    };
}

// Función para convertir JSON a Cancion
void from_json(const nlohmann::json& j, Cancion& c) {
    j.at("Nombre").get_to(c.Nombre);
    j.at("Artista").get_to(c.Artista);
    j.at("Duracion").get_to(c.Duracion);
}
// Guarda en Musica.json
void guardarJSON(const string& archivo = "Musica.json") {
    auto vec = exportarLista();
    nlohmann::json j = vec;
    ofstream ofs(archivo);
    if(!ofs) {
        cerr << "No se pudo abrir " << archivo << " para escribir\n";
        return;
    }
    ofs << j.dump(4);
    cout << "Guardado JSON en " << archivo << "\n";
}

// Carga desde Musica.json
void cargarJSON(const string& archivo = "Musica.json") {
    ifstream ifs(archivo);
    if (!ifs) {
        cerr << "No se encontró " << archivo << ", lista vacía\n";
        return;
    }
    nlohmann::json j;
    ifs >> j;
    auto vec = j.get<vector<Cancion>>();
    importarLista(vec);
    cout << "Cargados " << vec.size() << " canciones desde JSON\n";
}


void pedirDatosCancion(string &nombre, string &artista, int &duracion){  //opcion a considerar
    cin.ignore();
    cout<<"Nombre de la canción: "; getline(cin, nombre);
    cout<<"Nombre del artista: "; getline(cin, artista);
    cout<<"Duración de la canción (en segundos): ";
    while (!(cin >> duracion) || duracion <= 0) {
        cout << "Caracter inválido. Ingrese una duración válida: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}


Cancion* obtenerPosCancion(int pos){  //Mi parte
    if(empty() || pos < 1 || pos > size()){
        return nullptr;
    }
    Cancion * aux = cabeza;
    int i = 1;
    while(i < pos){
        aux = aux -> sig;
        ++i;
    }
    return aux;
}

void reproductor(){  //Mi parte
    cout<<endl;
    if(empty()) {
        cout<<"La playlist está vacía."<<endl;
        return;
    } 
    int opcion, pos;
    Cancion* actual;

    cout<<"Reproductor de música\n"<<endl;
    mostrar();

    //cout <<"Menu inicial"<<endl;
    cout<<"1.Elegir canción\n"
        <<"2.Reproducir canción inicial\n"
        <<"3.Volver al menú principal"<<endl;
    cout<<"Selección: ";

    while (!(cin >> opcion) || opcion < 1 || opcion > 3) {
        cout << "Caracter inválido. Ingresa una opción valida: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }

    switch(opcion){
        case 1: 
            cout<<"Inserte la posicion de la cancion que desea reproducir: ";
            while (!(cin >> pos) || pos <= 0) {
                cout << "Caracter inválido. Ingrese una posición válida: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }
            actual = obtenerPosCancion(pos);
            if (actual == nullptr) {
                cout << "No se pudo encontrar la canción en esa posición." << endl;
                return;
            }
            break;
        case 2:
            actual = cabeza;
            break;
        case 3: cout << "\nSaliendo del reproductor...\n";
            return;
            break;
    }
    do {
        if (opcion != 3 && actual != nullptr) {
            cout << "\n==================================\n";
            cout << "   🎶 Reproduciendo ahora \n";
            cout << "-----------------------------------\n";
            cout << "   🎵 Canción : " << actual->Nombre << endl;
            cout << "   🎤 Artista : " << actual->Artista << endl;
            cout << "   ⏱️  Duración: " << actual->Duracion << " segundos" << endl;
            cout << "===================================\n";
        }
        if (actual != nullptr && actual->sig != nullptr && actual->sig != actual) {
            cout << "   ⏭ Siguiente canción \n";
            cout << "-----------------------------------\n";
            cout << "   🎵 Canción : " << actual->sig->Nombre << endl;
            cout << "   🎤 Artista : " << actual->sig->Artista << endl;
            cout << "   ⏱️  Duración: " << actual->sig->Duracion << " segundos" << endl;
            cout << "===================================\n";
        }

        cout<<"\nAcciones: "<<endl;
        cout<<"1.Siguiente\n"
                <<"2.Anterior\n"
                <<"3.Salir"<<endl;
        cout<<"Selección: ";
        while (!(cin >> opcion) || opcion < 1 || opcion > 3) {
            cout << "Caracter inválido. Ingresa una opción valida: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        switch(opcion){
            case 1: actual = actual->sig;
                break;
            case 2: actual = actual->ant;
                break;
            case 3: cout << "\nSaliendo del reproductor...\n";
                break;
        }
    }while(opcion != 3);
}

void menu(){
    int opc, duracion, pos;
    string nombre, artista;

    cout << "Bienvenido, Menu de opciones: " << endl;

    do {
        cout << "\nTu playList:" << endl;
        mostrar();
        cout << "\nSeleccione la opción que desee:\n"
             << "1. Agregar Canción\n"
             << "2. Eliminar Canción\n"
             << "3. Buscar Canción\n"
             << "4. Reproducir Canción\n"
             << "5. Salir\n";
        cin >> opc;

        int subOpc;
        switch(opc){
            case 1:
                cout << "Agregar Canción\n"
                     << "1. Al final\n"
                     << "2. En alguna posición\n";
                cin >> subOpc;

                if(subOpc == 1){
                    pedirDatosCancion(nombre, artista, duracion);
                    insertarFinal(nombre, artista, duracion);
                } else if(subOpc == 2){
                    pedirDatosCancion(nombre, artista, duracion);
                    cout << "Posición: "; cin >> pos;
                    insertarPosicion(nombre, artista, duracion, pos);
                } else {
                    cout << "Opción inválida.\n";
                }
                break;
            case 2:
                cout << "Eliminar Canción\n"
                     << "1. Al final\n"
                     << "2. En alguna posición\n";
                cin >> subOpc;

                if(subOpc == 1){
                    eliminarFinal();
                } else if(subOpc == 2){
                    cout << "Posición: "; cin >> pos;
                    eliminarPosicion(pos);
                } else {
                    cout << "Opción inválida.\n";
                }
                break;
            case 3:
                cout << "Función de Leo\n";
                break;
            case 4:
                cout << "Reproductor:\n";
                reproductor();
                break;
            case 5:
                cout << "¡Hasta luego!\n";
                break;
            default:
                cout << "Opción no válida. Intente de nuevo.\n";
                break;
        }
    } while(opc != 5);
}


int main(){
    inicializar();
    cargarJSON();
    menu();
    guardarJSON();
    destruir();
    return 0;
}