//******************************************************************************

#include <iostream>
#include <cassert>
#include <tuple>
#include <list>

#include "Equivalence.h"

#include "Determinize.h"

////////////////////////////////////////////////////////////////////////////////
sAutoNDE Arrange(const sAutoNDE& at){
  sAutoNDE r;

  r.nb_etats=at.nb_etats+2;//2 états ajoutés : l'état départ et l'état final
  r.nb_symbs=at.nb_symbs;
  r.nb_finaux=1;
  r.finaux={r.nb_etats-1};//Un seul état final
  r.initial=0;

  //Resize trans
  r.trans.resize(r.nb_etats);
  for(unsigned int i=0; i<r.nb_etats; ++i) r.trans[i].resize(r.nb_symbs);

  //Copie
	for(unsigned int i=0; i<at.nb_etats; ++i) {
		for(unsigned int ch=0; ch<at.nb_symbs; ++ch) {
  			for(const auto& it:at.trans[i][ch]) r.trans[i+1][ch].insert(it+1);
  		}
	}

  //Resize epsilon
  r.epsilon.resize(r.nb_etats);

  r.epsilon[0].insert(at.initial+1);//On rajoute l'epsilon-transition de l'état initial
  //Copie
  for(unsigned int i=0; i<at.epsilon.size(); ++i) {
  	for(auto it:at.epsilon[i]) r.epsilon[i+1].insert(it+1);
  }
  for(const auto& it:at.finaux) r.epsilon[it+1].insert(r.nb_etats-1);

  return r;
}


sExpressionRationnelle InitExpressionRationnelle(const sAutoNDE& at, unsigned int etatDepart, unsigned int etatArrive) {
  //TODO renvoie la str contenu dans la flèche de etatDepart à etatArrive dans at, si elle n'existe pas, renvoie str vide

  sExpressionRationnelle res = 0;
  char vecStr[4] = {'a', 'b', 'c', 'd'};
  std::string var;

  for(unsigned int symb=0; symb<at.nb_symbs; ++symb) {
    //S'il existe une transition de etatDepart à etatArrive, on l'ajoute (ou on crée la variable)
    if(at.trans[etatDepart][symb].find(etatArrive) != at.trans[etatDepart][symb].end()) {
      var = vecStr[symb];
      if(res == 0)
        res = variable(var);

      else res = concat(res, variable(var));
    }
  }
  //Sinon s'il existe une epsilon transition, on rajoute {e} (ou la variable associée à 'e')
  if(at.epsilon[etatDepart].find(etatArrive) != at.epsilon[etatDepart].end()) {
    var = "e";
    if(res == 0)
      res=variable(var);

    else res = concat(res, variable(var));    
  }

  return res;
}

