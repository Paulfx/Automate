//******************************************************************************

#include "FA_tools.h"

////////////////////////////////////////////////////////////////////////////////

bool FromFile(sAutoNDE& at, std::string path){
 std::ifstream myfile(path.c_str(), std::ios::in); 
  //un flux d'entree obtenu à partir du nom du fichier
  std::string line;
  // un ligne lue dans le fichier avec getline(myfile,line);
  std::istringstream iss;
  // flux associé à la chaine, pour lire morceau par morceau avec >> (comme cin)
  etat_t s(0), t(0);
  // deux états temporaires
  symb_t a(0);
  // un symbole temporaire

  if (myfile.is_open()){
    // la première ligne donne 'nb_etats nb_symbs nb_finaux'
    do{ 
      getline(myfile,line);
    } while (line.empty() || line[0]=='#');
    // on autorise les lignes de commentaires : celles qui commencent par '#'
    iss.str(line);
    if((iss >> at.nb_etats).fail() || (iss >> at.nb_symbs).fail() || (iss >> at.nb_finaux).fail())
        return false;
    // la deuxième ligne donne l'état initial
    do{ 
      getline (myfile,line);
    } while (line.empty() || line[0]=='#');    
    iss.clear();
    iss.str(line);
    if((iss >> at.initial).fail())
      return -1;
    
    // les autres lignes donnent les états finaux
    for(size_t i = 0; i < at.nb_finaux; i++){
        do{ 
          getline (myfile,line);
        } while (line.empty() || line[0]=='#');
        iss.clear();
        iss.str(line);
         if((iss >> s).fail())
          continue;
//        cerr << "s= " << s << endl;
        at.finaux.insert(s);
    }     

    // on alloue les vectors à la taille connue à l'avance pour éviter les resize dynamiques
    at.epsilon.resize(at.nb_etats);
    at.trans.resize(at.nb_etats);
    for(size_t i=0;i<at.nb_etats;++i)
      at.trans[i].resize(at.nb_symbs);   

  // lecture de la relation de transition 
    while(myfile.good()){
      line.clear();
      getline (myfile,line);
      if (line.empty() && line[0]=='#')
        continue;
      iss.clear();
      iss.str(line); 

      // si une des trois lectures echoue, on passe à la suite
      if((iss >> s).fail() || (iss >> a).fail() || (iss >> t).fail() || (a< ASCII_A ) || (a> ASCII_Z ))
        continue; 
              
      //test espilon ou non
      if ((a-ASCII_A) >= at.nb_symbs){
//        cerr << "s=" << s<< ", (e), t=" << t << endl;
        at.epsilon[s].insert(t);
      }
      else{
//        cerr << "s=" << s<< ", a=" << a-ASCII_A << ", t=" << t << endl;
        at.trans[s][a-ASCII_A].insert(t);
      }
    }
    myfile.close();
    return true; 
 }
  return false;
  // on ne peut pas ouvrir le fichier
}

////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& out, const sAutoNDE& at){
  out << at.nb_etats << " " << at.nb_symbs << " " << at.nb_finaux << std::endl;
  out << std::endl;
  
  out << at.initial << std::endl;
  for(etatset_t::iterator it = at.finaux.begin() ; it != at.finaux.end() ; ++it )
      out << *it << std::endl;
  out << std::endl; 
  
  for(size_t i = 0 ; i < at.trans.size() ; ++i )
    for(size_t c = 0 ; c < at.trans[i].size() ; ++c )
      for(etatset_t::iterator it = at.trans[i][c].begin() ; it != at.trans[i][c].end() ; ++it )
        out << i << " " <<  (char)(c + ASCII_A) << " " << *it << std::endl;
  out << std::endl; 
  
  for(size_t i = 0 ; i < at.epsilon.size() ; ++i )
      for(etatset_t::iterator it = at.epsilon[i].begin() ; it != at.epsilon[i].end() ; ++it )
        //out << i << " ε " << *it << std::endl;
        out << i << " " << (char)(at.nb_symbs + ASCII_A) << " " << *it << std::endl;
  
  return out;
}

//******************************************************************************
