#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PraseWithRecursive.h"

QUATERNION pQuad[MAXLENGTH];
int NXQ = 0;    /*全局变量NXQ用于指示所要产生的下一个四元式的编号*/
int NXTemp = 1; //Tn的n值

WORD word;

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
    char opp[2], *E1_place, *E2_place, *Temp_place;
    E1_place = T();
    while (word.Class == PLUS || word.Class == MINUS) {
        if (word.Class == PLUS) {
            strcpy(opp, "+");
        } else {
            strcpy(opp, "-");
        }
        //sprintf(opp, "%s", word.Value.Val4);
        scanner();
        E2_place = T();
        Temp_place = NewTemp();
        GEN(opp, E1_place, E2_place, Temp_place);
        E1_place = Temp_place;
    }
    return E1_place;
}

char *T(void) {
    char opp[2], *T1_place, *T2_place, *Temp_place;
    T1_place = F();
    while (word.Class == MUL || word.Class == DIV) {
        if (word.Class == MUL) {
            strcpy(opp, "*");
        } else {
            strcpy(opp, "/");
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
        place = strdup(word.Value.Val1);
        scanner();
        return place;
    } else if (word.Class == LP) { // 左括号
        scanner();
        place = E();
        if (word.Class == RP) { // 右括号
            scanner();
            return place;
        } else {
            printf("ERROR: dont have')'\n");
            exit(-1);
        }
    } else {
        printf("ERROR: Not A Valid Prase\n");
        exit(-1);
    }
}

void scanner() {
    static int tokenIndex = 0;
    if (tokenIndex >= currentToken) {
        printf("Error: No more tokens\n");
        exit(1);
    }
    word.Class = tokens[tokenIndex].type;
    switch (word.Class) {
        case ID:
        case INT:
        case REAL:
            strcpy(word.Value.Val1, tokens[tokenIndex].value);
            break;
        case PLUS:
        case MINUS:
        case MUL:
        case DIV:
        case LP:
        case RP:
            strcpy(word.Value.Val4, tokens[tokenIndex].value);
            //word.Value.Val4 = tokens[tokenIndex].value;
            break;
        case END:
            printf("End of tokens\n");
            return ;
        default:
            printf("Error: Unknown token type\n");
            exit(1);
    }
    tokenIndex++;
}
// 启动递归下降下的语法制导翻译
int PraseWithRecursive() {
    // 初始化第一个单词符号
    scanner();
    char *result = E();
    printf("Result: %s\n", result);
    for (int i = 0; i < NXQ; i++) {
        printf("(%s, %s, %s, %s)\n", pQuad[i].op, pQuad[i].arg1, pQuad[i].arg2, pQuad[i].result);
    }

    return 0;
}