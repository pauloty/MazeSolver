#include <iostream>
#include <stack>
#include <queue>
#include <cmath>
#include <iomanip>
#include "algoritmosDeBusca.hpp"

using namespace std;

typedef struct noh_t{	//nós da fila de prioridade da bestfirst e da A*
	vector<Posicao_t> caminho;	//caminho até a posicao atual
	double peso;	//soma dos pesos do caminho ate o no (+ valor da heuristica na A*)
	double pesoCaminho; //soma dos pesos do caminho ate o no

	public:
	//metodo de comparacao entre nós para a fila de prioridade
	bool operator() (const struct noh_t &no1, const struct noh_t &no2) const{
		if(no1.peso > no2.peso)
			return true;
		return false;
	}
}Noh_t;

//imprime uma matriz
void imprime_mat(char** mat, int NLIN, int NCOL){
	for(int i=0;i<NLIN;i++){
		for(int j=0;j<NCOL;j++){
			cout<<mat[i][j]<<" ";
		}
		cout<<endl;
	}
}

//verifica se duas posicoes no labirinto sao iguais
bool mesma_posicao(Posicao_t pos1, Posicao_t pos2){
	if(pos1.linha == pos2.linha and pos1.coluna == pos2.coluna)
		return true;
	return false;
}


/* funcao para inserir os filhos de um no na pilha da busca em profundidade
	no : o vetor do caminho ateh a posicao atual
	dfs : a pilha do algoritmo
	visitados : a copia da matriz original com as posicoes ja visitadas demarcadas
*/
void insere_filhos_prof(vector<Posicao_t> &no, stack<vector<Posicao_t>> &dfs, char** visitados, int NLIN, int NCOL){
	Posicao_t pos = no[no.size()-1];
	Posicao_t filho;
	vector<Posicao_t> caminho_filho;

	/*
		procura andar em todas as direcoes possiveis, adicionando
		as posicoes validas encontradas como nohs na pilha junto com
		o caminho da origem ate as mesmas
	*/
	for(int i=-1;i<2;i++){
		for(int j=-1;j<2;j++){

			filho.linha = pos.linha+i;
			filho.coluna = pos.coluna+j;

			//verifica se a posicao do filho eh valida
			if(filho.linha >= 0 and filho.linha < NLIN and filho.coluna >= 0 and filho.coluna < NCOL){
				if(visitados[filho.linha][filho.coluna]!='-' and visitados[filho.linha][filho.coluna]!='v'){
					//salva caminho ate o filho como o caminho ate o no pai + posicao do no filho
					caminho_filho = no;
					caminho_filho.push_back(filho);

					//insere no filho na pilha e marca como visitado
					dfs.push(caminho_filho);
					visitados[filho.linha][filho.coluna] = 'v';
				}
			}
		}
	}
}

/* funcao da busca em profundidade
	lab : struct contendo a matriz do labirinto, suas dimensoes e posicoes de origem e destino
*/
vector<Posicao_t> busca_prof(Labirinto_t *lab, int &numCasasVisitadas){
	int NLIN = lab->nlin, NCOL = lab->ncol;

	//aloca matriz auxiliar para marcar posicoes visitadas
	char** visitados = (char**)malloc(NLIN * sizeof(char*));
	for(int i=0;i<NLIN;i++){
		visitados[i] = (char*)malloc(NCOL * sizeof(char));
	}

	for(int i=0;i<NLIN;i++){
		for(int j=0;j<NCOL;j++){
			visitados[i][j] = lab->mat[i][j];
		}
	}

	//pilha do algoritmo
	stack<vector<Posicao_t>> dfs;

	Posicao_t origem = lab->origem;
	Posicao_t destino = lab->destino;
	visitados[origem.linha][origem.coluna] = 'v';

	vector<Posicao_t> cam;
	cam.push_back(origem);

	dfs.push(cam);
	vector<Posicao_t> no = dfs.top();
	Posicao_t pos = origem;

	/* loop principal
		enquanto nao encontrar o destino e houver elementos na pilha,
			- remove noh atual
			- insere filhos do noh atual nao visitados na pilha
			- atualiza noh atual
	*/
	while(mesma_posicao(pos, destino)==false and dfs.empty()==false){
		dfs.pop();
		numCasasVisitadas++;
		insere_filhos_prof(no, dfs, visitados, NLIN, NCOL);
		if(dfs.empty()==false){
			no = dfs.top();
			pos = no[no.size()-1];
		}
	}

	for(int i=0;i<NLIN;i++){
		free(visitados[i]);
	}
	free(visitados);

	cam.clear();
	if(dfs.empty()==false){
		cam = dfs.top();
	}

	return cam;
}

