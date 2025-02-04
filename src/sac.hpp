#include <iostream>
#include <vector>
#include <random>
using namespace std;

class Sac {
public:
    // Déclaration des vecteurs pour les éléments
    vector<char> lettres = {};
    vector<int> nombre = {9, 2, 2, 3, 15, 2, 2, 2, 8, 1, 1, 5, 3, 6, 6, 2, 1, 6, 6, 6, 6, 2, 1, 1, 1, 1};
    vector<int> points = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 10, 1, 2, 1, 1, 3, 8, 1, 1, 1, 1, 4, 10, 10, 10, 10};

    // Constructeur
    Sac();
    std::default_random_engine rng;
    char alea();
    int Score(char lettre);
    
};
