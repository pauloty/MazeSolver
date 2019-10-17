all:
	g++ algoritmosDeBusca.cpp resolvedorDeLabirinto.cpp -o resolvedor -Wall -Wextra
	g++ geradorDeLabirinto.cpp -o gerador -Wall -Wextra
	mkdir -p labirintos caminhos/busca_largura caminhos/busca_profundidade caminhos/busca_best_first caminhos/busca_Aestrela
run:
	./resolvedor