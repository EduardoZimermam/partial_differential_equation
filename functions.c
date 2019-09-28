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

	if (*nx < 1) {
		fprintf(stderr, "ERRO: falta o parâmetro obrigatório -nx (número de pontos a serem calculados em X)\n");
		return (-1);
	}

	if (*ny < 1) {
		fprintf(stderr, "ERRO: falta o parâmetro obrigatório -ny (número de pontos a serem calculados em Y)\n");
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
double calculaEquacaoDiferencialParcial(double hx, double hy, double n, double nx, double ny, sL *sistemaLinear) {
	double esquerda, direita, cima, baixo, central, bordaSuperior, bordaInferior;
	int idx = 0;

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

	/*Cálculo do Ponto Superior Esquerdo*/
	bordaSuperior = limiteSuperior(0.0 + hx);
	// printf("Superior/Esquerda: %.15lf + %.15lf + %.15lf = %.15lf \n", direita, central, baixo, (calculaFuncao(hx, (M_PI - hy)) - (0.0 * esquerda) - (bordaSuperior * cima)));

	sistemaLinear->principal[idx] = central;
	sistemaLinear->superior[idx] = direita;
	sistemaLinear->superiorAfastada[idx] = 0.0;
	sistemaLinear->inferior[idx] = 0.0;
	sistemaLinear->inferiorAfastada[idx] = baixo;
	sistemaLinear->b[idx] = calculaFuncao(hx, (M_PI - hy)) - (0.0 * esquerda) - (bordaSuperior * cima);
	idx++;

	/*Cálculo do Ponto Inferior Esquerdo*/
	bordaInferior = limiteInferior(0.0 + hx);
	// printf("Inferior/Esquerda: %.15lf + %.15lf + %.15lf = %.15lf \n", direita, central, cima, (calculaFuncao(hx, hy) - (0.0 * esquerda) - (bordaInferior * baixo)));

	sistemaLinear->principal[idx] = central;
	sistemaLinear->superior[idx] = direita;
	sistemaLinear->superiorAfastada[idx] = cima;
	sistemaLinear->inferior[idx] = 0.0;
	sistemaLinear->inferiorAfastada[idx] = 0.0;
	sistemaLinear->b[idx] = calculaFuncao(hx, hy) - (0.0 * esquerda) - (bordaInferior * baixo);
	idx++;

	/*Cálculo dos pontos da Borda Inferior*/
	for (int i = 2; i <= nx - 1; i++)	{
		bordaInferior = limiteInferior(i);
		// printf("Para i=%d: %.15lf + %.15lf + %.15lf + %.15lf = %.15lf \n", i, direita, esquerda, central, cima, (calculaFuncao((i * hx), (0.0 + hy)) - (bordaInferior * baixo)));
		sistemaLinear->principal[idx] = central;
		sistemaLinear->superior[idx] = direita;
		sistemaLinear->superiorAfastada[idx] = cima;
		sistemaLinear->inferior[idx] = esquerda;
		sistemaLinear->inferiorAfastada[idx] = 0.0;
		sistemaLinear->b[idx] = calculaFuncao((i * hx), (0.0 + hy)) - (bordaInferior * baixo);
		idx++;
	}

	
	/*Cálculo do Ponto Inferior Direito*/
	bordaInferior = limiteInferior(M_PI - hx);
	// printf("Inferior/Direita: %.15lf + %.15lf + %.15lf = %.15lf \n", esquerda, central, cima, (calculaFuncao((M_PI - hx), (0.0 + hy)) - (0.0 * direita) - (bordaInferior * baixo)));

	sistemaLinear->principal[idx] = central;
	sistemaLinear->superior[idx] = 0.0;
	sistemaLinear->superiorAfastada[idx] = cima;
	sistemaLinear->inferior[idx] = esquerda;
	sistemaLinear->inferiorAfastada[idx] = 0.0;
	sistemaLinear->b[idx] = calculaFuncao((M_PI - hx), (0.0 + hy)) - (0.0 * direita) - (bordaInferior * baixo);
	idx++;

	/*Cálculo do Ponto Superior Direito*/
	bordaSuperior = limiteSuperior(M_PI - hx);
	// printf("Superior/Direita: %.15lf + %.15lf + %.15lf = %.15lf \n", esquerda, central, baixo, (calculaFuncao((M_PI - hx), (M_PI - hy)) - (0.0 * direita) - (bordaSuperior * cima)));

	sistemaLinear->principal[idx] = central;
	sistemaLinear->superior[idx] = 0.0;
	sistemaLinear->superiorAfastada[idx] = 0.0;
	sistemaLinear->inferior[idx] = esquerda;
	sistemaLinear->inferiorAfastada[idx] = baixo;
	sistemaLinear->b[idx] = calculaFuncao((M_PI - hx), (M_PI - hy)) - (0.0 * direita) - (bordaSuperior * cima);
	idx++;

	/*Cálculo dos pontos da Borda Superior*/
	for (int i = 2; i <= nx - 1; i++)	{
		bordaSuperior = limiteSuperior(i * hx);
		// printf("Para i=%d: %.15lf + %.15lf + %.15lf + %.15lf = %.15lf \n", i, direita, esquerda, central, baixo, (calculaFuncao((i * hx), (M_PI - hy)) - (bordaSuperior * cima)));
		sistemaLinear->principal[idx] = central;
		sistemaLinear->superior[idx] = direita;
		sistemaLinear->superiorAfastada[idx] = 0.0;
		sistemaLinear->inferior[idx] = esquerda;
		sistemaLinear->inferiorAfastada[idx] = baixo;
		sistemaLinear->b[idx] = calculaFuncao((i * hx), (M_PI - hy)) - (bordaSuperior * cima);
		idx++;
	}

	/*Cálculo da Lateral Esqueda*/
	for (int i = 2; i <= ny - 1; i++)	{
		// printf("Para i=%d: %.15lf + %.15lf + %.15lf + %.15lf = %.15lf \n", i, direita, central, cima, baixo, (calculaFuncao((i * hy), (0.0 + hx)) - (0.0 * esquerda)));
		sistemaLinear->principal[idx] = central;
		sistemaLinear->superior[idx] = direita;
		sistemaLinear->superiorAfastada[idx] = cima;
		sistemaLinear->inferior[idx] = 0.0;
		sistemaLinear->inferiorAfastada[idx] = baixo;
		sistemaLinear->b[idx] = calculaFuncao((i * hy), (0.0 + hx)) - (0.0 * esquerda);
		idx++;
	}

	/*Cálculo da Lateral Direita*/
	for (int i = 2; i <= ny - 1; i++)	{
		// printf("Para i=%d: %.15lf + %.15lf + %.15lf + %.15lf = %.15lf \n", i, esquerda, central, cima, baixo, (calculaFuncao((i * hy), (M_PI - hx)) - (0.0 * direita)));
		sistemaLinear->principal[idx] = central;
		sistemaLinear->superior[idx] = 0.0;
		sistemaLinear->superiorAfastada[idx] = cima;
		sistemaLinear->inferior[idx] = esquerda;
		sistemaLinear->inferiorAfastada[idx] = baixo;
		sistemaLinear->b[idx] = calculaFuncao((i * hy), (M_PI - hx)) - (0.0 * direita);
		idx++;
	}

	/*Cálculo dos Pontos internos da malha*/
	for (int i = 2; i <= nx - 1; i++)	{
		for (int j = 2; j <= ny - 1; j++) {
			// printf("Para i=%d e j=%d: %.15lf + %.15lf + %.15lf + %.15lf + %.15lf = %.15lf \n", i, j, esquerda, direita, central, cima, baixo, calculaFuncao((i * hx), (j * hy)));
			sistemaLinear->principal[idx] = central;
			sistemaLinear->superior[idx] = direita;
			sistemaLinear->superiorAfastada[idx] = cima;
			sistemaLinear->inferior[idx] = esquerda;
			sistemaLinear->inferiorAfastada[idx] = baixo;
			sistemaLinear->b[idx] = calculaFuncao((i * hx), (j * hy));
			idx++;
		}
	}
}






/**
 * @brief Método de Gauss-Seidel
 * @param SL Ponteiro para o sistema linear
 * @param x ponteiro para o vetor solução
 * @param erro menor erro aproximado para encerrar as iterações
 *
 * @return Retorna
 */
// int gaussSeidel(SistLinear_t *SL, real_t *x, real_t erro)
// {

// 	double tTotal, tIteracao, soma, *Xk; //X novo
// 	unsigned int n = SL->n;

// 	Xk = malloc(sizeof(double) * n);

// 	for (int k = 0; k < MAXIT; k++)
// 	{
// 		double start = timestamp();
// 		for (int i = 0; i < n; i++)
// 		{ // i = 1; i <= nx-1
// 			soma = 0.0;
// 			for (int j = 0; j < i; j++)
// 			{ // i = 1; i <= ny-1
// 				soma += SL->A[i * n + j] * Xk[j];
// 			}

// 			for (int j = (i + 1); j < n; j++)
// 			{
// 				soma += SL->A[i * n + j] * x[j];
// 			}

// 			Xk[i] = (SL->b[i] - soma) / SL->A[i * n + i];
// 		}

// 		erro = 0.0;
// 		for (int i = 0; i < n; i++)
// 		{ //criterio de parada
// 			double sub = fabs(x[i] - Xk[i]);
// 			if (erro < sub)
// 			{
// 				erro = sub;
// 			}
// 		}
// 		double end = timestamp() - start;

// 		for (int i = 0; i < n; ++i)
// 		{ //troca de ponteiro
// 			x[i] = Xk[i];
// 		}

// 		tTotal += end;
// 		tIteracao = tTotal / k;

// 		if (erro < EPS)
// 		{
// 			//printf("Tempo por iteração Gauss-Seidel: %lf.\n", tIteracao);
// 			//printf("Tempo total Gauss-Seidel: %lf.\n", tTotal);
// 			return (k);
// 		}
// 	}

// 	fprintf(stderr, "O método Gauss-Seidel não converge.\n");
// 	return (-1);
// }
