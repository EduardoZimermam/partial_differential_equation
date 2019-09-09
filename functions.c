/**
 * @author  Annelyse Schatzmann           GRR20151731
 * @author  Eduardo Zimermam Pereira      GRR20152952
 */

/**
 * @file functions.c
 * @brief Todas as funções utilizadas nos métodos.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "functions.h"

/**
 * @brief Lê a linha de comando de forma dinâmica.
 * @param argc    Número de argumentos da linha de comando.
 * @param argv    Vetor de argumentos da linha de comando.
 * @param nx      Número de pontos a serem calculados na dimensão X.
 * @param ny      Número de pontos a serem calculados na dimensão Y.
 * @param itr  	  Número de iterações a serem executadas.
 * @param caminhoSaida  Caminho da solução.
 * @return Retorna os valores passados por entrada padrão.
 *
 * Considera todas as condições de entrada das variáveis.
 */
int getParametros (int argc, char **argv, int *nx, int *ny, int *itr, char *caminhoSaida){
 
 	printf("ENTROU\n");

	for(int i = 1; i < argc; i ++){
		if (strcmp(argv[i], "-nx") == 0){
			*nx = atoi(argv[i+1]);
		}
		else{
			if (strcmp(argv[i], "-ny") == 0 ){
				*ny = atoi(argv [i+1]);
			}
			else{
				if (strcmp(argv[i], "-i") == 0){
					*itr = atoi(argv[i+1]);
				}			
				else{
					if (strcmp(argv[i], "-o") == 0 ){
						strcpy(caminhoSaida, argv [i+1]);
					}
				}
			}
		}
	}



	if (*nx < 1){
		fprintf (stderr,"ERRO: falta o parâmetro obrigatório -nx (número de pontos a serem calculados em X)\n");
	 	return (-1);
	}
	else{

		if (*ny < 1){
			fprintf (stderr,"ERRO: falta o parâmetro obrigatório -ny (número de pontos a serem calculados em Y)\n");
		 	return (-1);
		}
		else{
			if (*itr < 1){
				fprintf (stderr,"ERRO: falta o parâmetro obrigatório -i (número de iterações a serem executadas)\n");
	 			return (-1);
	 		}
		}
	}
}