/* funcao para inserir os filhos de um no na fila da busca em largura
	no : o vetor do caminho ateh a posicao atual
	bfs : a fila do algoritmo
	visitados : a copia da matriz original com as posicoes ja visitadas demarcadas
*/
void insere_filhos_larg(vector<Posicao_t> &no, queue<vector<Posicao_t>> &bfs, char** visitados, int NLIN, int NCOL){
	Posicao_t pos = no[no.size()-1];
	Posicao_t filho;
	vector<Posicao_t> caminho_filho;

	/*
		procura andar em todas as direcoes possiveis, adicionando
		as posicoes validas encontradas como nohs na fila junto com
		o caminho da origem ate as mesmas
	*/
	for(int i=-1;i<2;i++){
		for(int j=-1;j<2;j++){

			filho.linha = pos.linha+i;
			filho.coluna = pos.coluna+j;

			//verifica se a posicao do filho eh valida
			if(filho.linha >= 0 and filho.linha < NLIN and filho.coluna >= 0 and filho.coluna < NCOL){
				if(visitados[filho.linha][filho.coluna]!='-' and visitados[filho.linha][filho.coluna]!='v'){
					//salva caminho ate o filho como o caminho ate o no pai + posicao do no filho
					caminho_filho = no;
					caminho_filho.push_back(filho);

					//insere no filho na fila e marca como visitado
					bfs.push(caminho_filho);
					visitados[filho.linha][filho.coluna] = 'v';
				}
			}
		}
	}
}

/* funcao da busca em largura
	lab : struct contendo a matriz do labirinto, suas dimensoes e posicoes de origem e destino
*/
vector<Posicao_t> busca_larg(Labirinto_t *lab, int &numCasasVisitadas){
	int NLIN = lab->nlin, NCOL = lab->ncol;

	//aloca matriz auxiliar para marcar posicoes visitadas
	char** visitados = (char**)malloc(NLIN * sizeof(char*));
	for(int i=0;i<NLIN;i++){
		visitados[i] = (char*)malloc(NCOL * sizeof(char));
	}

	for(int i=0;i<NLIN;i++){
		for(int j=0;j<NCOL;j++){
			visitados[i][j] = lab->mat[i][j];
		}
	}
	
	//fila do algoritmo
	queue<vector<Posicao_t>> bfs;

	Posicao_t origem = lab->origem;
	Posicao_t destino = lab->destino;
	visitados[origem.linha][origem.coluna] = 'v';

	vector<Posicao_t> cam;
	cam.push_back(origem);

	bfs.push(cam);
	vector<Posicao_t> no = bfs.front();
	Posicao_t pos = origem;


	/* loop principal
		enquanto nao encontrar o destino e houver elementos na fila,
			- remove noh atual
			- insere filhos do noh atual nao visitados na fila
			- atualiza noh atual
	*/
	while(mesma_posicao(pos, destino)==false and bfs.empty()==false){
		bfs.pop();
		numCasasVisitadas++;
		insere_filhos_larg(no, bfs, visitados, NLIN, NCOL);
		if(bfs.empty()==false){
			no = bfs.front();
			pos = no[no.size()-1];
		}
	}

	for(int i=0;i<NLIN;i++){
		free(visitados[i]);
	}
	free(visitados);

	cam.clear();
	if(bfs.empty()==false){
		cam = bfs.front();
	}

	return cam;
}

