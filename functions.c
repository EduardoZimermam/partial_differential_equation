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
#include <math.h>

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


double limiteSuperior(double x){
	//u(x,PI)= sin (2 * PI * x) * sinh(PI*PI);
	double seno = sin(2 * M_PI * x) * (sinh(M_PI*M_PI));
	//printf("Valor de seno Superior de %.2f = %.2f \n",x,seno);

	return (seno);
}


double limiteInferior(double x){
	//u(x,0)= sin(2*PI(PI-x))* sinh(PI*PI);
	double seno = sin(2 * M_PI * (M_PI-x))* (sinh(M_PI*M_PI));
	//printf("Valor de seno Inferior de %.2f = %.2f \n",x,seno);
	return (seno);
}

double calculaEquacaoDiferencialParcial(double hx, double hy, double n, double nx, double ny){
  double esquerda, direita, cima, baixo, central, bordaSuperior, bordaInferior;

  esquerda = -2*(hy * hy) - hx * (hy * hy);
  direita = -2*(hy * hy) + hx * (hy * hy);
  cima = -2*(hx * hx) + hy * (hx * hx);
  baixo = -2*(hx * hx) - hy * (hx * hx);
  central = 4*(hy * hy) + 4*(hx * hx) + 2*(hx * hx) * (hy * hy) * n;

  // printf("esquerda: %lf, direita: %lf, cima: %lf, baixo: %lf, central: %lf\n", esquerda, direita, cima, baixo, central);

  // sistemaLinear->superiorAfastada = alocaVetor(nx * ny);
  // sistemaLinear->superior = alocaVetor(nx * ny);
  // sistemaLinear->principal = alocaVetor(nx * ny);
  // sistemaLinear->inferior = alocaVetor(nx * ny);
  // sistemaLinear->inferiorAfastada = alocaVetor(nx * ny);
  // sistemaLinear->b = alocaVetor(nx * ny);


  bordaSuperior = limiteSuperior(0.0 + hx);
  printf("Superior/Esquerda: %.15lf + %.15lf + %.15lf = %.15lf \n", direita, central, baixo, (calculaFuncao(hx, (M_PI - hy)) - (0.0 * esquerda) - (bordaSuperior * cima)));

  bordaSuperior = limiteSuperior(M_PI - hx);
  printf("Superior/Direita: %.15lf + %.15lf + %.15lf = %.15lf \n", esquerda, central, baixo, (calculaFuncao((M_PI - hx), (M_PI - hy)) - (0.0 * direita) - (bordaSuperior * cima)));

  bordaInferior = limiteInferior(0.0 + hx);
  printf("Inferior/Esquerda: %.15lf + %.15lf + %.15lf = %.15lf \n", direita, central, cima, (calculaFuncao(hx, hy) - (0.0 * esquerda) - (bordaInferior * baixo)));

  bordaInferior = limiteInferior(M_PI - hx);
  printf("Inferior/Direita: %.15lf + %.15lf + %.15lf = %.15lf \n", esquerda, central, cima, (calculaFuncao((M_PI - hx), (0.0 + hy)) - (0.0 * direita) - (bordaInferior * baixo)));

  printf("\n\n");

  // Rosa 2
  for (int i = 2; i <= nx - 1; i++) {
    bordaSuperior = limiteSuperior(i * hx);
    printf("Para i=%d: %.15lf + %.15lf + %.15lf + %.15lf = %.15lf \n", i, direita, esquerda, central, baixo, (calculaFuncao((i * hx), (M_PI - hy)) - (bordaSuperior * cima)));
  }

  printf("\n\n");

  // Verde
  for (int i = 2; i <= nx - 1; i++) {
    bordaInferior = limiteInferior(i);
    printf("Para i=%d: %.15lf + %.15lf + %.15lf + %.15lf = %.15lf \n", i, direita, esquerda, central, cima, (calculaFuncao((i * hx), (0.0 + hy)) - (bordaInferior * baixo)));
  }

  printf("\n\n");

  // Azul
  for (int i = 2; i <= ny - 1; i++) {
    printf("Para i=%d: %.15lf + %.15lf + %.15lf + %.15lf = %.15lf \n", i, direita, central, cima, baixo, (calculaFuncao((i * hy), (0.0 + hx)) - (0.0 * esquerda)));
  }

  printf("\n\n");

  // Rosa
  for (int i = 2; i <= ny - 1; i++) {
    printf("Para i=%d: %.15lf + %.15lf + %.15lf + %.15lf = %.15lf \n", i, esquerda, central, cima, baixo, (calculaFuncao((i * hy), (M_PI - hx)) - (0.0 * direita)));
  }

  printf("\n\n");

  // Miolo
  for (int i = 2; i <= nx - 1; i++) {
    for (int j = 2; j <= ny - 1; j++) {
          printf("Para i=%d e j=%d: %.15lf + %.15lf + %.15lf + %.15lf + %.15lf = %.15lf \n", i, j, esquerda, direita, central, cima, baixo, calculaFuncao((i * hx), (j * hy)));
    }
  }

}

/**
 * @brief Cálculo da função f(x,y).
 * @param x
 * @param y
 * @return Retorna o valor da função.
 */
double calculaFuncao(double x, double y){

	double aux1 = sin(2 * M_PI * x) * (sinh(M_PI*y));
	double aux = aux1 + (sin(2 * M_PI * (M_PI-x))* (sinh(M_PI * (M_PI - y))));
	double result = (4 * (M_PI*M_PI)) * aux;

  return result;
}
