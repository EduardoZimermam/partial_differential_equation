# Resolução de Equação Diferencial Parcial pelo Método de Gauss Seidel

* Projeto apresentado a disciplina de Introdução a Computação Científica com o objetivo de resolver através do Método de Gauss Seidel uma Equação Diferencial Parcial discretizada como uma malha com bordas de valores conhecidos.

## Colaboradores

[Annelyse Schatzmann](https://github.com/Schatzmann)

[Eduardo Zimermam Pereira](https://github.com/EduardoZimermam)

## Para executar o Projeto

* Compilar:
  ```sh
  make
  ```

* Executar:
 ```sh
 ./pdeSolver -nx <Nº de pontos no eixo x> -ny <Nº de pontos no eixo y> -i <Nº de iterações máximas para Método de Gauss Seidel>
 ```

* Para gerar a documentação pelo Doxygen:
  ```sh
  make doc
  ```

  * Neste caso, um diretório na raiz do projeto será gerado com o nome "html" para abrir a documentação é necessaŕio executar o arquivo index.html.
