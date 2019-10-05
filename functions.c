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
int getParametros(int argc, char **argv, int *nx, int *ny, int *itr, char *caminhoSaida) {
	*nx = 0;
	*ny = 0;
	*itr = 0;

	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-nx") == 0) {
			*nx = atoi(argv[i + 1]);
		}
		else {
			if (strcmp(argv[i], "-ny") == 0) {
				*ny = atoi(argv[i + 1]);
			}
			else {
				if (strcmp(argv[i], "-i") == 0) {
					*itr = atoi(argv[i + 1]);
				}
				else {
					if (strcmp(argv[i], "-o") == 0) {
						strcpy(caminhoSaida, argv[i + 1]);
					}
				}
			}
		}
	}
	
	if (*nx < 3) {
		fprintf(stderr, "ERRO: falta o parâmetro obrigatório -nx ou número informado menor que 2 (número de pontos a serem calculados em X)\n");
		return (-1);
	}

	if (*ny < 3) {
		fprintf(stderr, "ERRO: falta o parâmetro obrigatório -ny ou número informado menor que 2(número de pontos a serem calculados em Y)\n");
		return (-1);
	}

	if (*itr < 1)	{
		fprintf(stderr, "ERRO: falta o parâmetro obrigatório -i (número de iterações a serem executadas)\n");
		return (-1);
	}
}



/**
 * @brief Alocação dinâmica de um Vetor.
 * @param tamVetor  Tamanho do vetor a ser alocado.
 * @return Retorna um vetor alocado.
 */
double *alocaVetor(int tamVetor) {

	double *vetorSaida = malloc(tamVetor * sizeof(double));
	for (int i = 0; i < tamVetor; ++i) {
		vetorSaida[i] = 0;
	}

	return (vetorSaida);
}



/**
 * @brief Cálculo da Borda Superior.
 * @param x Ponto na dimensão x a ser considerado no cálculo.
 * @return Retorna o valor calculado para a borda superior.
 */
double limiteSuperior(double x) {
	
	double seno = sin(2 * M_PI * x) * (sinh(M_PI * M_PI));
	return (seno);
}

/**
 * @brief Cálculo da da Borda Inferior.
 * @param x Ponto na dimensão x a ser considerado no cálculo.
 * @return @return Retorna o valor calculado para a borda inferior.
 */
double limiteInferior(double x) {
	
	double seno = sin(2 * M_PI * (M_PI - x)) * (sinh(M_PI * M_PI));
	return (seno);
}

/**
 * @brief Cálculo da função f(x,y).
 * @param x Ponto na dimensão x a ser considerado no cálculo.
 * @param y Ponto na dimensão y a ser considerado no cálculo.
 * @return Retorna o valor da função.
 */
double calculaFuncao(double x, double y) {

	double aux1 = sin(2 * M_PI * x) * (sinh(M_PI * y));
	double aux = aux1 + (sin(2 * M_PI * (M_PI - x)) * (sinh(M_PI * (M_PI - y))));
	double result = (4 * (M_PI * M_PI)) * aux;

	return (result);
}


/**
 * @brief Cálculo da discretização da malha.
 * @param hx Tamanho entre um ponto e outro na dimensão X.
 * @param hy Tamanho entre um ponto e outro na dimensão Y.
 * @param n Parte do cálculo para a solução de discretização da malha.
 * @param nx Número de pontos a serem calculados na dimensão X.
 * @param ny Número de pontos a serem calculados na dimensão Y.
 * @param sistenaLinear Struct do sistema linear a ser montado.
 * @return Preenche os vetores com os valores dos pontos da malha.
 */
