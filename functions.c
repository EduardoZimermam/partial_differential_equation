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
#include <likwid.h>

#include "functions.h"
#include "utils.h"

/**
 * @brief Lê a linha de comando de forma dinâmica.
 * @param argc    Número de argumentos da linha de comando.
 * @param argv    Vetor de argumentos da linha de comando.
 * @param nx      Número de pontos a serem calculados na dimensão X.
 * @param ny      Número de pontos a serem calculados na dimensão Y.
 * @param itr  	  Número de iterações a serem executadas.
 * @param caminhoSaida  Caminho para escrever a solução final.
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
 * @param pontosSL Estrutura que guarda cada ponto discretizado na malha
 * @return Preenche os vetores de diagonais do sistema linear.
 */
sL* calculaEquacaoDiferencialParcial(double hx, double hy, double n, double nx, double ny, ponto *pontosSL) {
	double esquerda, direita, cima, baixo, central, bordaSuperior, bordaInferior;
	int idx = 0;
	sL *sistemaLinear;

	/*Cálculo dos Coeficientes Uij*/
	esquerda = -2 * (hy * hy) - hx * (hy * hy);
	direita = -2 * (hy * hy) + hx * (hy * hy);
	cima = -2 * (hx * hx) + hy * (hx * hx);
	baixo = -2 * (hx * hx) - hy * (hx * hx);
	central = 4 * (hy * hy) + 4 * (hx * hx) + 2 * (hx * hx) * (hy * hy) * n;

	/* Alocação de todo o sistema linear em um único vetor para melhorar a métrica de CACHE MISS */
	sistemaLinear = malloc(sizeof(sL));
	sistemaLinear->sistemaLinearCompleto = alocaVetor(((nx * ny) * PULO) + 1);

	/*Cálculo do Ponto Inferior Esquerdo*/
	bordaInferior = limiteInferior(0.0 + hx);
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 1] = 0.0;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 2] = 0.0;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 3] = central;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 4] = direita;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 5] = cima;
	pontosSL->x[idx] = hx;
	pontosSL->y[idx] = hy;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 6] = calculaFuncao(pontosSL->x[idx], pontosSL->y[idx]) - (0.0 * esquerda) - (bordaInferior * baixo);
	idx++;

	/*Cálculo dos pontos da Borda Inferior*/
	for (int i = 2; i <= nx - 1; i++)	{
		bordaInferior = limiteInferior(i * hx);
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 1] = 0.0;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 2] = esquerda;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 3] = central;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 4] = direita;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 5] = cima;
		pontosSL->x[idx] = i * hx;
		pontosSL->y[idx] = hy;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 6] = calculaFuncao(pontosSL->x[idx], pontosSL->y[idx]) - (bordaInferior * baixo);
		idx++;
	}

	/*Cálculo do Ponto Inferior Direito*/
	bordaInferior = limiteInferior(M_PI - hx);
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 1] = 0.0;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 2] = esquerda;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 3] = central;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 4] = 0.0;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 5] = cima;
	pontosSL->x[idx] = M_PI - hx;
	pontosSL->y[idx] = hy;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 6] = calculaFuncao(pontosSL->x[idx], pontosSL->y[idx]) - (0.0 * direita) - (bordaInferior * baixo);
	idx++;

	/*Cálculo dos Pontos internos da malha*/
	for (int i = 2; i <= ny - 1; i++)	{
		
		/* Cálculo do ponto da borda da esquerda */
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 1] = baixo;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 2] = 0.0;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 3] = central;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 4] = direita;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 5] = cima;
		pontosSL->x[idx] = hx;
		pontosSL->y[idx] = i * hy;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 6] = calculaFuncao(pontosSL->x[idx], pontosSL->y[idx]) - (0.0 * esquerda);
		idx++;

		for (int j = 2; j <= nx - 1; j++) {
			sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 1] = baixo;
			sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 2] = esquerda;
			sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 3] = central;
			sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 4] = direita;
			sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 5] = cima;
			pontosSL->x[idx] = j * hx;
			pontosSL->y[idx] = i * hy;
			sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 6] = calculaFuncao(pontosSL->x[idx], pontosSL->y[idx]);
			idx++;
		}

		/* Cálculo do ponto da borda da direita */
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 1] = baixo;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 2] = esquerda;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 3] = central;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 4] = 0.0;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 5] = cima;
		pontosSL->x[idx] = M_PI - hx;
		pontosSL->y[idx] = i * hy;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 6] = calculaFuncao(pontosSL->x[idx], pontosSL->y[idx]) - (0.0 * direita);
		idx++;
	}

	/*Cálculo do Ponto Superior Esquerdo*/
	bordaSuperior = limiteSuperior(0.0 + hx);
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 1] = baixo;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 2] = 0.0;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 3] = central;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 4] = direita;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 5] = 0.0;
	pontosSL->x[idx] = hx;
	pontosSL->y[idx] = M_PI - hy;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 6] = calculaFuncao(pontosSL->x[idx], pontosSL->y[idx]) - (0.0 * esquerda) - (bordaSuperior * cima);
	idx++;

	/*Cálculo dos pontos da Borda Superior*/
	for (int i = 2; i <= nx - 1; i++)	{
		bordaSuperior = limiteSuperior(i * hx);
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 1] = baixo;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 2] = esquerda;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 3] = central;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 4] = direita;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 5] = 0.0;
		pontosSL->x[idx] = i * hx;
		pontosSL->y[idx] = M_PI - hy;
		sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 6] = calculaFuncao(pontosSL->x[idx], pontosSL->y[idx]) - (bordaSuperior * cima);
		idx++;
	}

	/*Cálculo do Ponto Superior Direito*/
	bordaSuperior = limiteSuperior(M_PI - hx);
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 1] = baixo;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 2] = esquerda;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 3] = central;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 4] = 0.0;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 5] = 0.0;
	pontosSL->x[idx] = M_PI - hx;
	pontosSL->y[idx] = M_PI - hy;
	sistemaLinear->sistemaLinearCompleto[(idx * PULO) + 6] = calculaFuncao(pontosSL->x[idx], pontosSL->y[idx]) - (0.0 * direita) - (bordaSuperior * cima);
	idx++;

	return (sistemaLinear);
}

