/**
 *  @author Annelyse Schatzmann           GRR20151731
 *  @author Eduardo Zimermam Pereira      GRR20152952
 */

/**
 * @file functions.h
 * @brief Todas as funções utilizadas no método.
 */

#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#define PULO 7

typedef struct sL {
  double *sistemaLinearCompleto;
} sL;

typedef struct ponto {
	double *x , *y;
} ponto;

int getParametros (int argc, char **argv, int *nx, int *ny, int *itr, char *caminhoSaida);
sL* calculaEquacaoDiferencialParcial(double hx, double hy, double n, double nx, double ny, ponto *pontosSL);
double* alocaVetor(int tamVetor);
double limiteSuperior(double x);
double limiteInferior(double x);
double calculaFuncao(double x, double y);
double normaL2Residuo(sL *SL, int nx, int ny);
int gaussSeidel (sL *SL, int nx, int ny, int itr, double *tempoItr, double *normaL2Itr);
void printResultado(double *tempoItr, double *normaL2Itr, int itrConverge, int tam, ponto *pontosSL, char *caminhoSaida, sL *SL);

#endif
