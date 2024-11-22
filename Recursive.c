//
// Created by ddd on 2024/11/8.
//
#include "Recursive.h"

Token tokens[100];
int tokenIndex = 0;
int currentToken = 0;

void parse_E() {
    parse_T();
    Token token = getNextToken();
    while (token.type == PLUS || token.type == MINUS) {
        parse_T();
        token = getNextToken();
    }
    retractToken();
}

void parse_T() {
    parse_F();
    Token token = getNextToken();
    while (token.type == MUL || token.type == DIV) {
        parse_F();
        token = getNextToken();
    }
    retractToken();
}

void parse_F() {
    Token token = getNextToken();
    if (token.type == ID || token.type == INT || token.type == REAL) {
        return;
    } else if (token.type == LP) {
        parse_E();
        token = getNextToken();
        if (token.type != RP) {
            parseError("Expected closing parenthesis");
        }
    } else {
        parseError("Unexpected token");
    }
}

int Recursive() {
    //readTokens("E:\\CODE\\ComplieTest2\\output.txt");
    printf("----Start Recursive analysis----\n");
    parse_E();
    Token token = getNextToken();
    if (token.type == END) {
        printf("RIGHT\n\n");
        printf("Recursive analysis success\n----------------------\n");
    } else {
        parseError("Unexpected token at the end");
        return -1;
    }
    return 0;
}