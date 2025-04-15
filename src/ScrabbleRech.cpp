#include "ScrabbleRech.hpp"
#include "gaddag.hpp"

ScrabbleRech::ScrabbleRech(Board& board, Gaddag& gaddag) : board(board), gaddag(gaddag) {}

bool ScrabbleRech::estVide() const {
    for (int i = 0; i < 225; ++i) {
        if (board.spots[i].letter != 0) return false;
    }
    return true;
}

void ScrabbleRech::placerMot(const std::string& mot, int ligne, int col, bool horizontal) {
    for (size_t i = 0; i < mot.size(); ++i) {
        int l = ligne + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        
        // Ne placer la lettre que si la case est vide
        if (board(l, c).letter == 0) {
            board(l, c).letter = mot[i];
        }
    }
}

// placer un mot verifie les limites ; case soit vide ; soit un mot
bool ScrabbleRech::peutPlacerMot(const std::string& mot, int ligne, int col, bool horizontal) const {
    // Vérifier les limites
    if (ligne < 0 || col < 0) return false;
    if (horizontal) {
        if (col + mot.length() > 15) return false;
    } else {
        if (ligne + mot.length() > 15) return false;
    }

    // Vérifier les conflits de lettres
    for (size_t i = 0; i < mot.size(); ++i) {
        int l = ligne + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        
        Spot spot = board(l, c);
        if (spot.letter != 0 && spot.letter != mot[i]) {
            return false;
        }
    }
    return true;
}

