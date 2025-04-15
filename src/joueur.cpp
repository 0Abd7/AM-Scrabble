#include "joueur.hpp"
#include <iostream>
#include <algorithm>


Joueur::Joueur(Sac &sac)
{
    for (int i = 0; i < 7; ++i)
    { // Un joueur a 7 lettres par défaut
        chevalet.push_back(sac.alea());
    }
}

void Joueur::tirer(Sac& sac) {
    while (chevalet.size() < 7 && !sac.lettres.empty()) {
        chevalet.push_back(sac.alea());
    }
}

void Joueur::remplir_chevalet_manuellement(const std::vector<char>& lettres) {
    chevalet = lettres;
}


void Joueur::afficher_chevalet() const {
    std::cout << "Chevalet : ";
    for (char c : chevalet) {
        std::cout << c << ' ';
    }
    std::cout << std::endl;
}

bool Joueur::a_lettre(char c) const {
    return std::count(chevalet.begin(), chevalet.end(), c) > 0;
}

bool Joueur::peut_former(const std::string& mot) const {
    std::vector<char> temp = chevalet;
    for (char c : mot) {
        auto it = std::find(temp.begin(), temp.end(), c);
        if (it == temp.end())
            return false;
        temp.erase(it);
    }
    return true;
}

void Joueur::retirer_lettres(const std::string& mot) {
    for (char c : mot) {
        auto it = std::find(chevalet.begin(), chevalet.end(), c);
        if (it != chevalet.end()) {
            chevalet.erase(it);
        }
    }
}

std::string Joueur::getChevalet() const {
    std::string lettres;
    for (char lettre : chevalet) {
        lettres += lettre;
    }
    return lettres;
}

void Joueur::supprLettreDuChevalet(const std::string& mot) {
    std::string lettresRestantes = mot;
    for (auto it = chevalet.begin(); it != chevalet.end(); ) {
        auto pos = lettresRestantes.find(*it);
        if (pos != std::string::npos) {
            lettresRestantes.erase(pos, 1);
            it = chevalet.erase(it);
        } else {
            ++it;
        }
    }
}