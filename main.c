#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "include/genericstack.h"

#define SIZE(X) sizeof(X) / sizeof(X[0])
#define NUM_ALFABETO 26

typedef signed char int8;

typedef struct{
    char simb;
    int8 value;
} boolean;

typedef enum {TAUTOLOGY, CONTRADICTION, CONTINGENCY, AMOUNT_VALENCE} VALENCE_TRUTH_TABLE;

typedef struct{
    char data[256];
    boolean var[NUM_ALFABETO];
    int8 count;
}logic_expr;

typedef struct {
    logic_expr expr;
    char **data;
    int rows, cols;
    VALENCE_TRUTH_TABLE valence;
}truth_table;

const char* valid_operates[] = { "+", "v", ".", "^", "*", "->", "<->", "\'", "(", ")"};

int8 eval(char *str);
int8 parsestr(char **str);

char** Matrix_char(size_t rows, size_t cols){
    char** matrix = (char**) malloc(rows * sizeof(char*));
    if(!matrix) return NULL;

    *matrix = (char*) malloc(cols * rows * sizeof(char));
    if(!matrix[0]){
        free(matrix);
        return NULL;
    }
    for(int i = 0; i < rows; i++){
        matrix[i] = *matrix + i * cols;
    }
    return matrix;
}

void free_matrix(char*** matrix){
    if(matrix == NULL || *matrix == NULL) return;
    free((*matrix)[0]);
    free(*matrix);
    matrix = NULL;      
}

void skipspace(char **str)
{
    if (str == NULL)
    return;
    while (**str == ' ')
    {
        (*str)++;
    }
}

char invalid_operate(char* str){
    int is_invalid_operate;
    while(*str){
        is_invalid_operate = 1;
        for(size_t i = 0; i < SIZE(valid_operates); i++){
            int len = strlen(valid_operates[i]);
            if(strncmp(str, valid_operates[i], len) == 0){
                str += len;
                is_invalid_operate = 0;
                break;
            }
        } 
        if(is_invalid_operate && !isalpha(*str) && !isspace(*str) && *str != '1' && *str != '0'){
            return *str;
        }
        str++;
    }
    return 0;
}

int8 bracketsIsbalanced(char *str)
{
    stack_char *stk = new_stack_char();
    while (*str)
    {
        if (*str == '(')
        {
            stk->push(stk, *str);
        }
        else if (*str == ')')
        {
            if (stk->top == -1)
                return 0;
            stk->pop(stk);
        }
        str++;
    }
    int flag = stk->top == -1;
    delete_stack_char(stk);
    return flag;
}

void strupper(char *str)
{
    while (*str)
    {
        if (islower(*str))
        {
            *str = toupper(*str);
        }
        str++;
    }
}

int8 parsevalue(char **str)
{
    char *end;
    skipspace(str);
    int8 result = strtol(*str, &end, 10);
    skipspace(str);

    if (end == *str)
    {
        printf("espera-se um numero \\ expressão vazia\n");
        exit(EXIT_FAILURE);
    }
    *str = end;
    return result;
}

int8 parsebracket(char **str)
{
    skipspace(str);
    if (**str == '(')
    {
        (*str)++;
        int8 result = parsestr(str);
        (*str)++;
        return result;
    }
    return parsevalue(str);
}

int8 parsenot(char **str)
{
    skipspace(str);
    int8 result = parsebracket(str);
    skipspace(str);

    while (**str == '\'')
    {
        (*str)++;
        result = !(result);
    }
    return result;
}

int8 parseand(char **str)
{
    skipspace(str);
    int8 result = parsenot(str);
    skipspace(str);

    while (**str == '*' || **str == '^' || **str == '.')
    {
        (*str)++;
        int8 nextfactor = parseand(str);
        result = result && nextfactor;
        skipspace(str);
    }
    return result;
}

int8 parseor(char **str)
{
    skipspace(str);
    int8 result = parseand(str);
    skipspace(str);

    while (**str == '+' || **str == 'v')
    {
        (*str)++;
        int8 nextterm = parseor(str);
        result = result || nextterm;
        skipspace(str);
    }
    return result;
}

int8 parsecondit(char **str)
{
    skipspace(str);
    int8 anteced = parseor(str);
    while (strncmp(*str, "->", 2) == 0)
    {
        (*str) += strlen("->");
        int8 consec = parsecondit(str);
        anteced = !anteced || consec;
    }
    return anteced;
}

int8 parsestr(char **str)
{
    skipspace(str);
    int8 left_side = parsecondit(str);
    while (strncmp(*str, "<->", 3) == 0)
    {
        (*str) += strlen("<->");
        int8 right_side = parsestr(str);
        left_side = (!left_side && !right_side) || (left_side && right_side);
    }
    return left_side;
}