sL* calculaEquacaoDiferencialParcial(double hx, double hy, double n, double nx, double ny) {
	double esquerda, direita, cima, baixo, central, bordaSuperior, bordaInferior;
	int idx = 0;
	sL *sistemaLinear;

	sistemaLinear = malloc(sizeof(sL));

	/*Cálculo dos Coeficientes Uij*/
	esquerda = -2 * (hy * hy) - hx * (hy * hy);
	direita = -2 * (hy * hy) + hx * (hy * hy);
	cima = -2 * (hx * hx) + hy * (hx * hx);
	baixo = -2 * (hx * hx) - hy * (hx * hx);
	central = 4 * (hy * hy) + 4 * (hx * hx) + 2 * (hx * hx) * (hy * hy) * n;

	/*Alocação dos vetores das diagonais*/
	sistemaLinear->superiorAfastada = alocaVetor(nx * ny);
	sistemaLinear->superior = alocaVetor(nx * ny);
	sistemaLinear->principal = alocaVetor(nx * ny);
	sistemaLinear->inferior = alocaVetor(nx * ny);
	sistemaLinear->inferiorAfastada = alocaVetor(nx * ny);
	sistemaLinear->b = alocaVetor(nx * ny);

	/*Cálculo do Ponto Inferior Esquerdo*/
	bordaInferior = limiteInferior(0.0 + hx);
	sistemaLinear->superiorAfastada[idx] = cima;
	sistemaLinear->superior[idx] = direita;
	sistemaLinear->principal[idx] = central;
	sistemaLinear->inferior[idx] = 0.0;
	sistemaLinear->inferiorAfastada[idx] = 0.0;
	sistemaLinear->b[idx] = calculaFuncao(hx, hy) - (0.0 * esquerda) - (bordaInferior * baixo);
	idx++;

	/*Cálculo dos pontos da Borda Inferior*/
	for (int i = 2; i <= nx - 1; i++)	{
		bordaInferior = limiteInferior(i);
		sistemaLinear->superiorAfastada[idx] = cima;
		sistemaLinear->superior[idx] = direita;
		sistemaLinear->principal[idx] = central;
		sistemaLinear->inferior[idx] = esquerda;
		sistemaLinear->inferiorAfastada[idx] = 0.0;
		sistemaLinear->b[idx] = calculaFuncao((i * hx), (0.0 + hy)) - (bordaInferior * baixo);
		idx++;
	}

	/*Cálculo do Ponto Inferior Direito*/
	bordaInferior = limiteInferior(M_PI - hx);
	sistemaLinear->superiorAfastada[idx] = cima;
	sistemaLinear->superior[idx] = 0.0;
	sistemaLinear->principal[idx] = central;
	sistemaLinear->inferior[idx] = esquerda;
	sistemaLinear->inferiorAfastada[idx] = 0.0;
	sistemaLinear->b[idx] = calculaFuncao((M_PI - hx), (0.0 + hy)) - (0.0 * direita) - (bordaInferior * baixo);
	idx++;

	/*Cálculo dos Pontos internos da malha*/
	for (int i = 2; i <= nx - 1; i++)	{
		
		/* Cálculo do ponto da borda da esquerda */
		sistemaLinear->superiorAfastada[idx] = cima;
		sistemaLinear->superior[idx] = direita;
		sistemaLinear->principal[idx] = central;
		sistemaLinear->inferior[idx] = 0.0;
		sistemaLinear->inferiorAfastada[idx] = baixo;
		sistemaLinear->b[idx] = calculaFuncao((i * hy), (0.0 + hx)) - (0.0 * esquerda);
		idx++;

		for (int j = 2; j <= ny - 1; j++) {
			sistemaLinear->superiorAfastada[idx] = cima;
			sistemaLinear->superior[idx] = direita;
			sistemaLinear->principal[idx] = central;
			sistemaLinear->inferior[idx] = esquerda;
			sistemaLinear->inferiorAfastada[idx] = baixo;
			sistemaLinear->b[idx] = calculaFuncao((i * hx), (j * hy));
			idx++;
		}

		/* Cálculo do ponto da borda da direita */
		sistemaLinear->superiorAfastada[idx] = cima;
		sistemaLinear->superior[idx] = 0.0;
		sistemaLinear->principal[idx] = central;
		sistemaLinear->inferior[idx] = esquerda;
		sistemaLinear->inferiorAfastada[idx] = baixo;
		sistemaLinear->b[idx] = calculaFuncao((i * hy), (M_PI - hx)) - (0.0 * direita);
		idx++;
	}

	/*Cálculo do Ponto Superior Esquerdo*/
	bordaSuperior = limiteSuperior(0.0 + hx);
	sistemaLinear->superiorAfastada[idx] = 0.0;
	sistemaLinear->superior[idx] = direita;
	sistemaLinear->principal[idx] = central;
	sistemaLinear->inferior[idx] = 0.0;
	sistemaLinear->inferiorAfastada[idx] = baixo;
	sistemaLinear->b[idx] = calculaFuncao(hx, (M_PI - hy)) - (0.0 * esquerda) - (bordaSuperior * cima);
	idx++;

	/*Cálculo dos pontos da Borda Superior*/
	for (int i = 2; i <= nx - 1; i++)	{
		bordaSuperior = limiteSuperior(i * hx);
		sistemaLinear->superiorAfastada[idx] = 0.0;
		sistemaLinear->superior[idx] = direita;
		sistemaLinear->principal[idx] = central;
		sistemaLinear->inferior[idx] = esquerda;
		sistemaLinear->inferiorAfastada[idx] = baixo;
		sistemaLinear->b[idx] = calculaFuncao((i * hx), (M_PI - hy)) - (bordaSuperior * cima);
		idx++;
	}

	/*Cálculo do Ponto Superior Direito*/
	bordaSuperior = limiteSuperior(M_PI - hx);
	sistemaLinear->superiorAfastada[idx] = 0.0;
	sistemaLinear->superior[idx] = 0.0;
	sistemaLinear->principal[idx] = central;
	sistemaLinear->inferior[idx] = esquerda;
	sistemaLinear->inferiorAfastada[idx] = baixo;
	sistemaLinear->b[idx] = calculaFuncao((M_PI - hx), (M_PI - hy)) - (0.0 * direita) - (bordaSuperior * cima);
	idx++;

	// for (int i = 0; i < (nx * ny); ++i){
	// 	printf("%.5lf ", sistemaLinear->superiorAfastada[i]);
	// }

	// printf("\n");

	// for (int i = 0; i < (nx * ny); ++i){
	// 	printf("%.5lf ", sistemaLinear->superior[i]);
	// }

	// printf("\n");

	// for (int i = 0; i < (nx * ny); ++i){
	// 	printf("%.5lf ", sistemaLinear->principal[i]);
	// }

	// printf("\n");

	// for (int i = 0; i < (nx * ny); ++i){
	// 	printf("%.5lf ", sistemaLinear->inferior[i]);
	// }

	// printf("\n");

	// for (int i = 0; i < (nx * ny); ++i){
	// 	printf("%.5lf ", sistemaLinear->inferiorAfastada[i]);
	// }

	for (int i = 0; i < (nx * ny); ++i){
		printf("%.5lf %.5lf %.5lf %.5lf %.5lf\n", sistemaLinear->inferiorAfastada[i], sistemaLinear->inferior[i], sistemaLinear->principal[i], sistemaLinear->superior[i], sistemaLinear->superiorAfastada[i]); 
	}

	printf("\n");

	return (sistemaLinear);
}

