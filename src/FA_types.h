//******************************************************************************
#ifndef FA_TYPES_H
#define FA_TYPES_H
//******************************************************************************

#include <vector>
#include <set>
#include <map>

////////////////////////////////////////////////////////////////////////////////

const unsigned int ASCII_A = 97;
const unsigned int ASCII_Z = ASCII_A + 26;
const bool         DEBUG = false;

typedef size_t                              etat_t;
typedef unsigned char                       symb_t;
typedef std::set<etat_t>                    etatset_t;
typedef std::vector<std::vector<etatset_t>> trans_t;
typedef std::vector<etatset_t>              epsilon_t;
typedef std::map<etatset_t, etat_t>         map_t;

////////////////////////////////////////////////////////////////////////////////

struct sAutoNDE{
  // caractéristiques
  size_t nb_etats;
  size_t nb_symbs;
  size_t nb_finaux;

  etat_t initial;
  // état initial

  etatset_t finaux;
  // états finaux : finaux_t peut être un int*, un tableau dynamique comme vector<int>
  // ou une autre structure de donnée de votre choix.

  trans_t trans;
  // matrice de transition : trans_t peut être un int***, une structure dynamique 3D comme vector< vector< set<int> > >
  // ou une autre structure de donnée de votre choix.
  
  epsilon_t epsilon; 
  // transitions spontanées : epsilon_t peut être un int**, une structure dynamique 2D comme vector< set<int> >
  // ou une autre structure de donnée de votre choix.
};

//******************************************************************************
#endif // FA_TYPES_H
//******************************************************************************
