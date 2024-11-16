//
// Created by ddd on 2024/11/8.
//
#include "GramAnalysis.h"

void parseError(const char *message) {
    printf("ERROR: %s at token %d (%s,%s),%d:%d\n", message, currentToken, token_names[tokens[currentToken-1].type],tokens[currentToken-1].value,tokens[currentToken-1].row, tokens[currentToken-1].col);
    exit(1);
}

Token getNextToken() {
    return tokens[currentToken++];
}

void retractToken() {
    currentToken--;
}

void readTokens(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Cannot open token file\n");
        exit(1);
    }
    char typeStr[MAX_TOKEN_LENGTH];
    char value[MAX_TOKEN_LENGTH];
    int row = 1;
    int col = 1;
    while (1) {
        fscanf(file, "(%[^,],%[^)])", typeStr, value);
        fgetc(file); // 读取并丢弃右括号
        if (strcmp(typeStr, "ENTER") == 0) {
            row++;
            col = 1;
            continue;
        }
        for (int i = 0; i < TOKEN_NAMES_SIZE; i++) {
            if (strcmp(typeStr, token_names[i]) == 0) {
                tokens[tokenIndex].type = i;
                break;
            }
        }
        strcpy(tokens[tokenIndex].value, value);
        tokens[tokenIndex].row = row;
        tokens[tokenIndex].col = col;
        col ++;
        // 将字符串类型转换为枚举类型
        printf("typeStr: %s\n", typeStr);
        printf("value: %s\n", tokens[tokenIndex].value);

        if (strcmp(typeStr, "RP") == 0) {
            fgetc(file);
        }
        if(strcmp(typeStr, "END") == 0) {
            break;
        }
        tokenIndex++;
        if (tokenIndex >= 100) {
            printf("Error: Too many tokens\n");
            exit(1);
        }
    }
    tokens[tokenIndex].type = END;
    fclose(file);
}