std::string Automate2ExpressionRationnelle2(sAutoNDE at){

  //at = Determinize(at);  

  sAutoNDE atArr=Arrange(at);

  std::map<std::tuple<unsigned int,unsigned int>,sExpressionRationnelle> mapR;
  //Cas de base :
  //Remplissage de mapR pour k=0 (ie. R(i,j,0) )
  for(unsigned int i=0; i<atArr.nb_etats; ++i) {
    for(unsigned int j=1; j<atArr.nb_etats; ++j) {
      sExpressionRationnelle expr = InitExpressionRationnelle(atArr, i, j);
      if (expr != 0)
        mapR[std::make_tuple(i,j)]=expr;
    }
  }

  sExpressionRationnelle exprIJ;
  sExpressionRationnelle exprIK;
  sExpressionRationnelle exprKJ;
  sExpressionRationnelle exprBoucle;

  for(unsigned int k=1; k < atArr.nb_etats-1; ++k) { //Les états à supprimer
    std::cout<<k<<std::endl;
    for(unsigned int etatDepart=0; etatDepart<atArr.nb_etats; ++etatDepart) {
      std::cout<<"---"<<etatDepart<<std::endl;
      if(etatDepart == k) continue;
      auto itIK = mapR.find(std::make_tuple(etatDepart,k));
      if(itIK != mapR.end()) {//On a trouvé un état possédant une flèche vers k
        for(unsigned int etatArrive=1; etatArrive < atArr.nb_etats; ++etatArrive) {
          std::cout<<"------"<<etatArrive<<std::endl;
          //|| etatArrive == etatDepart
          if(etatArrive == k ) continue;
          auto itKJ = mapR.find(std::make_tuple(k,etatArrive));
          if(itKJ != mapR.end()) { //On a trouvé un état k avec une flèche de i vers k ET une flèche de k à j
            auto itKK = mapR.find(std::make_tuple(k,k));
            auto itIJ = mapR.find(std::make_tuple(etatDepart, etatArrive));
            exprIJ = 0; 
            exprIK = 0;
            exprKJ = 0;
            exprBoucle = 0;
            
            //Boucle de K à K
            if(itKK != mapR.end())
              exprBoucle = etoile(itKK->second);

            //Transition de etatDepart à etatArrivee
            if(itIJ != mapR.end())
              exprIJ = itIJ->second;
            
            exprIK=itIK->second;

            exprKJ=itKJ->second;
            
            if (exprBoucle != 0)
              exprIK = concat(exprIK, concat(exprBoucle, exprKJ));
            else 
              exprIK = concat(exprIK, exprKJ);

            if (exprIJ != 0) 
              exprIJ = ou(exprIJ, exprIK);
            else
              exprIJ = exprIK;

            mapR[std::make_tuple(etatDepart,etatArrive)] = exprIJ;
            //std::cout<<"etatDep : "<<etatDepart<<", etatArr : "<<etatArrive<<", K : "<<k<<std::endl<<", sBoucle : "<<sBoucle<<", sIJ :"<<sIJ<<", sIK :"<<sIK<<", sKJ : "<<sKJ<<std::endl<<"-----------------------------------------------------------------------"<<std::endl;
          }
        }
      }
    }
    //On a supprimé l'état k, on supprime de notre table de transition toutes les flèches 
    //allant vers k ou partant de k
    for(auto it=mapR.begin(); it!=mapR.end(); ++it)
      if (std::get<0>(it->first) == k || std::get<1>(it->first) == k) 
        mapR.erase(it);
  }

  return expression_rationnelle2string(mapR[std::make_tuple(0,atArr.nb_etats-1)]);
}



/* ----------------------------------------- */


std::string Transition2ExpressionRationnelle(const sAutoNDE& at, unsigned int etatDepart, unsigned int etatArrive) {
	//TODO renvoie la str contenu dans la flèche de etatDepart à etatArrive dans at, si elle n'existe pas, renvoie str vide
  
  std::string res="";
  char vecStr[4] = {'a', 'b', 'c', 'd'};

  for(unsigned int symb=0; symb<at.nb_symbs; ++symb) {
    //S'il existe une transition de etatDepart à etatArrive, on l'ajoute
    if(at.trans[etatDepart][symb].find(etatArrive) != at.trans[etatDepart][symb].end()) {
      if(!res.empty()) res+="|";
      res+=vecStr[symb];
    }
  }
  //Sinon s'il existe une epsilon transition, on renvoie {e} (ou | {e})
  if(at.epsilon[etatDepart].find(etatArrive) != at.epsilon[etatDepart].end()) {
    if(res.empty()) res="{e}";
    else res+="|{e}";    
  }
  return res;
}