/**
 * @brief Cálculo da norma L2 do Resíduo.
 * @param SL Ponteiro para o sistema linear
 * @param x Vetor de .
 * @param nx Número de pontos a serem calculados na dimensão X.
 * @param ny Número de pontos a serem calculados na dimensão Y.
 *
 * @return Retorna o cálculo na norma Euclidiana.
 */
double normaL2Residuo(sL *SL, double *x, int nx, int ny){ 

	double residuo, normaL2, multiplicacao;

	// residuo = alocaVetor(nx * ny);


	// multiplicacao = sL->inferiorAfastada[i] * x[?] +  sL->inferior[i] * x[?] + sL->principal[i] * x[?] + sL->superior[i] * x[?] + sL->superiorAfastada[i] * x[?];

	// for (int i = 0; i < (nx * ny); i++){
	// 	residuo[i] = (SL->b[i] - A[?] * x[i]);
	// 	normaL2 += residuo[i]*residuo[i];
	// }

	return sqrt(normaL2);
}

/**
 * @brief Método de Gauss-Seidel
 * @param SL Ponteiro para o sistema linear
 * @param x ponteiro para o vetor solução
 * @param erro menor erro aproximado para encerrar as iterações
 *
 * @return Retorna
 */
int gaussSeidel (sL *SL, double *x, int nx, int ny, double erro, int itr) {
	double norma, diff, xk;
	int i, k = 0;

	norma = erro + 1.0;

	while (k < itr && norma > erro) {
		i = 0;
		
		xk = (SL->b[i] - (SL->superior[i]*x[i+1] + SL->superiorAfastada[i]*x[i+1])) / SL->principal[i];
		norma = fabs(xk - x[i]);
		x[i] = xk;


		for (i = 1; i < (nx * ny) - 1; i++)	{	
			xk = (SL->b[i] - ((SL->superiorAfastada[i]*x[i+1] + SL->superior[i]*x[i+1]) - (SL->inferior[i]*x[i-1] + SL->inferiorAfastada[i]*x[i-1]))) / SL->principal[i];
			// Calcula norma || x (k) – x (k – 1) ||
			diff = fabs(xk - x[i]);
			norma = (diff > norma) ? (diff) : (norma);
			x[i] = xk;
		}

		xk = (SL->b[i] - (SL->inferior[i]*x[i-1] + SL->inferiorAfastada[i]*x[i-1])) / SL->principal[i];
		norma = (diff > norma) ? (diff) : (norma);
		x[i] = xk;

		++k;
	}

	if (k < itr){
		return 0;
	} else {
		return -1;
	}
}
	