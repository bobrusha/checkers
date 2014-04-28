#include "game.h"


void startGame(bool is_white, vector < vector < int > > :: iterator it, int& kolvo_wh, int& kolvo_bk){
	kolvo_wh = 12;
	kolvo_bk = 12;
	

	if ( is_white ) 
	{
		for (int i=0; i < 3; ++i ) 
		{
			for (int j = 0; j < 8;  ++j){
				if (!( (i%2 && j%2) || !(i%2) && !(j%2) )) {
					(*it)[j] = 1;
				}
				else {
					(*it)[j] = 0;
				}
				cout<<(*it)[j]<<endl;
			}
			++it;
		}

		for (int i = 0; i < 2; ++i){
			for (int j = 0; j < 8;  ++j){
				(*it)[j] = 0;
			}
			++it;
		}

		for (int i = 5; i < 8; ++i){
			for (int j = 0; j < 8;  ++j){
				if (!(i%2 && j%2 || !(i%2) && !(j%2)))
					(*it)[j] = 2;
				else (*it)[j] = 0;
			}
			++it;
		}
	}


	return;
}