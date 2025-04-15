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


    int Sac::Score(char lettre)const{
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

    const std::map<char, int> Sac::lettrePoints = {
        {'A', 1}, {'B', 3}, {'C', 3}, {'D', 2}, {'E', 1},
        {'F', 4}, {'G', 2}, {'H', 4}, {'I', 1}, {'J', 8},
        {'K', 10}, {'L', 1}, {'M', 2}, {'N', 1}, {'O', 1},
        {'P', 3}, {'Q', 8}, {'R', 1}, {'S', 1}, {'T', 1},
        {'U', 1}, {'V', 4}, {'W', 10}, {'X', 10}, {'Y', 10}, {'Z', 10}
    };