/**
 * @brief Cálculo do resíduo e da norma L2 do mesmo.
 * @param sistemaLinear Ponteiro para o sistema linear
 * @param x Vetor de incógnitas.
 * @param nx Número de pontos a serem calculados na dimensão X.
 * @param ny Número de pontos a serem calculados na dimensão Y.
 *
 * @return Retorna o cálculo na norma Euclidiana.
 */
double normaL2Residuo(sL *sistemaLinear, int nx, int ny){ 

	double *residuo, normaL2, multiplicacao;
	int i = 0;

	residuo = alocaVetor(nx * ny);

	LIKWID_MARKER_START("CALCULO-RESIDUO");

	residuo[i] = sistemaLinear->sistemaLinearCompleto[(i * PULO) + 6] - 
				(((sistemaLinear->sistemaLinearCompleto[(i * PULO) + 3] * sistemaLinear->sistemaLinearCompleto[(i * PULO) + 7]) + 
				  (sistemaLinear->sistemaLinearCompleto[(i * PULO) + 4] * sistemaLinear->sistemaLinearCompleto[((i + 1) * PULO) + 7])) + 
				  (sistemaLinear->sistemaLinearCompleto[(i * PULO) + 5] * sistemaLinear->sistemaLinearCompleto[((i + (nx - 1)) * PULO) + 7]));

	for (i = 1; i - (nx - 1) < 0; ++i){
		residuo[i] = sistemaLinear->sistemaLinearCompleto[(i * PULO) + 6] - 
				  (((sistemaLinear->sistemaLinearCompleto[(i * PULO) + 3] * sistemaLinear->sistemaLinearCompleto[(i * PULO) + 7]) + 
				    (sistemaLinear->sistemaLinearCompleto[(i * PULO) + 2] * sistemaLinear->sistemaLinearCompleto[((i - 1) * PULO) + 7])) + 
				   ((sistemaLinear->sistemaLinearCompleto[(i * PULO) + 4] * sistemaLinear->sistemaLinearCompleto[((i + 1) * PULO) + 7]) + 
				    (sistemaLinear->sistemaLinearCompleto[(i * PULO) + 5] * sistemaLinear->sistemaLinearCompleto[((i + (nx - 1)) * PULO) + 7])));
	}

	for (i = i; (i + (nx - 1)) < (nx * ny); ++i){
		residuo[i] = sistemaLinear->sistemaLinearCompleto[(i * PULO) + 6] - 
				  (((sistemaLinear->sistemaLinearCompleto[(i * PULO) + 3] * sistemaLinear->sistemaLinearCompleto[(i * PULO) + 7]) + 
				    (sistemaLinear->sistemaLinearCompleto[(i * PULO) + 1] * sistemaLinear->sistemaLinearCompleto[((i - (nx - 1)) * PULO) + 7])) + 
				   ((sistemaLinear->sistemaLinearCompleto[(i * PULO) + 2] * sistemaLinear->sistemaLinearCompleto[((i - 1) * PULO) + 7]) + 
				    (sistemaLinear->sistemaLinearCompleto[(i * PULO) + 4] * sistemaLinear->sistemaLinearCompleto[((i + 1) * PULO) + 7])) + 
					(sistemaLinear->sistemaLinearCompleto[(i * PULO) + 5] * sistemaLinear->sistemaLinearCompleto[((i + (nx - 1)) * PULO) + 7]));
	}

	for (i = i; i < (nx * ny) - 1; ++i){
		residuo[i] = sistemaLinear->sistemaLinearCompleto[(i * PULO) + 6] - 
				  (((sistemaLinear->sistemaLinearCompleto[(i * PULO) + 3] * sistemaLinear->sistemaLinearCompleto[(i * PULO) + 7]) + 
				    (sistemaLinear->sistemaLinearCompleto[(i * PULO) + 1] * sistemaLinear->sistemaLinearCompleto[((i - (nx - 1)) * PULO) + 7])) + 
				   ((sistemaLinear->sistemaLinearCompleto[(i * PULO) + 2] * sistemaLinear->sistemaLinearCompleto[((i - 1) * PULO) + 7]) + 
					(sistemaLinear->sistemaLinearCompleto[(i * PULO) + 4] * sistemaLinear->sistemaLinearCompleto[((i + 1) * PULO) + 7])));
	}

	residuo[i] = sistemaLinear->sistemaLinearCompleto[(i * PULO) + 6] - 
			  (((sistemaLinear->sistemaLinearCompleto[(i * PULO) + 3] * sistemaLinear->sistemaLinearCompleto[(i * PULO) + 7]) + 
			    (sistemaLinear->sistemaLinearCompleto[(i * PULO) + 1] * sistemaLinear->sistemaLinearCompleto[((i - (nx - 1)) * PULO) + 7])) + 
			    (sistemaLinear->sistemaLinearCompleto[(i * PULO) + 2] * sistemaLinear->sistemaLinearCompleto[((i - 1) * PULO) + 7]));

	LIKWID_MARKER_STOP("CALCULO-RESIDUO");

	for (i = 0; i < (nx * ny); ++i){
		normaL2 += (residuo[i] * residuo[i]);
	}


	return sqrt(normaL2);
}

