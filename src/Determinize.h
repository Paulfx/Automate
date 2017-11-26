//******************************************************************************
#ifndef DETERMINIZE_H
#define DETERMINIZE_H
//******************************************************************************

#include "FA_types.h"
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
/**
 * Retourne s'il existe l'indice de l'étatset e dans le vector vec, -1 sinon.
**/
int findIndex(const std::vector<etatset_t>& vec, const etatset_t& e);

////////////////////////////////////////////////////////////////////////////////
/**
 * Retourne true si l'ensemble d'états e contient au moins un état final de
 * l'automate at, false sinon.
**/
bool isFinal(const sAutoNDE& at, const etatset_t& e);

////////////////////////////////////////////////////////////////////////////////
/**
 * Determinise un automate at.
**/
sAutoNDE Determinize(const sAutoNDE& at);

//******************************************************************************
#endif // DETERMINIZE_H
//******************************************************************************
