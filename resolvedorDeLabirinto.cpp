#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <ctime>
#include <vector>
#include <iomanip>
#include "algoritmosDeBusca.hpp"

using namespace std;

/*
	Se nenhum argumento for passado, labirintos devem ser gerados pela opcao 5

	Caso contrario, eh possivel passar o numero de labirintos, numero de linhas e numero de colunas (nesta ordem)
	para resolver labirintos gerados anteriormente

	Os labirintos gerados sao salvos na pasta "labirintos" como "labirinto<numero_do_labirinto>.txt"

	O algoritmo a ser executado pode ser selecionado digitando o numero correspondente no menu

	A solucao dada pelo algoritmo para cada labirinto eh salva na pasta "caminhos/<nome_do_algoritmo>/caminho<numero_do_labirinto>.txt"

	Exemplo: para o algoritmo de busca em largura executado no labirinto 67, salvo em labirintos/labirinto67.txt, o caminho eh salvo em
	caminhos/busca_largura/caminho67.txt

	Para o algoritmo selecionado, após executar em todos os labirintos, sao impressos na saida padrao:
		- A media e o desvio padrao do tempo de execucao
		- A media e o desvio padrao do numero de casas do caminho resultado
		- A media e o desvio padrao do numero de casas visitadas
		- e a media e o desvio padrao da soma dos pesos do caminho
*/

// Struct para armazenar as metricas usadas para comparacao entre algoritmos
typedef struct n{
	double mediaTempoExec;
	double mediaNumCasas;
	double mediaNumCasasVisitadas;
	double mediaPesos;
	double DpTempoExec;
	double DpNumCasas;
	double DpNumCasasVisitadas	;
	double DpPesos;
}Metricas;

// Soma dos pesos de um caminho retornado por um algoritmo
double calculaSomaPesos(vector<Posicao_t> caminho){
	int dx, dy;
	double soma = 0;
	for(int i=0; i<(int)caminho.size()-1; i++){
		dx = caminho[i].linha - caminho[i+1].linha;
		dy = caminho[i].coluna - caminho[i+1].coluna;
		soma += sqrt(dx*dx + dy*dy);
	}
	return soma;
}

// Calcula media de um vetor
template<class T>
double media(vector<T> vet){
	double r=0;
	for(int i=0; i<(int)vet.size(); i++){
		r += vet[i];
	}
	return r/(double)vet.size();
}

// Calcula variancia de um vetor, dada a media
template<class T>
double variancia(vector<T> vet, double media){
	double v=0;
	int size = (int)vet.size();
	for(int i=0; i<size; i++){
		v += pow(vet[i] - media, 2);
	}
	return v/(size - 1);
}

// Calcula o desvio padrao, dada a variancia
double desvioPadrao(double variancia){
	return sqrt(variancia);
}