//funcao que calcula a previsao otimista do peso ateh o destino
//calcula a distancia entre duas posicoes (posicao atual e destino)
double previsao_Aestr(Posicao_t pos1, Posicao_t pos2){
	double dx, dy;
	dy = abs(pos2.linha - pos1.linha);
	dx = abs(pos2.coluna - pos1.coluna);
	double maior, menor;
	if(dx > dy){
		maior = dx;
		menor = dy;
	}
	else{
		maior = dy;
		menor = dx;
	}	
	return (menor * sqrt(2)) + (maior-menor);
}

/* funcao para inserir os filhos de um no na fila de prioridade da busca best-first
	no : struct com o vetor do caminho ateh a posicao atual e o peso do passo ate a posicao
	bestf : a fila de prioridade do algoritmo
	visitados : a copia da matriz original com as posicoes ja visitadas demarcadas
*/
void insere_filhos_bestf(Noh_t &no, priority_queue<Noh_t, vector<Noh_t>, Noh_t> &bestf, char** visitados, int NLIN, int NCOL, Posicao_t destino){
	Posicao_t pos = no.caminho[no.caminho.size()-1];
	Posicao_t filho;
	Noh_t no_filho;

	/*
		procura andar em todas as direcoes possiveis, adicionando
		as posicoes validas encontradas como nohs na fila com
			- caminho ate a posicao
			- peso do passo ate o noh
	*/
	for(int i=-1;i<2;i++){
		for(int j=-1;j<2;j++){

			filho.linha = pos.linha+i;
			filho.coluna = pos.coluna+j;

			if(filho.linha >= 0 and filho.linha < NLIN and filho.coluna >= 0 and filho.coluna < NCOL){
				if(visitados[filho.linha][filho.coluna]!='-' and visitados[filho.linha][filho.coluna]!='v'){
					//salva caminho ate o filho como o caminho ate o no pai + posicao do no filho
					no_filho.caminho = no.caminho;
					no_filho.caminho.push_back(filho);

					//peso do no filho eh igual a estimativa do custo até o destino
					no_filho.peso = previsao_Aestr(filho, destino);

					//insere no filho na fila e marca como visitado
					bestf.push(no_filho);
					visitados[filho.linha][filho.coluna] = 'v';
				}
			}
		}
	}
}

/* funcao da busca best-first
	lab : struct contendo a matriz do labirinto, suas dimensoes e posicoes de origem e destino
*/
vector<Posicao_t> busca_bestf(Labirinto_t *lab, int &numCasasVisitadas){
	int NLIN = lab->nlin, NCOL = lab->ncol;

	//aloca matriz auxiliar para marcar posicoes visitadas
	char** visitados = (char**)malloc(NLIN * sizeof(char*));
	for(int i=0;i<NLIN;i++){
		visitados[i] = (char*)malloc(NCOL * sizeof(char));
	}

	for(int i=0;i<NLIN;i++){
		for(int j=0;j<NCOL;j++){
			visitados[i][j] = lab->mat[i][j];
		}
	}
	
	//fila de prioridade do algoritmo
	priority_queue<Noh_t, vector<Noh_t>, Noh_t> bestf;

	Posicao_t origem = lab->origem;
	Posicao_t destino = lab->destino;
	visitados[origem.linha][origem.coluna] = 'v';

	Noh_t no;
	no.caminho.push_back(origem);
	no.peso=0;
	bestf.push(no);

	Posicao_t pos = origem;

	/* loop principal
		enquanto nao encontrar o destino e houver elementos na fila,
			- remove noh atual
			- insere filhos do noh atual nao visitados na fila
			- atualiza noh atual
	*/
	while(mesma_posicao(pos, destino)==false and bestf.empty()==false){
		bestf.pop();
		numCasasVisitadas++;
		insere_filhos_bestf(no, bestf, visitados, NLIN, NCOL, destino);
		if(bestf.empty()==false){
			no = bestf.top();
			pos = no.caminho[no.caminho.size()-1];
		}
	}

	for(int i=0;i<NLIN;i++){
		free(visitados[i]);
	}
	free(visitados);

	vector<Posicao_t> cam;
	if(bestf.empty()==false){
		cam = bestf.top().caminho;
	}

	return cam;
}

