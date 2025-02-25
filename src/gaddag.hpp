#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

class Gaddag {


    struct Noeud{
        bool motTermine = false;
        array<Noeud*, 26> voisins = {};
    };

    Gaddag();
    Noeud* racine;
    void insertNoeud(Noeud* noeud, const string& mot, size_t index);
    inserer
}