//******************************************************************************

#include <iostream>
#include <algorithm>

#include "Determinize.h"
#include "ToGraph.h"

#include "Accept.h"
#include "FA_tools.h"

////////////////////////////////////////////////////////////////////////////////

bool EstDeterministe(const sAutoNDE& at){
	for(unsigned int i=0;i<at.nb_etats;++i) {
		for(unsigned int j=0;j<at.nb_symbs;++j) {
			if( at.trans[i][j].size() > 1 ) return false;
			if( at.epsilon[i].size() > 0 ) return false;
		}
	}
  	return true;
}

////////////////////////////////////////////////////////////////////////////////

void Fermeture(const sAutoNDE& at, etatset_t& e){
	etatset_t r;

	for (const auto& etatDepart:e){ //On parcourt les états de départ 
		for(const auto& itEpsilon:at.epsilon[etatDepart]) { //Parcours des ε-transitions
			r.insert(itEpsilon); //ajout sans duplication
		}
		//On fusionne les sets
		for(const auto& it:r) {
			e.insert(it);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

etatset_t Delta(const sAutoNDE& at, const etatset_t& e, symb_t c){
  etatset_t r;

  for(const auto& etatDepart : e) { //On parcourt les états de départs
		for(const auto& itTmp : at.trans[etatDepart][c]) { //On parcourt les états accessibles
			r.insert(itTmp); //On les rajoute sans duplication
		}
  	}
  return r;
}

////////////////////////////////////////////////////////////////////////////////

bool Accept(const sAutoNDE& at, std::string str){
	etatset_t etatDepartTmp; //Contiendra nos états de départs après chaque caractère lu
	
	etatDepartTmp.insert(at.initial);
	Fermeture(at,etatDepartTmp); //L'état initial est la fermeture de l'état de départ

	//On parcourt le mot
	for(unsigned int i=0;i<str.length();++i) {
		symb_t ch=str[i]-ASCII_A;
		std::cout<<ch<<std::endl;
		etatDepartTmp=Delta(at,etatDepartTmp,ch);//On calcule les états suivants après avoir lu ch
		Fermeture(at,etatDepartTmp);			//On y ajoute les états accessibles sans rien lire
	}

	//On regarde si un de nos états finaux possibles est un état final
	for(const auto& etatFinal:at.finaux) {
		if (etatDepartTmp.find(etatFinal) != etatDepartTmp.end()) return true;
	}
	return false;
}

//******************************************************************************
