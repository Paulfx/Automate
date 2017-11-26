//******************************************************************************

#include <iostream>

#include "Determinize.h"
#include "FA_tools.h"
#include "Accept.h"

////////////////////////////////////////////////////////////////////////////////

int findIndex(const std::vector<etatset_t>& vec, const etatset_t& e) {
	const auto it=std::find(vec.begin(), vec.end(), e);
	if(it==vec.end()) return -1;
	return it-vec.begin(); //Indice de e
}

////////////////////////////////////////////////////////////////////////////////

bool isFinal(const sAutoNDE& at, const etatset_t& e) {
	for(auto etat:e) {
		if(at.finaux.find(etat)!=at.finaux.end()) return true;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////

sAutoNDE Determinize(const sAutoNDE& at){
  	sAutoNDE r;
  	std::vector<etatset_t> vecEtatSet;//Contiendra les états de l'automate déterminisé
	int indexEtatArrivee;
	etatset_t etatSetDepart; //L'état de départ local
	etatset_t etatSetTmp;	 //Le prochain état de départ associé à un symbole

	//Modification des attributs de r
	r.nb_symbs=at.nb_symbs;
	r.nb_finaux=0; //Initialement
	r.initial=0;

	etatSetDepart={at.initial};
  	Fermeture(at,etatSetDepart);//L'état départ est la fermeture de l'état initial
	vecEtatSet.push_back(etatSetDepart);//état initial à l'indice 0
	
	unsigned int indexEtatDepart=0;

	//Si indexEtatDepart < vec.size(), alors on doit traiter de nouveaux états
	//Si indexEtatDepart=vecEtatSet.size(), alors on a pas trouvé de nouvel état
	while(indexEtatDepart<vecEtatSet.size()) {
		etatSetDepart=vecEtatSet[indexEtatDepart];
	
		//On augmente la taille de la table de transition au fur et à mesure des nouveaux états
		r.trans.resize(indexEtatDepart+1);
		r.trans[indexEtatDepart].resize(r.nb_symbs);

		//Si l'ensemble d'état actuel contient un état final, alors ce sera un état final
		if(isFinal(at,etatSetDepart)) {
			r.finaux.insert(indexEtatDepart);
  			r.nb_finaux++;
		}

		//Calcul des états suivants possibles en partant des états de etatSetDepart
		for (unsigned int ch=0; ch<r.nb_symbs; ++ch) {
  			etatSetTmp.clear();
  			etatSetTmp=Delta(at,etatSetDepart,ch);//Calcul des états suivant possible en lisant ch
  			Fermeture(at,etatSetTmp);			  //Puis calcul de sa fermeture

  			//On recherche si l'ensemble d'états d'arrivée est un état de l'automate déterminisé
  			//Si l'état d'arrivée n'est pas connu, on l'ajoute à nos états connus.
  			indexEtatArrivee=findIndex(vecEtatSet,etatSetTmp);
  			if(indexEtatArrivee==-1) {
  				indexEtatArrivee=vecEtatSet.size();
  				vecEtatSet.push_back(etatSetTmp);
  			}

  			//On ajoute le lien dans l'automate determinisé
  			r.trans[indexEtatDepart][ch].insert(indexEtatArrivee);
  		}

  		//On avance
  		indexEtatDepart++;
	}
	r.nb_etats=vecEtatSet.size();
  	return r;
}

//******************************************************************************
	