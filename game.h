#ifndef _GAME_H_
#define  _GAME_H_

#include <iostream>
#include <vector>

using namespace std;

void startGame ( const bool, vector < vector <int> >& , int&, int& );

bool chooseChecker (const int , const int, vector <vector <int> > & );
bool makeStep ( const int , const int, vector < vector <int> > & );

void print ( vector < vector <int> >& );

//void gameIsOver(HWND, vector < vector <int> > :: iterator, int&, int&);

#endif