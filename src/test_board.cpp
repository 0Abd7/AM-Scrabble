#include "board.hpp"
#include "sac.hpp"
#include <iostream>
#include <sstream>

int main() {
  Board b ;
  Sac Sac;
  char val;
  std::cout << "Lettre aleatoire: " << Sac.alea() << std::endl;
  std::cout << "Entrez une lettre pour avoir le nombre de points :"<< endl;
  std::cin>> val;
  cout << "Score: " << Sac.Score(val)<<endl;
  
  for (unsigned i=0; i<Sac.lettres.size(); i++){
    cout << Sac.lettres[i];
  }
  std::cout<<endl;
/*  std::cout << b << std::endl ;

  std::stringstream ss ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "....JUSTIN....." << std::endl ;
  ss << ".......J......." << std::endl ;
  ss << ".......U......." << std::endl ;
  ss << ".......S......." << std::endl ;
  ss << ".......T......." << std::endl ;
  ss << ".....RIADH....." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;

  b.load(ss) ;

  std::cout << b << std::endl ;
*/
  return 0 ;
}
