/**#include "Reproductor.h"
#include <iostream>
using namespace std;
#include <nlohmann/json.hpp>

Reproductor::Reproductor() : cabeza(nullptr) {}


bool Reproductor::empty(){
    return cabeza == nullptr;
}


void Reproductor::insertarFinal(const string &nombre, const string &artista, int duracion){
    auto nuevaCancion = new Cancion(nombre,artista,duracion);
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

void Reproductor::eliminarFinal(){
    if(empty()){
        cout<<"La playlist está vacía"<<endl;
        return;
    }
    if(cabeza -> sig == cabeza){ //o cabeza -> ant == cabeza, sirve para lo mismo
        delete cabeza;
        cabeza = nullptr;
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


int Reproductor::size(){
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

void Reproductor::insertarPosicion(const string &nombre, const string &artista, int duracion, int pos){
    if(pos > size()){
        cout<<"Posición fuera de rango, agregando canción al final de la playlist..."<<endl;
        insertarFinal(nombre, artista, duracion);
        return;
    }
    auto nuevo = new Cancion(nombre, artista, duracion);
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


void Reproductor::eliminarPosicion(int pos){
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
            cabeza = nullptr;
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


void Reproductor::mostrar(){
    if(empty()){
        cout<<"Playlist vacia, agrega Canciones"<<endl;
        return;
    }
    auto aux = cabeza;
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


void Reproductor::destruir(){
    if(empty()){
        cout<<"Lista vacía"<<endl;
        return;
    }
    Cancion * aux = cabeza, * siguiente = nullptr;
    do{
        siguiente = aux -> sig;
        delete aux;
        aux = siguiente;
    }while(aux != cabeza);
    cout<<"Se eliminó la PlayList"<<endl;
}*/