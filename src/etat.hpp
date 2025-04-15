#pragma once

#include <vector>
#include "gaddag.hpp"
#include "direction.hpp"

// Déclaration anticipée (on ne connaît que le nom ici)
struct GaddagNode;

struct Etat {
    int position; // index du plateau (0 à 224)
    std::vector<char> lettres_restantes;
    GaddagNode* noeud;
    Direction direction;
    std::vector<int> lettres_posees;
    bool plus_utilise;
    std::string mot_construit;


    Etat(int pos, const std::vector<char>& lettres, GaddagNode* n, Direction dir, 
         std::vector<int> posees, bool plus, std::string mot = "")
        : position(pos), lettres_restantes(lettres), noeud(n), 
          direction(dir), lettres_posees(posees), plus_utilise(plus), mot_construit(mot) {}
};