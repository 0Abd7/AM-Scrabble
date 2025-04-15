#include "gaddag.hpp"
#include "board.hpp"
#include "joueur.hpp"
#include "spot.hpp"
#include "direction.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <stack>
#include "board.hpp"

Gaddag::Gaddag()
{
    racine = new GaddagNode();
}

void Gaddag::ajouter_mot(const std::string &mot)
{
    for (size_t i = 1; i <= mot.size(); ++i)
    {
        std::string prefix = mot.substr(0, i);
        std::reverse(prefix.begin(), prefix.end());

        std::string suffix = mot.substr(i);

        std::string forme = prefix + "+" + suffix;

        // std::cout << "Ajout dans le GADDAG : " << forme << std::endl;
        GaddagNode *current = racine;
        for (char c : forme)
        {
            if (current->children.find(c) == current->children.end())
            {
                current->children[c] = new GaddagNode();
            }
            current = current->children[c];
        }
        current->terminal = true;
        //Chaque caractère devient un nœud dans l'arbre, avec le dernier nœud marqué comme terminal.
    }
}


void Gaddag::dictionnaire() {
    std::ifstream fichier("../data/dico.txt"); // Ouvre le fichier
    std::string ligne;

    if (fichier.is_open()) {
        std::cout << "Chargement du dictionnaire...\n";
        while (getline(fichier, ligne)) { 
            ajouter_mot(ligne);  // Insère chaque mot dans le Gaddag
            dictionnaire_complet.insert(ligne);
        }
        std::cout << "Chargement termine. " << dictionnaire_complet.size() << " mots charges.\n";
        fichier.close(); // Fermer le fichier après lecture
    } else {
        std::cout << "Erreur : Impossible de lire le fichier." << std::endl;
    }
}



bool Gaddag::cherche_mot_sans_plus(const std::string &mot) const
{
    // Générer toutes les formes Gaddag possibles pour le mot
    for (size_t i = 1; i <= mot.size(); ++i)
    {
        std::string prefix = mot.substr(0, i);
        std::reverse(prefix.begin(), prefix.end());
        std::string suffix = mot.substr(i);
        std::string forme = prefix + "+" + suffix;

        // Rechercher la forme transformée dans l'arbre
        GaddagNode *node = racine;
        bool found = true;
        for (char c : forme)
        {
            if (node->children.find(c) == node->children.end())
            {
                found = false;
                break;
            }
            node = node->children[c];
        }
        if (found && node->terminal)
        {
            return true; // Le mot existe sous cette forme
        }
    }
    return false; // Aucune forme transformée n'a été trouvée
} 

//recursive genere toutes les combinaison possibles 
void Gaddag::genereCombinaisons(const std::string &lettres, std::string actuelle, std::set<std::string> &combinations)
{
    if (!actuelle.empty())
    {
        combinations.insert(actuelle); // Ajouter la combinaison courante
    }
    for (size_t i = 0; i < lettres.size(); ++i)
    {
        genereCombinaisons(lettres.substr(0, i) + lettres.substr(i + 1), actuelle + lettres[i], combinations);
    }
}

std::vector<std::string> Gaddag::trouveMotValideChevalet(const Gaddag &gaddag, const std::string &lettresChevalet)
{
    std::set<std::string> combinaisons;
    genereCombinaisons(lettresChevalet, "", combinaisons);

    std::vector<std::string> motsValides;
    for (const std::string &mot : combinaisons)
    {
        if (gaddag.cherche_mot_sans_plus(mot))
        {
            motsValides.push_back(mot);
        }
    }
    //std::cout << "Nb mots valides trouves : " << motsValides.size() << std::endl;
    return motsValides;
}

//cree une copie du tableau pour tester toute formes si cest valides return vrai si passe ttes les verifs
bool Gaddag::checkMotsCroises(const Board &board, int ligne, int col, bool horizontal, const std::string &mot) {
    Board tempBoard = board;

    // Placer temporairement le mot sur la copie du plateau
    for (size_t i = 0; i < mot.size(); ++i) {
        int l = ligne + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);

        // Si la case est déjà occupée, vérifier que c'est la même lettre
        if (tempBoard(l, c).letter != 0 && tempBoard(l, c).letter != mot[i]) {
            return false;
        }

        tempBoard(l, c).letter = mot[i];
    }

    // Vérifier le mot principal
    std::string motPrincipal;
    if (horizontal) {
        // Construire le mot horizontal complet
        motPrincipal = getMotHorizontalComplet(tempBoard, ligne, col);
    }
    else {
        // Construire le mot vertical complet
        motPrincipal = getMotVerticalComplet(tempBoard, ligne, col);
    }

    // Vérifier que le mot principal est valide
    if (motPrincipal.length() > 1 && !cherche_mot_sans_plus(motPrincipal)) {
        return false;
    }

    // Vérifier tous les mots croisés
    for (size_t i = 0; i < mot.size(); ++i) {
        int l = ligne + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);

        // Vérifier seulement aux positions où nous avons ajouté une lettre
        // et où la position était vide sur le plateau original
        if (board(l, c).letter == 0) {
            std::string motCroise;

            if (horizontal) {
                // Vérifier le mot vertical croisé
                motCroise = getMotVerticalComplet(tempBoard, l, c);
            }
            else {
                // Vérifier le mot horizontal croisé
                motCroise = getMotHorizontalComplet(tempBoard, l, c);
            }

            // Si un mot croisé est formé, vérifier qu'il est valide
            if (motCroise.length() > 1 && !cherche_mot_sans_plus(motCroise)) {
                return false;
            }
        }
    }

    return true;
}

std::string Gaddag::getMotHorizontalComplet(const Board &board, int ligne, int col) const {
    std::string mot;
    // Aller à gauche jusqu'au début du mot
    int debutCol = col;
    while (debutCol > 0 && board(ligne, debutCol - 1).letter != 0) {
        debutCol--;
    }
    // Construire le mot complet vers la droite
    int colActuel = debutCol;
    while (colActuel < 15 && board(ligne, colActuel).letter != 0) {
        mot += board(ligne, colActuel).letter;
        colActuel++;
    }
    return mot;
}

std::string Gaddag::getMotVerticalComplet(const Board &board, int ligne, int col) const {
    std::string mot;
    // Remonter jusqu'au début du mot
    int debutLigne = ligne;
    while (debutLigne > 0 && board(debutLigne - 1, col).letter != 0) {
        debutLigne--;
    }
    // Construire le mot complet vers le bas
    int ligneActuelle = debutLigne;
    while (ligneActuelle < 15 && board(ligneActuelle, col).letter != 0) {
        mot += board(ligneActuelle, col).letter;
        ligneActuelle++;
    }
    return mot;
}
