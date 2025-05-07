//
// Created by cland on 06/05/2025.
//

#ifndef REPRODUCTOR_H
#define REPRODUCTOR_H
using namespace std;
#include <nlohmann/json.hpp>
struct Cancion{
    string Nombre, Artista;
    int Duracion;
    Cancion * sig;
    Cancion * ant;
    Cancion()
      : Nombre(), Artista(), Duracion(0),
        sig(nullptr), ant(nullptr)
    {}

    Cancion(const string &nombre, const string &artista,int duracion){
        Nombre = nombre;
        Artista = artista;
        Duracion = duracion;
        sig = ant = nullptr;
    }
};

class Reproductor {
private:
    Cancion *cabeza = nullptr;
public:

    Reproductor();

    bool empty();

    void insertarFinal(const string &nombre, const string &artista, int duracion);

    void eliminarFinal();

    int size();

    void insertarPosicion(const string &nombre, const string &artista, int duracion, int pos);

    void eliminarPosicion(int pos);

    void mostrar();

    void destruir();
};



#endif //REPRODUCTOR_H
