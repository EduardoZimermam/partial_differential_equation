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
#include <likwid.h>
#include "functions.h"

int main(int argc, char **argv){
	LIKWID_MARKER_INIT;

	int nx, ny, itr, param, itrConverge;
	char *caminhoSaida;

	caminhoSaida = malloc(200 * sizeof(char));

	param = getParametros (argc, argv, &nx, &ny, &itr, caminhoSaida);

	if (param == -1){

		fprintf(stderr, "\n\n ERRO NOS PARÂMETROS!!!\n\n");
		return(-1);
	}
	else {
		
		double  *x, *tempoItr, *normaL2Itr, hx, hy, n;
		sL *sistemaLinear;
		ponto *pontosSL;

		x = alocaVetor(nx * ny);
		pontosSL = malloc(sizeof(ponto));
		pontosSL->x = alocaVetor(nx * ny);
		pontosSL->y = alocaVetor(nx * ny);
		
		hx = M_PI / (nx + 1);
		hy = M_PI / (ny + 1);
		n = 4 * (M_PI * M_PI);

		sistemaLinear = calculaEquacaoDiferencialParcial(hx, hy, n, nx, ny, pontosSL);

		normaL2Itr = alocaVetor(itr);
		tempoItr = alocaVetor(itr);

		itrConverge = gaussSeidel(sistemaLinear, x, nx, ny, itr, tempoItr, normaL2Itr);
		
		printResultado(tempoItr, normaL2Itr, itrConverge, (nx * ny), pontosSL, caminhoSaida, x);
	}

	LIKWID_MARKER_CLOSE;
}