std::string Automate2ExpressionRationnelle(sAutoNDE at){
  //TODO définir cette fonction
  return Automate2ExpressionRationnelle2(at);
  /*
  std::string sr;
  
  sAutoNDE atArr=Arrange(at);
  std::string sIJ;
  std::string sIK;
  std::string sKJ;
  std::string sBoucle;

  std::map<std::tuple<unsigned int,unsigned int>,std::string> mapR;
  //Cas de base :
  //Remplissage de mapR pour k=0 (ie. R(i,j,0) )
  for(unsigned int i=0; i<atArr.nb_etats; ++i) {
    for(unsigned int j=1; j<atArr.nb_etats; ++j) {
      sIJ=Transition2ExpressionRationnelle(atArr, i, j);
      if(!sIJ.empty()) {
        if(sIJ=="{e}") mapR[std::make_tuple(i,j)]="";
        else mapR[std::make_tuple(i,j)]=sIJ;
      }
    }
  }

  for(unsigned int k=1; k<atArr.nb_etats-1; ++k) { //Les états à supprimer
    std::cout<<k<<std::endl;
    for(unsigned int etatDepart=0; etatDepart<atArr.nb_etats; ++etatDepart) {
      std::cout<<"---"<<etatDepart<<std::endl;
      if(etatDepart == k) continue;
      auto itIK = mapR.find(std::make_tuple(etatDepart,k));
      if(itIK != mapR.end()) {//On a trouvé un état possédant une flèche vers k
        for(unsigned int etatArrive=1; etatArrive<atArr.nb_etats; ++etatArrive) {
          std::cout<<"------"<<etatArrive<<std::endl;
          if(etatArrive == k) continue;
          auto itKJ = mapR.find(std::make_tuple(k,etatArrive));
          if(itKJ != mapR.end()) { //On a trouvé un état k avec une flèche de i vers k ET une flèche de k à j
            auto itKK = mapR.find(std::make_tuple(k,k));
            auto itIJ = mapR.find(std::make_tuple(etatDepart, etatArrive));
            //Boucle de K à K
            if(itKK != mapR.end()) {
              sBoucle = itKK->second;
              if(sBoucle.length()>1) sBoucle="("+sBoucle+")";
              sBoucle+="*.";
            }
            else sBoucle = "";

            //Transition de etatDepart à etatArrivee
            if(itIJ != mapR.end()) {
              sIJ = itIJ->second;
              if(sIJ.length()>=1) {
                //if(sIJ[sIJ.length()-1]=='.') sIJ.pop_back();//
                sIJ+="|";
              }
            }
            else sIJ = "";
            

            sIK=itIK->second;
            if(sIK.length()>=1 && sIK[sIK.length()-1]!='.') sIK+=".";

            sKJ=itKJ->second;
            if(sKJ.length()>=1 && sKJ[sKJ.length()-1]!='.') sKJ+=".";
            
            std::cout<<"etatDep : "<<etatDepart<<", etatArr : "<<etatArrive<<", K : "<<k<<std::endl<<", sBoucle : "<<sBoucle<<", sIJ :"<<sIJ<<", sIK :"<<sIK<<", sKJ : "<<sKJ<<std::endl<<"-----------------------------------------------------------------------"<<std::endl;

            //mapR[std::make_tuple(etatDepart,etatArrive,k)] = sIJ+"("+itIK->second+")."+sBoucle+"("+itKJ->second+")";
            mapR[std::make_tuple(etatDepart,etatArrive)] = "("+sIJ+sIK+sBoucle+sKJ+")";
          }
        }
      }
      
    }
    //On a supprimé l'état k, on supprime de notre table de transition toutes les flèches 
    //allant vers k ou partant de k
    for(std::map<std::tuple<unsigned int,unsigned int>,std::string>::iterator it=mapR.begin(); it!=mapR.end(); ++it) {
      if (std::get<0>(it->first)==k || std::get<1>(it->first)==k) mapR.erase(it);
    }
  }

  //Calcul de sr
  //sr=mapR[std::make_tuple(0,atArr.nb_etats-1,atArr.nb_etats-2)];
  sr=mapR[std::make_tuple(0,atArr.nb_etats-1)];
  if(sr[sr.length()-1]=='.' || sr[sr.length()-1]=='|') 
    sr.pop_back();
  return sr;*/
}

////////////////////////////////////////////////////////////////////////////////

void permutations(std::list<std::string>& r, unsigned int len, size_t nbSymb) {
  //std::cout<<r.size()<<std::endl;
  if (len != 0) {
    //std::cout<<len<<std::endl;
    permutations(r ,len - 1, nbSymb);
    for (unsigned char ch = 'a'; ch <= ASCII_A + nbSymb - 1; ch++) {
      std::cout<<len<<"   "<<ch<<std::endl;
      if (len == 1) r.emplace_back(1, ch); //On crée la std::string contenant ch et on l'append
      auto fin = --r.end();//Dernier élément;
      for(auto itMot = r.begin(); itMot != fin; ++itMot) {
        r.push_back(std::string(1, ch) + *itMot);
      }
      r.push_back(std::string(1, ch) + *fin);
    }
  }
}



bool PseudoEquivalent(const sAutoNDE& a1, const sAutoNDE& a2, unsigned int word_size_max) {
  //TODO définir cette fonction
  if (a1.nb_symbs != a2.nb_symbs) return false;
  size_t nbSymbs = a1.nb_symbs;
  std::list<std::string> listMots;
  std::cout<<listMots.size()<<std::endl;
  permutations(listMots, word_size_max, nbSymbs);

  listMots.sort();
  listMots.unique();

  for (const auto& mot : listMots)
    std::cout<<mot<<std::endl;

  for (const auto& mot : listMots)
    if(Accept(a1, mot) != Accept(a2, mot)) return false;
  return true;
}

////////////////////////////////////////////////////////////////////////////////

bool Equivalent(const sAutoNDE& a1, const sAutoNDE& a2) {
  //TODO définir cette fonction

  return true;
}

//******************************************************************************
