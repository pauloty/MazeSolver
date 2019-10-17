#include <fstream>
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <string>

#define min(x, y) x < y ? x : y

using namespace std;

/* 
	Gera n labirintos como uma matriz, marcando onde eh passagem (*), parede (-),
	a posicao inicial (#) e a posicao de destino ($)

	Cada labirinto k eh salvo em labirintos/labirinto<k>.txt (labirinto0.txt, labirinto1.txt, ...)
*/

//calcula distancia em linha reta entre duas posicoes
double dist(int x1, int y1, int x2, int y2){
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int main(int argc, char** argv)
{
	srand(time(NULL));

	int NLABS = 1, NLIN, NCOL, MINDIST;
	int MAX_TAM_PAREDES = 10, MIN_TAM_PAREDES = 8;

	if(argc < 4){
		cout << "Como usar: ./gerador <numero_de_labirintos> <numero_de_linhas> <numero_de_colunas>" << endl;
		return 0;
	}

	NLABS = atoi(argv[1]);
	NLIN = atoi(argv[2]);
	NCOL = atoi(argv[3]);

	//distancia minima entre origem e destino
	MINDIST = (min(NLIN/2, NCOL/2)) - 2;

	//aloca matriz do labirinto
	char** mat = (char**)malloc(NLIN * sizeof(char*));
	for(int i=0;i<NLIN;i++){
		mat[i] = (char*)malloc(NCOL * sizeof(char));
	}

	//gera os labirintos aleatorios e salva em seus respectivos arquivos
	for(int k=0; k<NLABS; k++){		
		for(int i=0;i<NLIN;i++){
			for(int j=0;j<NCOL;j++){
				mat[i][j] = '*';
			}
		}

		int vertical, lado=0, tam=0, x, y;
		int np = (min(NLIN, NCOL)) *0.7;
		//vertical: define direcao da paredes (vertical ou horizontal)
		//lado: define sentido em que a parede cresce
		//tam: define tamanho da parede
		//x e y: posicao inicial da parede
		//np: numero de paredes

		MAX_TAM_PAREDES = (min(NLIN, NCOL)) * 0.3;
		MIN_TAM_PAREDES = (min(NLIN, NCOL)) * 0.1;

		/*
			define aleatoriamente posicao e tamanho das paredes
		*/
		for(int i=0;i<np;i++){
			x = rand()%NCOL;
			y = rand()%NLIN;

			tam = (rand()%(MAX_TAM_PAREDES-MIN_TAM_PAREDES))+MIN_TAM_PAREDES;

			//direcao e sentido da paredes
			vertical = rand()%2;
			lado = ((float)(rand()%2)-0.5)*2;	//lado = -1 ou +1

			for(int j=0;j<tam;j++){
				if(x>=0 and x<NCOL and y>=0 and y<NLIN){
					mat[y][x] = '-';
				}
				if(vertical){	//se for vertical, muda o y
					y+=lado;
				}
				else{
					x+=lado;	//se nao, muda o x
				}
			}
		}

		//define posicoes de origem e destino
		int xo, yo, xd, yd;
		do{
			xo = rand()%NCOL;
			yo = rand()%NLIN;
		}while(mat[yo][xo]=='-');
		mat[yo][xo] = '#';

		do{
			xd = rand()%NCOL;
			yd = rand()%NLIN;
		}while(mat[yd][xd]=='-' or mat[yd][xd]=='#' or dist(xo, yo, xd, yd) < MINDIST);
		mat[yd][xd] = '$';

		// Salva as dimensões e o proprio labirinto no arquivo apropriado
		ofstream arquivo;
		string nome("labirintos/labirinto" + to_string(k) + ".txt");
		arquivo.open(nome, ios::trunc);

		arquivo << NLIN << " " << NCOL << endl;

		for(int i=0; i<NLIN; i++){
			for(int j=0; j<NCOL; j++){
				arquivo << mat[i][j];
			}
			arquivo << endl;
		}

		arquivo.close();

		cout << "Labirinto gerado" << endl;
	}


	return 1;
}