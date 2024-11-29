// PraseWithRecursive.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PraseWithRecursive.h"
#include "Lexical.h"

QUATERNION pQuad[MAXLENGTH];
int NXQ = 0;    /*全局变量NXQ用于指示所要产生的下一个四元式的编号*/
int NXTemp = 1; //Tn的n值

WORD word;
Token token;

void ErrorPrint(const char *message){
    printf("ERROR: %s at token %d (%s,%s),%d:%d\n", message, currentToken, token_names[token.type],token.value,token.row, token.col+1);
    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        printf("Error: Cannot open output file\n");
        exit(661);
    }
    for (int i = 0; i < NXQ; i++) {
        fprintf(output_file, "(%s, %s, %s, %s)\n", pQuad[i].op, pQuad[i].arg1, pQuad[i].arg2, pQuad[i].result);
    }
    fclose(output_file);
    exit(666);
}

void GEN(char *Op, char *Arg1, char *Arg2, char *Result) {
    strcpy(pQuad[NXQ].op, Op);
    strcpy(pQuad[NXQ].arg1, Arg1);
    strcpy(pQuad[NXQ].arg2, Arg2);
    strcpy(pQuad[NXQ].result, Result);
    NXQ++;
}

// 用于更新TempID，sprintf转化为字符串存储
char *NewTemp(void) {
    char *TempID = (char *)malloc(MAXLENGTH);
    sprintf(TempID, "T%d", NXTemp++);
    return TempID;
}

char *E(void) {
    char opp[3], *E1_place, *E2_place, *Temp_place;
    E1_place = T();
    while (word.Class == PLUS || word.Class == MINUS || word.Class == EQ) {
        if (word.Class == PLUS) {
            strcpy(opp, "+");
        } else if (word.Class == MINUS) {
            strcpy(opp, "-");
        } else if (word.Class == EQ) {
            strcpy(opp, "==");
        }
        scanner();
        E2_place = T();
        Temp_place = NewTemp();
        GEN(opp, E1_place, E2_place, Temp_place);
        E1_place = Temp_place;
    }
    return E1_place;
}

char *T(void) {
    char opp[3], *T1_place, *T2_place, *Temp_place;
    T1_place = F();
    while (word.Class == MUL || word.Class == DIV || word.Class == LT || word.Class == GT) {
        if (word.Class == MUL) {
            strcpy(opp, "*");
        } else if (word.Class == DIV) {
            strcpy(opp, "/");
        } else if (word.Class == LT) {
            strcpy(opp, "<");
        } else if (word.Class == GT) {
            strcpy(opp, ">");
        }
        scanner();
        T2_place = F();
        Temp_place = NewTemp();
        GEN(opp, T1_place, T2_place, Temp_place);
        T1_place = Temp_place;
    }
    return T1_place;
}

char *F(void) {
    char *place;
    if (word.Class == ID || word.Class == INT || word.Class == REAL) { // 标识符
        place = strdup(token.value);    // 自动分配内存，然后复制字符串
        scanner();
        return place;
    } else if (word.Class == LP) { // 左括号
        scanner();
        place = E();
        if (word.Class == RP) { // 右括号
            scanner();
            return place;
        } else {
            ErrorPrint("dont have')'");
        }
    } else {
        ErrorPrint("Not A Valid Prase");
    }
    return NULL;        // 无效返回，其实根本不会到这里，完全是做个警告处理
}

char *A(void) {         // 赋值语句的处理
    char *E_place, *A_place;
    E_place = E();
    if (word.Class == IS) {
        scanner();
        A_place = E();
        GEN("=", E_place, "", A_place);
        return A_place;
    }
    return E_place;
}

void scanner() {
    token = getNextToken();
    word.Class = token.type;
    switch (word.Class) {
        case ID:
        case INT:
        case REAL:
            strcpy(word.Value.Val1, token.value);
            break;
        case PLUS:
        case MINUS:
        case MUL:
        case DIV:
        case LP:
        case RP:
        case EQ:
        case IS:
        case LT:
        case GT:
            strcpy(word.Value.Val4, token.value);
            break;
        case END:
            printf("End of tokens");
            return;
        default:
            ErrorPrint("Unknown token type");
            exit(1);
    }
}

// 启动递归下降下的语法制导翻译
int PraseWithRecursive() {
    // 初始化第一个单词符号
    scanner();
    char *result = A();
    printf("Result: %s\n", result);
    FILE *output_file = fopen("output.txt", "w");
    if (output_file == NULL) {
        printf("Error: Cannot open output file\n");
        return 1;
    }
    for (int i = 0; i < NXQ; i++) {
        fprintf(output_file, "(%s, %s, %s, %s)\n", pQuad[i].op, pQuad[i].arg1, pQuad[i].arg2, pQuad[i].result);
    }
    fclose(output_file);
    return 0;
}

