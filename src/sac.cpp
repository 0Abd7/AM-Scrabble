#include <iostream>
#include <vector>
#include <random>
#include "sac.hpp"
using namespace std;

 // Constructeur
    Sac::Sac() {
        // Appel à la fonction alea()
        std::random_device hwseed;
        rng.seed( hwseed() );

        //alea();
        for (unsigned i=0; i<nombre.size();i++){
            for (int j=0 ; j<nombre[i]; j++){
                lettres.push_back('A'+i);
            }
        }
    }

    // Méthode pour générer des nombres aléatoires
    char Sac::alea(){
        std::uniform_int_distribution<int> uniform(0, lettres.size()-1); // Distribution uniforme entre 0 et N
        unsigned int i = uniform(rng);
        char stock = lettres[i];
        char store = lettres.back();
        lettres[i] = store;
        lettres.pop_back();
        return stock;
    }

    int Sac::Score(char lettre){
        if (lettre >= 'A' && lettre <= 'Z')
            return points[lettre-'A'];
        else {
            if (lettre >= 'a' && lettre <= 'z')
                return points[(lettre - 'a')];
            else {
                return 0;
            }
        }
    }