int main(int argc, char **argv){
	int opcaoSelecionada, nlin, ncol, nlabs=0;
	vector<Metricas> algMetricas(4);
	ifstream arquivoLabirinto;
	ofstream arquivoCaminho;

	// Recupera o numero de labirintos, linhas e colunas passados como argumentos de entrada
	if(argc == 4){
		nlabs = atoi(argv[1]);
		nlin = atoi(argv[2]);
		ncol = atoi(argv[3]);
	}

	// Matriz dos labirintos
	char **mat = NULL, **matCaminho = NULL;
	mat = (char**)malloc(nlin * sizeof(char*));
	for(int i=0; i<nlin; i++){
		mat[i] = (char*)malloc(ncol * sizeof(char));
	}

	// Matriz auxiliar usada pra imprimir o labirinto com o caminho demarcado
	matCaminho = (char**)malloc(nlin * sizeof(char*));
	for(int i=0; i<nlin; i++){
		matCaminho[i] = (char*)malloc(ncol * sizeof(char));
	}
	Labirinto_t *lab = (Labirinto_t *)malloc(sizeof(Labirinto_t));

	// Loop do menu
	while(1) {
		cout << "Selecione uma opcao:" << endl;
		cout << "1 - Busca em Largura" << endl;
		cout << "2 - Busca em Profundidade" << endl;
		cout << "3 - Busca Best-First" << endl;
		cout << "4 - Busca A*" << endl;
		cout << "5 - Gerar Labirintos" << endl;
		cout << "6 - Sair" << endl;

		cin >> opcaoSelecionada;

		while(opcaoSelecionada < 1 or opcaoSelecionada > 6){
			cout << "Opcao invalida!" << endl;
			cin >> opcaoSelecionada;
		}

		if(opcaoSelecionada == 6)
			break;

		if(opcaoSelecionada == 5){
			// Realoca matrizes do labirinto e gera novos labirintos
			if(mat != NULL){
				for(int i=0; i<nlin; i++){
					free(mat[i]);
				}
				free(mat);
			}

			if(matCaminho != NULL){
				for(int i=0; i<nlin; i++){
					free(matCaminho[i]);
				}
				free(matCaminho);
			}

			cout << "Numero de labirintos: ";
			cin >> nlabs;
			cout << "Numero de linhas: ";
			cin >> nlin;
			cout << "Numero de colunas: ";
			cin >> ncol;

			// Executa programa gerador de labirintos
			string command("./gerador " + to_string(nlabs) + " " + to_string(nlin) + " " + to_string(ncol));
			if(system(command.c_str()))
				cout << "Labirintos gerados" << endl;
			else {
				cout << "Erro na geracao de labirintos" << endl;
				break;
			}					

			mat = (char**)malloc(nlin * sizeof(char*));
			for(int i=0; i<nlin; i++){
				mat[i] = (char*)malloc(ncol * sizeof(char));
			}

			matCaminho = (char**)malloc(nlin * sizeof(char*));
			for(int i=0; i<nlin; i++){
				matCaminho[i] = (char*)malloc(ncol * sizeof(char));
			}

			continue;
		}

		if(nlabs == 0){
			cout << "Nao ha labirintos a serem resolvidos!" << endl;
			break;
		}

		// Vetores para armazenar as metricas resultantes do algoritmo executado em cada labirinto gerado
		vector<double> tempoExec;
		vector<int> numCasas;
		vector<int> numCasasVisitadas;
		vector<double> sumPesos;


		for(int k=0; k<nlabs; k++){
			double nclocks;	// variavel para calcular tempo de execucao do algoritmo
			
			// Recupera labirinto dos arquivos
			string nome("labirintos/labirinto" + to_string(k) + ".txt");
			arquivoLabirinto.open(nome);

			arquivoLabirinto >> nlin >> ncol;

			for(int i=0; i<nlin; i++){
				for(int j=0; j<ncol; j++){
					arquivoLabirinto >> mat[i][j];
					matCaminho[i][j] = mat[i][j];
				}
			}

			arquivoLabirinto.close();

			// Armazena labirinto na struct
			lab->nlin = nlin;
			lab->ncol = ncol;
			lab->mat = mat;

			// Encontra origem e destino
			for(int i=0;i<nlin;i++){
				for(int j=0;j<ncol;j++){
					if(mat[i][j]=='#'){
						lab->origem.linha = i;
						lab->origem.coluna = j;
					}
					if(mat[i][j]=='$'){
						lab->destino.linha = i;
						lab->destino.coluna = j;
					}
				}
			}

			// Encontra um caminho entre a origem e o destino e calcula tempo decorrido
			vector<Posicao_t> caminho;
			int casasVisitadas=0;
			nome = "caminhos/";	// Nome e diretorio do arquivo onde sera armazenado o caminho
			nclocks = clock();
			if(opcaoSelecionada == 1){
				caminho = busca_larg(lab, casasVisitadas);
				nome += "busca_largura/";
			} else if(opcaoSelecionada == 2){
				caminho = busca_prof(lab, casasVisitadas);
				nome += "busca_profundidade/";
			} else if(opcaoSelecionada == 3){
				caminho = busca_bestf(lab, casasVisitadas);
				nome += "busca_best_first/";
			} else if(opcaoSelecionada == 4){
				caminho = busca_Aestr(lab, casasVisitadas);
				nome += "busca_Aestrela/";
			}
			nclocks = clock() - nclocks;

			// Para o labirinto atual, caso haja solucao, salva as metricas calculadas
			if(!caminho.empty()){
				tempoExec.push_back(nclocks/CLOCKS_PER_SEC);
				numCasas.push_back((int)caminho.size());
				numCasasVisitadas.push_back(casasVisitadas);
				sumPesos.push_back(calculaSomaPesos(caminho));				
			}

			cout << "Caminho gerado" << endl;

			// Escreve as posicoes do caminho na matriz auxiliar
			for(int i=0;i<(int)caminho.size();i++){
				if(matCaminho[caminho[i].linha][caminho[i].coluna] != '#' and matCaminho[caminho[i].linha][caminho[i].coluna] != '$')
					matCaminho[caminho[i].linha][caminho[i].coluna] = 'c';
			}
			
			imprime_mat(matCaminho, nlin, ncol);

			// Restaura a matriz original na matriz auxiliar
			for(int i=0;i<(int)caminho.size();i++){
				if(matCaminho[caminho[i].linha][caminho[i].coluna] != '#' and matCaminho[caminho[i].linha][caminho[i].coluna] != '$')
					matCaminho[caminho[i].linha][caminho[i].coluna] = mat[caminho[i].linha][caminho[i].coluna];
			}

			// Salva o caminho no arquivo no formato [(x0,y0),(x1,y1),(x2,y2),...]
			nome += "caminho" + to_string(k) + ".txt";
			arquivoCaminho.open(nome, ios::trunc);

			arquivoCaminho << '[';
			cout << "[";
			for(int i=0; i<(int)caminho.size(); i++){
				arquivoCaminho << '(' << caminho[i].linha << ',' << caminho[i].coluna << ')';
				cout << "(" << caminho[i].linha << "," << caminho[i].coluna << ")";
				if(i < (int)caminho.size()-1){
					arquivoCaminho << ',';
					cout << ",";
				}
			}
			arquivoCaminho << ']' << endl;
			cout << "]" << endl<<endl;

			arquivoCaminho.close();

		}

		// Calcula, salva e imprime na saida padrao as metricas do algoritmo selecionado no vetor
		algMetricas[opcaoSelecionada-1].mediaTempoExec = media<double>(tempoExec);
		algMetricas[opcaoSelecionada-1].mediaNumCasas = media<int>(numCasas);
		algMetricas[opcaoSelecionada-1].mediaNumCasasVisitadas = media<int>(numCasasVisitadas);
		algMetricas[opcaoSelecionada-1].mediaPesos = media<double>(sumPesos);

		double v = variancia(tempoExec, algMetricas[opcaoSelecionada-1].mediaTempoExec);
		algMetricas[opcaoSelecionada-1].DpTempoExec = desvioPadrao(v);

		v = variancia(numCasas, algMetricas[opcaoSelecionada-1].mediaNumCasas);
		algMetricas[opcaoSelecionada-1].DpNumCasas = desvioPadrao(v);

		v = variancia(numCasasVisitadas, algMetricas[opcaoSelecionada-1].mediaNumCasasVisitadas);
		algMetricas[opcaoSelecionada-1].DpNumCasasVisitadas = desvioPadrao(v);

		v = variancia(sumPesos, algMetricas[opcaoSelecionada-1].mediaPesos);
		algMetricas[opcaoSelecionada-1].DpPesos = desvioPadrao(v);

		if(opcaoSelecionada == 1){
			cout << "Busca em Largura";
		} else if(opcaoSelecionada == 2){
			cout << "Busca em Profundidade";
		} else if(opcaoSelecionada == 3){
			cout << "Busca Best-First";
		} else if(opcaoSelecionada == 4){
			cout << "Busca A*";
		}
		cout << endl;
		cout << nlabs << " labirintos " << nlin << " por " << ncol << endl;
		cout << tempoExec.size() << " labirintos com solucao" << endl;

		cout << "Media de Tempo = " << setprecision(5) << fixed << algMetricas[opcaoSelecionada-1].mediaTempoExec << endl;
		cout << "Desvio Padrao do Tempo = " << setprecision(5) << fixed << algMetricas[opcaoSelecionada-1].DpTempoExec << endl;
		cout << "Media de Casas do Caminho = " << setprecision(5) << fixed << algMetricas[opcaoSelecionada-1].mediaNumCasas << endl;
		cout << "Desvio Padrao de Casas do Caminho = " << setprecision(5) << fixed << algMetricas[opcaoSelecionada-1].DpNumCasas << endl;
		cout << "Media de Casas Visitadas = " << setprecision(5) << fixed << algMetricas[opcaoSelecionada-1].mediaNumCasasVisitadas << endl;
		cout << "Desvio Padrao de Casas Visitadas = " << setprecision(5) << fixed << algMetricas[opcaoSelecionada-1].DpNumCasasVisitadas << endl;
		cout << "Media de Soma dos Pesos = " << setprecision(5) << fixed << algMetricas[opcaoSelecionada-1].mediaPesos << endl;
		cout << "Desvio Padrao da Soma dos Pesos = " << setprecision(5) << fixed << algMetricas[opcaoSelecionada-1].DpPesos << endl;
	}

	for(int i=0; i<nlin; i++){
		free(mat[i]);
	}
	free(mat);
	for(int i=0; i<nlin; i++){
		free(matCaminho[i]);
	}
	free(matCaminho);
	free(lab);

	return 0;
}