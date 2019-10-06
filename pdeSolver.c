/**
 * @author  Annelyse Schatzmann           GRR20151731
 * @author  Eduardo Zimermam Pereira      GRR20152952
 */

/**
 * @file pdeSolver.c
 * @brief Programa principal.
 * Chamada das funções.
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "functions.h"

int main(int argc, char **argv){

	int nx, ny, itr, param, itrConverge;
	char *caminhoSaida;
	sL sistemaLinear;

	caminhoSaida = malloc(200 * sizeof(char));

	param = getParametros (argc, argv, &nx, &ny, &itr, caminhoSaida);

	if (param == -1){

		fprintf(stderr, "\n\n ERRO NOS PARÂMETROS!!!\n\n");
		return(-1);
	}
	else {
		
		double  *x, *tempoItr, *normaL2Itr, erro, hx, hy, n;
		sL *sistemaLinear;

		x = alocaVetor(nx * ny);
		
		hx = M_PI / nx;
		hy = M_PI / ny;
		n = 4 * (M_PI * M_PI);

		sistemaLinear = calculaEquacaoDiferencialParcial(hx, hy, n, nx, ny);

		erro = 1.0e-4;

		normaL2Itr = alocaVetor(itr);
		tempoItr = alocaVetor(itr);

		itrConverge = gaussSeidel(sistemaLinear, x, nx, ny, erro, itr, tempoItr, normaL2Itr);

		if (itrConverge > 0){

			printResultado(tempoItr, normaL2Itr, itrConverge, caminhoSaida);

			return 0;
		} else {
			fprintf(stderr, "ERRO: O Método de Gauss Seidel não convergiu na quantidade de iterações específicada. PROGRAMA ABORTADA A EXECUÇÃO!\n");
			return -1;
		}
	}
}
