#include <vector>
using namespace std;

typedef struct Posicao_t{
	int linha, coluna;
}Posicao_t;

typedef struct l{
	char **mat;
	Posicao_t origem, destino;
	int nlin, ncol;
}Labirinto_t;

void imprime_mat(char**, int, int);
vector<Posicao_t> busca_prof(Labirinto_t *, int &);
vector<Posicao_t> busca_larg(Labirinto_t *, int &);
vector<Posicao_t> busca_bestf(Labirinto_t *, int &);
vector<Posicao_t> busca_Aestr(Labirinto_t *, int &);
