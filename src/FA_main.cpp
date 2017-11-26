//******************************************************************************

#include <iostream>
#include <algorithm>

#include "FA_types.h"
#include "FA_tools.h"

// première partie du projet
#include "Accept.h"
#include "Determinize.h"
#include "ToGraph.h"

// seconde partie du projet
#include "ExpressionRationnelle.h"
#include "parser.hpp"

#include "Minimize.h"
#include "Equivalence.h"

////////////////////////////////////////////////////////////////////////////////

void Help(std::ostream& out, char *s){
  out << "Utilisation du programme " << s << " :" << std::endl ;
  out << "-acc ou -accept Input Word :\n\t détermine si le mot Word est accepté par l'automate Input" << std::endl;
  out << "-det ou -determinize Input Output [-g] :\n\t déterminise l'automate Input, écrit le résultat dans Output" << std::endl;
  out << "-aut2expr ou automate2expressionrationnelle Input :\n\t calcule l'expression rationnelle correspondant à l'automate Input et l'affiche sur la sortie standard" << std::endl;
  out << "-min ou -minimisation Input Output [-g] :\n\t construit l'automate standard correspondant à Input, écrit le résultat dans Output" << std::endl;
  out << "-pequ ou -pseudo_equivalent Input1 Intput2 size :\n\t détermine si les deux automates Input1 et Input2 sont pseudo-équivalents par comparaison mot par mot de longueur < à size" << std::endl;
  out << "-equ ou -equivalent Input1 Intput2 :\n\t détermine si les deux automates Input1 et Input2 sont équivalents par minimisation et comparaison des tables de transition" << std::endl;
  out << "-nop ou -no_operation Input Output [-g] :\n\t ne fait rien, recopie juste l'entrée dans Output" << std::endl;
  out << "-g :\n\t paramètre optionnel pour avoir une sortie au format Graphviz dans Output" << std::endl;
  out << "\nExemple '" << s << " -determinize auto.txt resultat -g" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[] ){
  if(argc < 3){
    Help(std::cout, argv[0]);
    return EXIT_FAILURE;
  }
  
  int pos;
  int act=-1;                 // pos et act pour savoir quelle action effectuer
  int nb_ifiles = 0;          // nombre de fichiers en entrée
  int nb_ofiles = 0;          // nombre de fichiers en sortie
  std::string str, in1, in2, out, acc, expr;
  //sExpressionRationnelle er;
  unsigned int word_size_max; // pour la pseudo équivalence
  // chaines pour (resp.) tampon; fichier d'entrée Input1; fichier d'entrée Input2;
  // fichier de sortie et chaine dont l'acceptation est à tester 
  bool graphMode=false;     // sortie graphviz ?

  // options acceptées
  const size_t NBOPT = 8;
  std::string aLN[] = {"accept", "determinize", "automate2expressionrationnelle", "minimize", "pseudo-equivalent", "equivalent", "no_operation", "graph"};
  std::string aSN[] = {"acc", "det", "aut2expr", "min", "pequ", "equ", "nop", "g"};
  
  // on essaie de "parser" chaque option de la ligne de commande
  for(int i=1; i<argc; ++i){
    if (DEBUG) std::cerr << "argv[" << i << "] = '" << argv[i] << "'" << std::endl;
    str = argv[i];
    pos = -1;
    std::string* pL = find(aLN, aLN+NBOPT, str.substr(1));
    std::string* pS = find(aSN, aSN+NBOPT, str.substr(1));
    
    if(pL!=aLN+NBOPT)
      pos = pL - aLN;
    if(pS!=aSN+NBOPT)
      pos = pS - aSN;   
      
    if(pos != -1){
      // (pos != -1) <=> on a trouvé une option longue ou courte
      if (DEBUG) std::cerr << "Key found (" << pos << ") : " << str << std::endl;
      switch (pos) {
        case 0: //acc
          in1 = argv[++i];
          acc = argv[++i];
	  nb_ifiles = 1;
	  nb_ofiles = 0;
          break;
        case 1: //det
          in1 = argv[++i];
          out = argv[++i];
	  nb_ifiles = 1;
	  nb_ofiles = 1;
          break;
        case 2: //aut2expr
          in1 = argv[++i];
	  nb_ifiles = 1;
	  nb_ofiles = 0;
          break;
        case 3: //min
          in1 = argv[++i];
          out = argv[++i];
	  nb_ifiles = 1;
	  nb_ofiles = 1;
          break;
        case 4: //pequ
          in1 = argv[++i];
          in2 = argv[++i];
          word_size_max = atoi(argv[++i]);
	  nb_ifiles = 2;
	  nb_ofiles = 0;
          break;
        case 5: //equ
          in1 = argv[++i];
          in2 = argv[++i];
	  nb_ifiles = 2;
	  nb_ofiles = 0;
          break;
        case 6: //nop
          in1 = argv[++i];
          out = argv[++i];
	  nb_ifiles = 1;
	  nb_ofiles = 1;
          break;          
        case 7: //g
          graphMode = true;
          break;
        default:
          return EXIT_FAILURE;
        }
    }
    else{
      std::cerr << "Option inconnue "<< str << std::endl;
      return EXIT_FAILURE;
    }
    
    if(pos<7){
      if(act > -1){
        std::cerr << "Plusieurs actions spécififées"<< std::endl;
        return EXIT_FAILURE;
      }
      else
        act = pos;
    }    
  }
  
  if (act == -1){
    std::cerr << "Pas d'action spécififée"<< std::endl;
    return EXIT_FAILURE;
  }  

/* Les options sont OK, on va essayer de lire le(s) automate(s) at1 (et at2)
et effectuer l'action spécifiée. Atr stockera le résultat*/

  sAutoNDE at1, at2, atr;
  
  // lecture du des fichiers en entrée
  if ((nb_ifiles == 1 or nb_ifiles == 2) and !FromFile(at1, in1)){
    std::cerr << "Erreur de lecture " << in1 << std::endl;
    return EXIT_FAILURE;
  }  
  if (nb_ifiles ==2 and !FromFile(at2, in2)){
    std::cerr << "Erreur de lecture " << in2 << std::endl;
    return EXIT_FAILURE;
  }  
  
  switch(act) {
  case 0: //acc
    if (Accept(at1, acc)){
      std::cout << "'" << acc << "' est accepté : OUI\n";
    }
    else {
      std::cout << "'" << acc << "' est accepté : NON\n";
    }
    break;
  case 1: //det
    atr = Determinize(at1);
    break;
  case 2: //aut2expr
    expr =  Automate2ExpressionRationnelle(at1);
    //cout << "Expression rationnelle résultante (string) : " << expr << std::endl;
    //er = lit_expression_rationnelle(expr);
    std::cout << "Expression rationnelle résultante (ASA)    : " << expr << std::endl;
    break;
  case 3: //min
    atr =  Minimize(at1);
    break;
  case 4: //pseudo équivalence
    if (PseudoEquivalent(at1,at2,word_size_max)) {
      std::cout << "Après comparaison de tous les mots de longeur < à " << word_size_max << ", les deux automates sont pseudo-équivalents : OUI\n";
    }
    else {
      std::cout << "Après comparaison de tous les mots de longeur < à " << word_size_max << ", les deux automates sont pseudo-équivalents : NON\n";
    }
    break;
  case 5: //equ
    if (Equivalent(at1,at2)){
      std::cout << "les deux automates sont équivalents : OUI\n";
    }
    else {
      std::cout << "les deux automates sont équivalents : NON\n";
    }
    break;
  case 6: //nop
    atr = at1;
    break;
  default:
    return EXIT_FAILURE;
  }

  if (nb_ofiles == 1){
    // on affiche le résultat
    // cout << "Automate résultat :\n----------------\n";
    // cout << atr;

    // écriture dans un fichier texte
    std::ofstream f((out + ".txt").c_str(), std::ios::trunc); 
    if(f.fail())
      return EXIT_FAILURE;
    f << atr;    

    // génération d'un fichier graphviz
    if(graphMode){
      ToGraph(atr, out + ".gv");
      system(("dot -Tpng " + out + ".gv -o " + out + ".png").c_str());
    }
  }
  
  return EXIT_SUCCESS;
}



