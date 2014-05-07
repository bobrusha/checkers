#include "game.h"

int x_prev, y_prev;
int enemy;
//------------------------

void startGame( const bool is_white, vector < vector < int > >& ch, int& kolvo_wh, int& kolvo_bk ){
	kolvo_wh = 12;
	kolvo_bk = 12;

	
	if ( is_white ) 
	{
		enemy = 2;
		for (int i = 0; i < 3; ++i ) 
		{
			for (int j = 0; j < 8;  ++j){
				cout<< i%2<<" "<<j%2<<endl;
				if ( (i%2!=0 && j%2 !=0) || (( i%2 == 0 ) && ( j%2 == 0 ))) {
					ch[i][j] = 1;
				}
				else {
					ch[i][j] = 0;
				}
			}
		}

		for (int i = 3; i < 5; ++i){
			for (int j = 0; j < 8;  ++j){
				ch[i][j] = 0;
			}
		}

		for (int i = 5; i < 8; ++i){
			for (int j = 0; j < 8;  ++j){
				if ((i%2 && j%2 || !(i%2) && !(j%2)))
					ch[i][j] = 2;
				else ch[i][j] = 0;
			}
		}
	}
	print(ch);
	x_prev = -1;
	y_prev = -1;

	return;
}
bool chooseChecker ( const int x, const int y0, vector <vector <int> > & ch){
	int y = 7 - y0;
	if ( ch[y][x] == 0 ){
		cout<<" in if ! (ch) "<<endl;
		return false;
	}

	if ( (ch[y][x]) == enemy ){			
		cout<<"in if ch == 2"<<endl;
		return false;
	}
	x_prev = x;
	y_prev =  y;	
	cout<<"everytng ok"<<endl;	
	return true;
}

bool makeStep ( const int x, const int y, vector <vector <int> >& ch ){
		if (x < x_prev){
			cout<<"because of x< x_prev"<<endl; 
			return false;
		}

		if ( (ch[x][y]) == 2 ){
			cout<<"I did'nt make a step because of black!"<<x<<" "<< y<<endl;
			return false;
		}
		ch[x][y] = 1;


		ch[x][y] = 0;

		cout<<"I maked a step!"<< x <<" "<< y << endl; 

		x_prev = -1;
		y_prev = -1;		

	return true;
}

void print ( vector<vector <int> > & ch){
	for (int i=0; i<8; ++i){
		for (int j=0; j<8; ++j)
			cout<<ch[i][j]<< " ";
		cout<<endl;
	}
}