/* funcao para inserir os filhos de um no na fila de prioridade da busca A-estrela
	no : struct com o vetor do caminho ateh a posicao atual, soma dos pesos do caminho e o peso final (peso do caminho somado com a funcao heuristica)
	bestf : a fila de prioridade do algoritmo
	visitados : a copia da matriz original com as posicoes ja visitadas demarcadas
*/
void insere_filhos_Aestr(Noh_t &no, priority_queue<Noh_t, vector<Noh_t>, Noh_t> &Aestr, char** visitados, int NLIN, int NCOL, Posicao_t destino){
	Posicao_t pos = no.caminho[no.caminho.size()-1];
	Posicao_t filho;
	Noh_t no_filho;

	for(int i=-1;i<2;i++){
		for(int j=-1;j<2;j++){

			filho.linha = pos.linha+i;
			filho.coluna = pos.coluna+j;

			if(filho.linha >= 0 and filho.linha < NLIN and filho.coluna >= 0 and filho.coluna < NCOL){
				if(visitados[filho.linha][filho.coluna]!='-' and visitados[filho.linha][filho.coluna]!='v'){
					//salva caminho ate o filho como o caminho ate o no pai + posicao do no filho
					no_filho.caminho = no.caminho;
					no_filho.caminho.push_back(filho);

					//calcula soma dos pesos do caminho ateh o noh filho
					no_filho.pesoCaminho = no.pesoCaminho + sqrt(i*i+j*j);

					//peso final = soma dos pesos do caminho + valor da funcao heuristica
					no_filho.peso = no_filho.pesoCaminho + previsao_Aestr(filho, destino);
					Aestr.push(no_filho);
					visitados[filho.linha][filho.coluna] = 'v';
				}
			}
		}
	}
}

/* funcao da busca A-estrela
	lab : struct contendo a matriz do labirinto, suas dimensoes e posicoes de origem e destino
*/
vector<Posicao_t> busca_Aestr(Labirinto_t *lab, int &numCasasVisitadas){
	int NLIN = lab->nlin, NCOL = lab->ncol;

	//aloca matriz auxiliar para marcar posicoes visitadas
	char** visitados = (char**)malloc(NLIN * sizeof(char*));
	for(int i=0;i<NLIN;i++){
		visitados[i] = (char*)malloc(NCOL * sizeof(char));
	}

	for(int i=0;i<NLIN;i++){
		for(int j=0;j<NCOL;j++){
			visitados[i][j] = lab->mat[i][j];
		}
	}
	
	//fila de prioridade do algoritmo
	priority_queue<Noh_t, vector<Noh_t>, Noh_t> Aestr;

	Posicao_t origem = lab->origem;
	Posicao_t destino = lab->destino;
	visitados[origem.linha][origem.coluna] = 'v';

	Noh_t no;
	no.caminho.push_back(origem);
	no.peso=0;
	no.pesoCaminho=0;
	Aestr.push(no);

	Posicao_t pos = origem;

	/* loop principal
		enquanto nao encontrar o destino e houver elementos na fila,
			- remove noh atual
			- insere filhos do noh atual nao visitados na fila
			- atualiza noh atual
	*/
	while(mesma_posicao(pos, destino)==false and Aestr.empty()==false){
		Aestr.pop();
		numCasasVisitadas++;
		insere_filhos_Aestr(no, Aestr, visitados, NLIN, NCOL, destino);
		if(Aestr.empty()==false){
			no = Aestr.top();
			pos = no.caminho[no.caminho.size()-1];
		}
	}

	for(int i=0;i<NLIN;i++){
		free(visitados[i]);
	}
	free(visitados);

	vector<Posicao_t> cam;
	if(Aestr.empty()==false){
		cam = Aestr.top().caminho;
	}

	return cam;
}