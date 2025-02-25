#include <iostream>
#include <string>
#include <vector>

using namespace std;

Gaddag::Gaddag(){
    racine = new Noeud;
}

void Gaddag::insererNoeud(Noeud* noeud, const string& mot, size_t index) {
    if (index == mot.length()) {
        noeud->motTermine = true; 
        return;
    }

    char lettre = mot[index];
    size_t idx = lettre - 'a'; 

    if (noeud->voisins[idx] == nullptr) {
        noeud->voisins[idx] = new Noeud();  
    }

    insererNoeud(noeud->voisins[idx], mot, index + 1);
}

void inserer(const string& mot) {
    insererNoeud(racine, mot, 0);
}