//verif les regles de connexion si plateau vide place au milieu sinon le mot doit se connecter a une lettre
bool ScrabbleRech::estLie(int ligne, int col, bool horizontal, const std::string& mot) const {
    if (estVide()) {
        // Premier mot doit couvrir le centre
        for (size_t i = 0; i < mot.size(); ++i) {
            int l = ligne + (horizontal ? 0 : i);
            int c = col + (horizontal ? i : 0);
            if (l == 7 && c == 7) return true;
        }
        return false;
    }

    bool aConnection = false;
    bool motContinue = true;  // Vérifier que le mot forme une ligne continue
    
    // Vérifier si le mot est continu (sans espaces)
    for (size_t i = 0; i < mot.size(); ++i) {
        int l = ligne + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        
        if (board(l, c).letter != 0) {
            // Cette case est déjà occupée par une lettre
            aConnection = true;
        } else {
            // Cette case est vide, vérifier qu'elle est adjacente à une lettre existante
            bool aLettreAdjacente = false;
            
            // Vérifier les quatre directions adjacentes
            if (l > 0 && board(l-1, c).letter != 0) aLettreAdjacente = true;
            if (l < 14 && board(l+1, c).letter != 0) aLettreAdjacente = true;
            if (c > 0 && board(l, c-1).letter != 0) aLettreAdjacente = true;
            if (c < 14 && board(l, c+1).letter != 0) aLettreAdjacente = true;
            
            if (aLettreAdjacente) {
                aConnection = true;
            }
        }
    }
    
    // Vérifier que les nouvelles lettres forment un mot continu
    int debut = -1;
    int fin = -1;
    
    // Trouver la première nouvelle lettre
    for (size_t i = 0; i < mot.size(); ++i) {
        int l = ligne + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        
        if (board(l, c).letter == 0) {
            debut = i;
            break;
        }
    }
    
    // Trouver la dernière nouvelle lettre
    for (int i = mot.size() - 1; i >= 0; --i) {
        int l = ligne + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        
        if (board(l, c).letter == 0) {
            fin = i;
            break;
        }
    }
    
    // Vérifier que toutes les positions entre debut et fin sont des nouvelles lettres ou connectées à des mots existants
    if (debut != -1 && fin != -1) {
        for (int i = debut; i <= fin; ++i) {
            int l = ligne + (horizontal ? 0 : i);
            int c = col + (horizontal ? i : 0);
            
            if (board(l, c).letter == 0) {
                // C'est une nouvelle lettre, vérifier qu'elle est adjacente à une lettre existante
                bool nvlLettrePossedeAdjacente = false;
                
                if (horizontal) {
                    // Vérifier horizontalement
                    if (c > 0 && board(l, c-1).letter != 0) nvlLettrePossedeAdjacente = true;
                    if (c < 14 && board(l, c+1).letter != 0) nvlLettrePossedeAdjacente = true;
                } else {
                    // Vérifier verticalement
                    if (l > 0 && board(l-1, c).letter != 0) nvlLettrePossedeAdjacente = true;
                    if (l < 14 && board(l+1, c).letter != 0) nvlLettrePossedeAdjacente = true;
                }
                
                if (!nvlLettrePossedeAdjacente) {
                    // Si cette lettre n'est pas adjacente à une lettre existante dans la direction du mouvement,
                    // vérifier qu'elle est adjacente à une lettre dans la direction perpendiculaire
                    bool aAdjacentPerpendiculaire = false;
                    
                    if (horizontal) {
                        // Vérifier verticalement
                        if (l > 0 && board(l-1, c).letter != 0) aAdjacentPerpendiculaire = true;
                        if (l < 14 && board(l+1, c).letter != 0) aAdjacentPerpendiculaire = true;
                    } else {
                        // Vérifier horizontalement
                        if (c > 0 && board(l, c-1).letter != 0) aAdjacentPerpendiculaire = true;
                        if (c < 14 && board(l, c+1).letter != 0) aAdjacentPerpendiculaire = true;
                    }
                    
                    if (!aAdjacentPerpendiculaire) {
                        // Si cette lettre n'est pas adjacente à une lettre existante dans aucune direction,
                        // vérifier que la case précédente dans la direction du mouvement est occupée par une nouvelle lettre
                        if (i > debut) {
                            int precL = ligne + (horizontal ? 0 : i-1);
                            int precC = col + (horizontal ? i-1 : 0);
                            
                            if (board(precL, precC).letter == 0) {
                                // La case précédente est vide, donc il y a un espace dans le mot
                                motContinue = false;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    
    return aConnection && motContinue;
}

//applique les bonus 
int ScrabbleRech::motScore(const std::string &mot, int ligne, int col, bool horizontal) const {
    int score = 0;
    int motMultiplier = 1; // Multiplicateur global pour le mot

    for (size_t i = 0; i < mot.size(); ++i) {
        int ligneActuelle = ligne + (horizontal ? 0 : i);
        int colActuelle = col + (horizontal ? i : 0);

        Spot spot = board(ligneActuelle, colActuelle);
        int lettreScore = Sac::lettrePoints.at(mot[i]); // Points de la lettre

        // Appliquer le multiplicateur de lettre
        lettreScore = spot.bonus.apply_letter(lettreScore);

        // Appliquer le multiplicateur de mot
        motMultiplier *= spot.bonus.apply_word(1); // On multiplie par 1 pour récupérer le facteur

        score += lettreScore;
    }

    return score * motMultiplier;
}

//parcourt toute le tableau place le mot dans les deux sens verifie les colisions calculer le score 
//return les placements possible avec leur score
std::vector<CoupPossible> ScrabbleRech::touvePlacementPossible(const std::string& lettresChevalet) {
    std::cout << "Recherche de placement avec chevalet : " << lettresChevalet << std::endl;

    std::vector<CoupPossible> placements;
    
    // Trouver toutes les cases adjacentes aux lettres existantes
    for (int ligne = 0; ligne < 15; ++ligne) {
        for (int col = 0; col < 15; ++col) {
            // Si cette case est vide mais adjacente à une lettre
            if (board(ligne, col).letter == 0) {
                bool lettreAdjacente = false;
                
                // Vérifier les cases adjacentes
                if ((ligne > 0 && board(ligne-1, col).letter != 0) ||
                    (ligne < 14 && board(ligne+1, col).letter != 0) ||
                    (col > 0 && board(ligne, col-1).letter != 0) ||
                    (col < 14 && board(ligne, col+1).letter != 0)) {
                    lettreAdjacente = true;
                }
                
                // Si le plateau est vide, vérifier la case centrale
                if (estVide() && ligne == 7 && col == 7) {
                    lettreAdjacente = true;
                }
                
                if (lettreAdjacente) {
                    // Pour chaque direction (horizontale et verticale)
                    for (bool horizontal : {true, false}) {
                        // Trouver les mots valides à partir des lettres du joueur
                        auto motsValides = gaddag.trouveMotValideChevalet(gaddag, lettresChevalet);
                        
                        for (const auto& mot : motsValides) {
                            // Pour chaque lettre du mot, essayer de le placer avec cette lettre sur la case actuelle
                            for (size_t lettreIndex = 0; lettreIndex < mot.size(); ++lettreIndex) {
                                int startLigne = ligne - (horizontal ? 0 : lettreIndex);
                                int startCol = col - (horizontal ? lettreIndex : 0);
                                
                                // Vérifier si le placement est valide
                                if (startLigne >= 0 && startCol >= 0 && 
                                    (horizontal ? startLigne < 15 && startCol + mot.size() <= 15 : 
                                                startLigne + mot.size() <= 15 && startCol < 15)) {
                                    
                                    if (peutPlacerMot(mot, startLigne, startCol, horizontal) &&
                                        estLie(startLigne, startCol, horizontal, mot) &&
                                        gaddag.checkMotsCroises(board, startLigne, startCol, horizontal, mot)) {
                                        
                                        int score = motScore(mot, startLigne, startCol, horizontal);
                                        placements.push_back({mot, startLigne, startCol, horizontal, score});
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    return placements;
}

bool ScrabbleRech::essaiePlaceMot(Joueur& joueur, int& totalScore) {
    std::string lettresChevalet = joueur.getChevalet();
    
    // Trouver tous les placements possibles
    auto placements = touvePlacementPossible(lettresChevalet);
    
    if (placements.empty()) {
        std::cout << "Aucun placement possible trouvé avec : " << lettresChevalet << std::endl;
        return false;
    }
    
    // Trouver le meilleur placement (celui avec le score le plus élevé)
    auto meilleurPlacement = *std::max_element(placements.begin(), placements.end(),
        [](const CoupPossible& a, const CoupPossible& b) {
            return a.score < b.score;
        });
    
    // Placer le mot
    placerMot(meilleurPlacement.mot, meilleurPlacement.ligne, meilleurPlacement.col, meilleurPlacement.horizontal);
    joueur.supprLettreDuChevalet(meilleurPlacement.mot);
    totalScore += meilleurPlacement.score;
    
    // Afficher le mot placé
    std::cout << "Mot placé : " << meilleurPlacement.mot
              << " en position (" << meilleurPlacement.ligne << "," << meilleurPlacement.col << ")"
              << " direction " << (meilleurPlacement.horizontal ? "horizontale" : "verticale")
              << " - Score : " << meilleurPlacement.score << std::endl;
    
    return true;
}