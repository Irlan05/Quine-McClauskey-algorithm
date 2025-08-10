# Quine-McCluskey Minimizer

Este é um projeto em C que implementa o algoritmo de **Quine-McCluskey** para a minimização de funções lógicas booleanas.

<p align="center">
  <img src="https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white" alt="C Language">
</p>

## O Que É?

O algoritmo de Quine-McCluskey é um método tabular para encontrar a forma de soma de produtos (SOP) mais simples para uma função booleana, ou seja, para encontrar os implicantes primos essenciais. É uma alternativa ao uso de Mapas de Karnaugh (K-maps), sendo mais adequado para funções com um número maior de variáveis.

## 📌 Funcionalidades

- **Minimização de Funções Booleanas**: O programa recebe uma lista de minitermos e utiliza o algoritmo de Quine-McCluskey para encontrar a expressão lógica minimizada.
- **Análise Tabular**: Implementa as duas fases do algoritmo: a fase de combinação para encontrar os implicantes primos e a fase de seleção para identificar os implicantes primos essenciais.
- **Suporte a Arquivos PLA**: O programa lê arquivos de entrada no formato padrão **PLA (Berkeley Logic Interchange Format)**, facilitando a integração com outras ferramentas de design lógico.
- **Geração de Saída**: O resultado minimizado (a soma de produtos) é escrito em um arquivo de saída, pronto para ser usado ou analisado.

## Como Usar

Para compilar o programa, use o seguinte comando:

```bash
gcc -o quine quine.c
./quine <nome_arquivo_PLA> <nome_arquivo_saida>