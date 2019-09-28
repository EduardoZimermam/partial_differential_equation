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
#include "functions.h"

int main(int argc, char **argv){

	int nx, ny, itr, param;
	char caminhoSaida[200];
	sL sistemaLinear;

	param = getParametros (argc, argv, &nx, &ny, &itr, caminhoSaida);

	if (param == -1){

		fprintf(stderr, "\n\n ERRO NOS PARÂMETROS!!!\n\n");
		return(-1);
	}
	else {
		
		double hx, hy, n;
		sL sistemaLinear;

		hx = M_PI / nx;
		hy = M_PI / ny;
		n = 4 * (M_PI * M_PI);

		calculaEquacaoDiferencialParcial(hx, hy, n, nx, ny, &sistemaLinear);

		return(0);
	}
}