int8 eval(char *str){
    return parsestr(&str);
}

int8 setvars(logic_expr* expr)
{
    char letter[NUM_ALFABETO] = {};
    strupper(expr->data);
    char* iterator = expr->data;
    while (*iterator)
    {
        if (isalpha(*iterator)){
            letter[*iterator - 'A'] = 1;
        }
        iterator++;
    }
    for (int i = 'A'; i <= 'Z'; i++)
    {
        if (letter[i - 'A']){
            expr->var[expr->count++].simb = i;
        }
    }
    return expr->count * (expr->count != 0);
}

int eval_X(logic_expr* expr)
{
    char *iterator = malloc(strlen(expr->data) + 1);
    strcpy(iterator, expr->data);
    char *begin = iterator;
    while (*iterator)
    {
        if (!isalpha(*iterator))
        {
            iterator++;
            continue;
        }
        for (int i = 0; i < expr->count; i++)
        {
            if (*iterator == expr->var[i].simb)
            {
                *iterator = expr->var[i].value + '0';
            }
        }
        iterator++;
    }
    int8 result = eval(begin);
    free(begin);
    return result;
}

void classify_truth_table(truth_table* table){
    int sum = 0;
    for(int i = 0; i < table->rows; i++){
        sum += table->data[i][table->cols - 1] - '0';
    }
    if(sum == table->rows){
        table->valence = TAUTOLOGY;
    } else if (sum == 0 ){
        table->valence = CONTRADICTION;
    } else {
        table->valence = CONTINGENCY;
    }
}

truth_table* generate_truth_table(char* exp){
    strupper(exp);
    int len = strlen(exp);

    if(!bracketsIsbalanced(exp)){
        printf("Erro: parenteses desbalanceados! verifique se cada '(' tem um ')' correspondente\n");
        return NULL;
    }
    int ch = invalid_operate(exp);
    if(ch){
        printf("Erro: expressão invalida: '%c'\n", ch);
        return NULL;
    }
    truth_table* table = malloc(sizeof(truth_table));
    table->expr.count = 0;
    strcpy(table->expr.data, exp);
    setvars(&table->expr);
    table->rows = (1 << table->expr.count);
    table->cols = table->expr.count + 1; 
    //gera matriz dinamica
    table->data = Matrix_char(table->rows, table->cols);

    for(int mask = 0; mask < (1 << table->expr.count); mask++){
        for(int i = 0, j = table->expr.count - 1; i < table->expr.count; i++, j--){
            table->expr.var[i].value = (mask >> j) & 1;
            table->data[mask][i] = table->expr.var[i].value + '0';
        }
        table->data[mask][table->cols - 1] = eval_X(&table->expr) + '0'; 
    }
    classify_truth_table(table);
    return table;
}

void delete_truth_table(truth_table* table){
    if(table == NULL) return;
    free_matrix(&table->data);
    free(table);
    table = NULL;
}

void print_valence_truth_table(truth_table* table){
    if(table->valence == TAUTOLOGY) {
        printf("\nTAUTOLOGIA\n\n");
    } else if( table->valence == CONTRADICTION){
        printf("\nCONTRADIÇÃO\n\n");
    } else {
        printf("\nCONTINGENCIA\n\n");
    }
}

void print_truth_table(truth_table* table){
    if(table == NULL)
        return;
    int len = strlen(table->expr.data);
    //linha de variaveis;
    putchar('\n');
    for(int i = 0; i < table->expr.count; i++){
        printf("%c", table->expr.var[i].simb);
        printf("%s", (i != table->expr.count - 1) ? "\t" : " ");
    }
    printf("|%c%s|\n", len % 2 ? '\0' : ' ', table->expr.data);
    //demais linhas
    for(int i = 0; i < table->rows; i++){
        for(int j = 0; j < table->cols; j++){
            if(j == table->cols - 1){
                putchar('|');

                for(int k = 0; k < len / 2; k++)  putchar(' ');

                printf("%c", table->data[i][j]);

                for(int k = 0; k < len / 2; k++)  putchar(' ');

                putchar('|');
                continue;
            }
            printf("%c", table->data[i][j]);
            printf("%s", j != table->cols - 2 ? "\t" : " ");

        }
        putchar('\n');
    }
    print_valence_truth_table(table);
    delete_truth_table(table);
}

int main()
{
    char input[512], op, c;
    while(1){
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif
        
        printf("Informe uma expressão logica\n");
        fgets(input, sizeof (input), stdin);
        input[strcspn(input, "\n")] = '\0';

        truth_table* table = generate_truth_table(input);
        print_truth_table(table);

        printf("quer a tabela de outra expressão? (s/n)\n");
        op = getchar();
        while(c = getchar() != '\n' && c != EOF);

        if(op == 'n' || op == 'N') break;
    }

    return 0;
}