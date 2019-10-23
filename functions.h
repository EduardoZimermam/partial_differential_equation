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

typedef struct sL {
  double *superiorAfastada, *superior, *principal, *inferior, *inferiorAfastada;
  double *b;
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
double normaL2Residuo(sL *SL, double *x, int nx, int ny);
int gaussSeidel (sL *SL, double *x, int nx, int ny, double erro, int itr, double *tempoItr, double *normaL2Itr);
void printResultado(double *tempoItr, double *normaL2Itr, int itrConverge, int tam, ponto *pontosSL, char *caminhoSaida, double *x);

#endif
