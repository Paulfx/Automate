//******************************************************************************

#include "ToGraph.h"

////////////////////////////////////////////////////////////////////////////////

bool ToGraph(sAutoNDE& at, std::string path){
	std::ofstream f(path);
	if (!f.is_open()) return false;
	f<<"digraph finite_state_machine {\n\trankdir=LR;\n\tsize=\"10,10\";\n\n\t";

	//Etats finaux
	f<<"node [shape = doublecircle];";
	for(const auto& etatFinal:at.finaux) f<<" "<<etatFinal;
	f<<" ;\n\tnode [shape = point ]; q;\n\tnode [shape = circle];\n\n\tq -> "<<at.initial<<";\n\t";
	
	//Transitions
	const char caractereIndice[5]={'a','b','c','d','e'};
	char lettre;
	for(unsigned int etatDepart=0;etatDepart<at.trans.size();++etatDepart) {
		for(unsigned int iChar=0;iChar<at.trans[etatDepart].size();++iChar) {
			lettre=caractereIndice[iChar];
			for(const auto& etatArrivee:at.trans[etatDepart][iChar]) f<<etatDepart<<" -> "<<etatArrivee<<" [label = \""<<lettre<<"\"];\n\t";		
		}
	}

	//ε-transitions
	for(unsigned int etatDepart=0;etatDepart<at.epsilon.size();++etatDepart) {
		for(auto etatArrivee:at.epsilon[etatDepart]) f<<etatDepart<<" -> "<<etatArrivee<<" [label = \"ε\"];\n\t";
	}
	
	f<<"\n}";
	f.close();

  return true;
}

//******************************************************************************