/**
 * @brief Método de Gauss-Seidel
 * @param SL Ponteiro para o sistema linear
 * @param x ponteiro para o vetor solução.
 * @param nx Número de pontos a serem calculados na dimensão X.
 * @param ny Número de pontos a serem calculados na dimensão Y.
 * @param itr Número máximo de iterações que o método pode executar
 * @param tempoItr vetor dos tempos de cara iteração.
 * @param normaL2Itr vetor da norma L2 do resíduo de cada iteração.
 *
 * @return Retorna o total de iterações efetuadas se convergiu, ou -1 se houve erro.
 */
int gaussSeidel (sL *SL, int nx, int ny, int itr, double *tempoItr, double *normaL2Itr) {
	double diff, inicio, fim;
	int i, k = 0;

	while (k < itr) {
		i = 0;

		inicio = timestamp();

		LIKWID_MARKER_START("GAUSS-SEIDEL");
		
		SL->sistemaLinearCompleto[(i * PULO) + 7] = (SL->sistemaLinearCompleto[(i * PULO) + 6] - 
			 (SL->sistemaLinearCompleto[(i * PULO) + 4]*SL->sistemaLinearCompleto[((i+1) * PULO) + 7] + 
			  SL->sistemaLinearCompleto[(i * PULO) + 5]*SL->sistemaLinearCompleto[((i + (nx - 1)) * PULO) + 7])) / SL->sistemaLinearCompleto[(i * PULO) + 3];
		
		for (i = 1; i - (nx - 1) < 0; ++i){
			SL->sistemaLinearCompleto[(i * PULO) + 7] = (SL->sistemaLinearCompleto[(i * PULO) + 6] - 
				((SL->sistemaLinearCompleto[(i * PULO) + 5]*SL->sistemaLinearCompleto[((i + (nx - 1)) * PULO) + 7] + 
				  SL->sistemaLinearCompleto[(i * PULO) + 4]*SL->sistemaLinearCompleto[((i+1) * PULO) + 7]) - 
				  SL->sistemaLinearCompleto[(i * PULO) + 2]*SL->sistemaLinearCompleto[((i-1) * PULO) + 7])) / SL->sistemaLinearCompleto[(i * PULO) + 3];
		}
	
		for (i = i; (i + (nx - 1)) < (nx * ny); ++i){
			SL->sistemaLinearCompleto[(i * PULO) + 7] = (SL->sistemaLinearCompleto[(i * PULO) + 6] - 
				((SL->sistemaLinearCompleto[(i * PULO) + 5]*SL->sistemaLinearCompleto[((i + (nx - 1)) * PULO) + 7] + 
				  SL->sistemaLinearCompleto[(i * PULO) + 4]*SL->sistemaLinearCompleto[((i+1) * PULO) + 7]) - 
				 (SL->sistemaLinearCompleto[(i * PULO) + 2]*SL->sistemaLinearCompleto[((i-1) * PULO) + 7] + 
				  SL->sistemaLinearCompleto[(i * PULO) + 1]*SL->sistemaLinearCompleto[((i - (nx - 1)) * PULO) + 7]))) / SL->sistemaLinearCompleto[(i * PULO) + 3];
		}
	
		for (i = i; i < (nx * ny) - 1; ++i){
			SL->sistemaLinearCompleto[(i * PULO) + 7] = (SL->sistemaLinearCompleto[(i * PULO) + 6] - 
				 (SL->sistemaLinearCompleto[(i * PULO) + 4]*SL->sistemaLinearCompleto[((i+1) * PULO) + 7] - 
				 (SL->sistemaLinearCompleto[(i * PULO) + 2]*SL->sistemaLinearCompleto[((i-1) * PULO) + 7] + 
				  SL->sistemaLinearCompleto[(i * PULO) + 1]*SL->sistemaLinearCompleto[((i - (nx - 1)) * PULO) + 7]))) / SL->sistemaLinearCompleto[(i * PULO) + 3];
		}

		SL->sistemaLinearCompleto[(i * PULO) + 7] = (SL->sistemaLinearCompleto[(i * PULO) + 6] - 
			 (SL->sistemaLinearCompleto[(i * PULO) + 2]*SL->sistemaLinearCompleto[((i-1) * PULO) + 7] + 
			  SL->sistemaLinearCompleto[(i * PULO) + 1]*SL->sistemaLinearCompleto[((i- (nx - 1)) * PULO) + 7])) / SL->sistemaLinearCompleto[(i * PULO) + 3];

		LIKWID_MARKER_STOP("GAUSS-SEIDEL");

		fim = timestamp();

		normaL2Itr[k] = normaL2Residuo(SL, nx, ny);
		tempoItr[k] = fim - inicio;
		
		++k;
	}

	return k;
}


