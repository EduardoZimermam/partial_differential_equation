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


int getParametros (int argc, char **argv, int *nx, int *ny, int *itr, char *caminhoSaida);
double* alocaVetor(int tamVetor);
double limiteSuperior(double x, double PI);
double limiteInferior(double x, double PI);
void calculaFuncao(double x, double y, double PI);

#endif