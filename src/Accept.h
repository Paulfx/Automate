//******************************************************************************
#ifndef ACCEPT_H
#define ACCEPT_H
//******************************************************************************

#include "FA_types.h"

////////////////////////////////////////////////////////////////////////////////
/**
 *  Retourne true si l'automate at est déterministe, false sinon.
**/
bool EstDeterministe(const sAutoNDE& at);

////////////////////////////////////////////////////////////////////////////////
/**
 *	Calcule la fermeture de l'automate at et l'ajoute dans l'ensemble d'états e. 
**/
void Fermeture(const sAutoNDE& at, etatset_t& e);

////////////////////////////////////////////////////////////////////////////////
/**
 *  Retourne un ensemble d'états composé des états suivants possible à partir
 *  des étas de l'automate at, en lisant le symbole c.
**/
etatset_t Delta(const sAutoNDE& at, const etatset_t& e, symb_t c);

////////////////////////////////////////////////////////////////////////////////
/**
 *  Retourne true si str est accepté par l'automate at, false sinon.
**/
bool Accept(const sAutoNDE& at, std::string str);

//******************************************************************************
#endif // ACCEPT_H
//******************************************************************************
