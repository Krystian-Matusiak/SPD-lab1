#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <chrono>


using namespace std;



/*  Pierwsza wersja algorytmu - niewystarczający
void RPQ( int n , int*R, int*P, int*Q, int*X){
	for (int i = 0; i < n; ++i)
		X[i]=i;
	for( int i = 0; i < n; i++ )
		for(int j=0; j<n-1; j++){
			if( R[X[j]] > R[X[j+1]] )
				swap( R[X[j]], R[X[j+1]]);
		}
}*/


/* Druga wersja algorytmu - niewystarczający
void A2( int n , int*R, int*P, int*Q, int*X){

	for (int i = 0; i < n; ++i)
		X[i]=i;

	float w1,w2;

	for (int i = 0; i < n; ++i){
		for(int j=0 ; j<n-1 ; j++){
			if( Q[X[j]]!=0 && Q[X[j+1]]!=0 ){
				w1 = (float)(R[X[j]])   / (float)(Q[X[j]])   ;
				w2 = (float)(R[X[j+1]]) / (float)(Q[X[j+1]]) ;				
				if( w1>w2 )
					swap(X[j] , X[j+1]);
			}
			else{
				if( Q[X[j]]!=0 )
					for(int k=j ; k<n ; k++)
						if( Q[X[k]] < Q[X[k+1]]  )
							swap( Q[X[k]] , Q[X[k+1]] );
				if( Q[X[j+1]]!=0 )
					for(int k=j ; k<n ; k++)
						if( Q[X[k+1]] > Q[X[k+1]]  )
							swap( Q[X[k+1]] , Q[X[k+1]] );
			}
		}
	}
}*/

int cmax( int n , int*R, int*P, int*Q, int*X){
	int t=0,c=0;

	for (int i = 0; i < n; ++i){
		t=max(t,R[X[i]]) + P[X[i]];
		c=max(c,t + Q[X[i]]);
	}
	return c;
}

/*
// Trzecia wersja algorytmu - wyniki zadowalające
void Algorytm( int n , int*R, int*P, int*Q, int*X){
	int ref;
	for (int i = 0; i < n; ++i)
		X[i]=i;
	ref=cmax(n,R,P,Q,X);

		for( int i = 0; i<n; i++ )
			for(int j=0; j<n-1; j++){
				swap( X[n-j-1], X[n-j-2]);
				if( cmax(n,R,P,Q,X) > ref )
					swap( X[n-j-1], X[n-j-2]);
				else
					ref=cmax(n,R,P,Q,X);
			}			
}*/

// Czwarta wersja algorytmu - wyniki bardzo zadowalające
void Algorytm2( int n , int*R, int*P, int*Q, int*X){
	int ref;
	for (int i = 0; i < n; ++i)
		X[i]=i;
	ref=cmax(n,R,P,Q,X);

	for(int h=0 ; h<5 ; h++){
		for( int i = 0; i<n; i++ )
			for(int j=0; j<n; j++){
				swap( X[i], X[j]);
				if( cmax(n,R,P,Q,X) > ref )
					swap( X[i], X[j]);
				else
					ref=cmax(n,R,P,Q,X);
			}			
	}
}



int main(int argc, char const *argv[])
{
	srand(time(NULL));

	int n, R[100] , P[100] , Q[100], X[100];
	ifstream plik("dat.txt");
	string s;
	string DATA[]= {"data.1" , "data.2" , "data.3" , "data.4" };
	int wyniki[4];
	int czasy[4];
	int suma_wynikow=0;
	int suma_czasow=0;


	for(int c=0 ; c<4 ; c++){
		while( s!=DATA[c] )
			plik >> s;
		
		plik >> n;
		for (int i = 0; i < n; ++i)
			plik>>R[i]>>P[i]>>Q[i];
		
		//for (int i = 0; i < n; ++i)
		//	cout<<i+1<<") "<<R[i]<<" "<<P[i]<<" "<<Q[i]<<" "<<endl;

		//////////////////
		// Pomiar
		auto q1 = chrono::high_resolution_clock::now() ;

		Algorytm2(n,R,P,Q,X);

		auto q2 = chrono::high_resolution_clock::now() ;
		auto timeq = chrono::duration_cast<std::chrono::microseconds>( q2-q1 ).count();
		czasy[c] = timeq;
		//////////////////

		//for (int i = 0; i < n; ++i)
		//	cout<<i+1<<") "<<R[i]<<" "<<P[i]<<" "<<Q[i]<<" "<<endl;

		cout<<"\nkolejnosc:\n";
		for (int i = 0; i < n; ++i)
			cout<< X[i]+1 << " ";
		cout<<endl<<endl;
		wyniki[c]=cmax(n,R,P,Q,X);
	}
	
	for (int i = 0; i < 4; ++i){
		cout<<"Wynik "<<i<<": "<<wyniki[i]<<endl;
		cout<<"Czas: "<<czasy[i]/1000<<"ms\n";		
		suma_wynikow+=wyniki[i];
		suma_czasow+=czasy[i]/1000;
	}
	cout<<"Suma wynikow: "<<suma_wynikow<<endl;
	cout<<"Suma czasow: "<<suma_czasow<<"ms\n";

	plik.close();
	cin.get();
	return 0;
}

