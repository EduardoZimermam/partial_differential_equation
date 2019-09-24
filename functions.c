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


/**
 * @brief Alocação dinâmica de um Vetor.
 * @param tamVetor  Tamanho do vetor a ser alocado.
 * @return Retorna um vetor alocado.
 */
double* alocaVetor(int tamVetor){

	double *vetorSaida = malloc(tamVetor * sizeof(double));
	for (int i = 0; i < tamVetor; ++i){
		vetorSaida[i] = 0;
	}

 return(vetorSaida);
}


double limiteSuperior(double x, double PI){
	//u(x,PI)= sin (2 * PI * x) * sinh(PI*PI);
	double seno = sin(2 * PI * x) * (sinh(PI*PI));
	//printf("Valor de seno Superior de %.2f = %.2f \n",x,seno);

	return (seno);
}


double limiteInferior(double x, double PI){
	//u(x,0)= sin(2*PI(PI-x))* sinh(PI*PI);
	double seno = sin(2 * PI * (PI-x))* (sinh(PI*PI));
	//printf("Valor de seno Inferior de %.2f = %.2f \n",x,seno);
	return (seno);
}


/**
 * @brief Cálculo da função f(x,y).
 * @param x  
 * @param y  
 * @return Retorna o valor da função.
 */
void calculaFuncao(double x, double y, double PI){

	double aux1 = sin(2 * PI * x) * (sinh(PI*y));
	double aux = aux1 + (sin(2 * PI * (PI-x))* (sinh(PI * (PI - y))));
	double result = (4 * (PI*PI)) * aux;

	printf("Valor da Função é = %.2f \n",result);
}
