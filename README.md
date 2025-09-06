# Tabela verdade

Projeto feito em C que implementa um gerador de tabelas verdades, contendo as operações AND ('.', '^', '*'), OR ('+', 'v'),
implicação ('->'), bicondicional('<->') e negação('), levando em conta as devidas ordens de prescedencia.

exemplo: A -> B + C'.

## Estrutura
- `main.c` -> ponto de entrada
- `stack.h` -> pilha generica usada para balancear parenteses
- `gnrstack.h` -> include guards da pilha genérica

## Como compilar
gcc main.c -Iinclude -o build/program

## Como executar
./build/program

