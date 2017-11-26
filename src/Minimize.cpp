//******************************************************************************

#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

#include "Minimize.h"

////////////////////////////////////////////////////////////////////////////////

sAutoNDE Minimize(const sAutoNDE& at){
  //TODO définir cette fonction

  sAutoNDE r;

  r.nb_etats=at.nb_etats+2;//2 états ajoutés : l'état départ et l'état final
  r.nb_symbs=at.nb_symbs;
  r.nb_finaux=at.nb_finaux;
  r.finaux={r.nb_etats};//Un seul état final
  r.initial=0;

  r.trans.resize(at.nb_etats+1);
  r.trans[0].resize(at.nb_symbs);

	for(unsigned int i=0;i<at.nb_etats;++i) {
		r.trans[i+1].resize(at.nb_symbs);
		for(unsigned int ch=0;ch<at.nb_symbs;++ch) {
  			for(const auto& it:at.trans[i][ch]) r.trans[i+1][ch].insert(it+1);
  		}
	}

  r.epsilon.resize(at.epsilon.size()+1+at.nb_finaux);
  r.epsilon[0].insert(at.initial+1);
  for(unsigned int i=0;i<at.epsilon.size();++i) {
  	for(auto it:at.epsilon[i]) r.epsilon[i+1].insert(it+1);
  }
  for(const auto& it:at.finaux) r.epsilon[it+1].insert(r.nb_etats);


  return r;
}

//******************************************************************************
