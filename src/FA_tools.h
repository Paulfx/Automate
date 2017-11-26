//******************************************************************************
#ifndef FA_TOOLS_H
#define FA_TOOLS_H
//******************************************************************************

#include <iostream>
#include <cassert>
#include <queue>
#include <list>
#include <fstream>
#include <sstream>

#include "FA_types.h"

////////////////////////////////////////////////////////////////////////////////

// je rajoute ces petites fonctions génériques pour l'affichage.
template<class InputIterator> std::ostream& show_container(std::ostream& out, InputIterator first, InputIterator last, char beg='{', char end='}', char sep=',');

template<class T>
std::ostream& operator<<(std::ostream& out, const std::set<T>& s) {
  return show_container(out, s.begin(), s.end());
}

template<class T1, class T2>
std::ostream& operator<<(std::ostream& out, const std::map<T1,T2>& m) {
  return show_container(out, m.begin(), m.end(), '{', '}', '\n');
}

template<class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& s) {
  return show_container(out, s.begin(), s.end(), '[', ']', ',');
}

template<class T1, class T2> std::ostream& operator<<(std::ostream& out, const std::pair<T1,T2>& p){
  out << '<' << p.first << ',' << p.second << '>';
  return out;
}

template<class InputIterator> std::ostream& show_container(std::ostream& out, InputIterator first, InputIterator last, char beg, char end, char sep){
  out << beg;
  if(first!=last)
    out << *first++;  
  for(; first != last ; ++first)
     out << sep << (*first);
  out << end;
  return out;
}
////////////////////////////////////////////////////////////////////////////////

bool FromFile(sAutoNDE& at, std::string path);

////////////////////////////////////////////////////////////////////////////////

std::ostream& operator<<(std::ostream& out, const sAutoNDE& at);

//******************************************************************************
#endif // FA_TOOLS_H
//******************************************************************************
