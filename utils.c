/**
 * @author  Annelyse Schatzmann           GRR20151731
 * @author  Eduardo Zimermam Pereira      GRR20152952
 */

/**
 * @file utils.c
 * @brief Função para calcular o tempo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "utils.h"

/**
 * @brief Calcula o tempo em milisegundos.
 *
 * @return Retorna o cálculo do tempo em milisegundos.
 */
double timestamp(void){
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return((double)(tp.tv_sec*1000.0 + tp.tv_usec/1000.0));
}