/**
 * @brief Escreve resultado na saída desejada.
 * @param tempoItr vetor dos tempos de cara iteração.
 * @param normaL2Itr vetor da norma L2 do resíduo de cada iteração.
 * @param itrConverge número de iterações realizadas.
 * @param tam Tamanho da malha discretizada (Quantidade de pontos)
 * @param pontosSL Estrutura que guarda o valor de cada ponto discretizado na malha
 * @param caminhoSaida ponteiro para o caminho do arquivo de saída.
 * @param x Resultado após a solução do sistema linear pelo método de Gauss Seidel
 *
 * @return Retorna a escrita do resultado no arquivo de saída ou no stdout.
 */
void printResultado(double *tempoItr, double *normaL2Itr, int itrConverge, int tam, ponto *pontosSL, char *caminhoSaida, sL *SL){

	FILE *arquivoSaida;
	double somaTempo, tempoMedioGS;

	if (strlen(caminhoSaida) == 0)
		arquivoSaida = stdout;
	else
		arquivoSaida = fopen(caminhoSaida, "w");


	for (int i = 0; i < itrConverge; ++i){
		somaTempo += tempoItr[i];
	}

	tempoMedioGS = somaTempo / itrConverge;

	fprintf(arquivoSaida, "###########\n");
	fprintf(arquivoSaida, "# Tempo Método GS: %lf\n", tempoMedioGS);
	fprintf(arquivoSaida, "#\n");
	fprintf(arquivoSaida, "# Norma L2 do Residuo\n");

	for (int i = 0; i < itrConverge; ++i){
		// fprintf(arquivoSaida, "# i=%d: %.15lf\n", i + 1, normaL2Itr[i]);
	}

	fprintf(arquivoSaida, "###########\n\n");

	/*print de todos os pontos da malha*/
	for (int i = 0; i < tam; ++i){
		// fprintf(arquivoSaida, "%.15lf %.15lf %.15lf\n", pontosSL->x[i], pontosSL->y[i], SL->sistemaLinearCompleto[(i * PULO) + 7]);
	}	
}