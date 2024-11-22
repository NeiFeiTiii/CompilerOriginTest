//
// Created by ddd on 2024/11/22.
//

#include "Lexical.h"
#include "PraseWithRecursive.h"

const char* token_names[] = {
        "ID","PLUS", "MINUS", "MUL", "DIV","LP","RP","END",
        "FOR", "IF", "ELSE", "AND", "OR", "NOT",
        "INT", "REAL","STRING","CHAR","OCTAL","HEX",
        "EQ", "NE", "LT", "GT", "LE", "GE",
        "IS", "BRACKET","DOT",
        "UNKNOWN", "ENTER"
};

Token tokens[100];
int currentToken = 0;
char *TOKEN = NULL;
FILE *output_file = NULL;
int line_number = 1;
int column_number = 0;

int lookup(char *token) {
    if (strcmp(token, "for") == 0){
        out(FOR, "",line_number,column_number);
        return FOR;
    }
    if (strcmp(token, "if") == 0) {
        out(IF, "",line_number,column_number);
        return IF;
    }
    if (strcmp(token, "else") == 0) {
        out(ELSE, "",line_number,column_number);
        return ELSE;
    }

    if (strcmp(token, "and") == 0)
    {
        out(AND, "",line_number,column_number);
        return AND;
    }
    if (strcmp(token, "or") == 0) {
        out(OR, "",line_number,column_number);
        return OR;
    }
    if (strcmp(token, "not") == 0) {
        out(NOT, "",line_number,column_number);
        return NOT;
    }
    return ID;
}

void out(int code, char *token,int line,int column) {
    fprintf(output_file, "(%s, %s)\n", token_names[code], token);
    tokens[currentToken].type = code;
    strcpy(tokens[currentToken].value, token);
    tokens[currentToken].row = line;
    tokens[currentToken].col = column;
    currentToken ++;

}

void report_error(const char *error_type) {
    fprintf(output_file, "Error: %s, in line %d, column %d\n", error_type, line_number,column_number);
}

Token getNextToken() {
    static FILE *fp = NULL;
    static int initialized = 0;
    static long file_offset = 0;
    if (!initialized) {
        fp = fopen("E:\\CODE\\CompileTestEnd\\input.txt", "r");
        if (fp == NULL) {
            printf("Error: Cannot open source file\n");
            exit(1);
        }
        initialized = 1;
    }
    fseek(fp, file_offset, SEEK_SET);

    char ch;
    int i, c;
    TOKEN = (char *)malloc(MAX_TOKEN_LENGTH * sizeof(char));
    if (TOKEN == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }

    ch = fgetc(fp);
    while (1) {
        if (ch == EOF) {
            Token token = {END, " ", -1, -1};
            free(TOKEN);
            return token;
        }
        fseek(fp, -1, SEEK_CUR);
        ch = fgetc(fp);
        if (ch == EOF) {
            Token token = {END, " ", -1, -1};
            free(TOKEN);
            return token;
        }
        if (isspace(ch)) {
            if (ch == '\n') {
                line_number++;
                column_number = 0;
            } else {
                column_number++;
            }
            ch = fgetc(fp);
            continue;
        }
        if (ch == '#') {
            while (ch != '\n') {
                ch = fgetc(fp);
                column_number++;
            }
            continue;
        }
        if (isalpha(ch)) {
            TOKEN[0] = ch;
            ch = fgetc(fp);
            column_number++;
            i = 1;
            while (isalnum(ch)) {
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
                column_number++;
            }
            TOKEN[i] = '\0';
            fseek(fp, -1, SEEK_CUR);
            column_number--;
            c = lookup(TOKEN);
            if (c == ID) {
                Token token = {ID, "", line_number, column_number};
                strcpy(token.value, TOKEN);
                free(TOKEN);
                file_offset = ftell(fp);
                return token;
            }
        } else if (isdigit(ch)) {
            TOKEN[0] = ch;
            ch = fgetc(fp);
            column_number++;
            i = 1;
            int is_real = 0;
            int is_octal = (TOKEN[0] == '0');
            int is_hex = 0;
            if (is_octal && (ch == 'x' || ch == 'X')) {
                is_octal = 0;
                is_hex = 1;
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
                column_number++;
                while ((ch <= 70 && ch >= 65) || (ch <= 102 && ch >= 97) || isdigit(ch)) {
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                    column_number++;
                }
            }
            if (is_octal && ch != 'x' && ch != 'X') {
                while (isdigit(ch)) {
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                    column_number++;
                }
            }
            while (isdigit(ch)) {
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
                column_number++;
            }
            if (ch == '.') {
                TOKEN[i] = ch;
                is_real = 1;
                i++;
                ch = fgetc(fp);
                while (isdigit(ch)) {
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                    column_number++;
                }
            }
            if (ch == 'e' || ch == 'E') {
                is_real = 1;
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
                column_number++;
                if (ch == '+' || ch == '-') {
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                    column_number++;
                    while (isdigit(ch)) {
                        TOKEN[i] = ch;
                        i++;
                        ch = fgetc(fp);
                        column_number++;
                    }
                }
                while (isdigit(ch)) {
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                    column_number++;
                }
            }
            TOKEN[i] = '\0';
            fseek(fp, -1, SEEK_CUR);
            if (is_real) {
                Token token = {REAL, "", line_number, column_number};
                strcpy(token.value, TOKEN);
                free(TOKEN);
                file_offset = ftell(fp);
                return token;
            } else if (is_hex) {
                Token token = {HEX, "", line_number, column_number};
                strcpy(token.value, TOKEN);
                free(TOKEN);
                file_offset = ftell(fp);
                return token;
            } else if (is_octal) {
                Token token = {OCTAL, "", line_number, column_number};
                strcpy(token.value, TOKEN);
                free(TOKEN);
                file_offset = ftell(fp);
                return token;
            } else {
                Token token = {INT, "", line_number, column_number};
                strcpy(token.value, TOKEN);
                free(TOKEN);
                file_offset = ftell(fp);
                return token;
            }
        } else if (ch == '"') {
            ch = fgetc(fp);
            column_number++;
            i = 0;
            while (ch != '"') {
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
                column_number++;
                if (ch == EOF) {
                    Token token = {END, " ", -1, -1};
                    report_error("String not closed");
                    free(TOKEN);
                    return token;
                }
            }
            TOKEN[i] = '\0';
            Token token = {STRING, "", line_number, column_number};
            strcpy(token.value, TOKEN);
            free(TOKEN);
            file_offset = ftell(fp);
            return token;
        } else if (ch == '\'') {
            ch = fgetc(fp);
            column_number++;
            i = 0;
            while (ch != '\'') {
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
                column_number++;
            }
            TOKEN[i] = '\0';
            Token token = {CHAR, "", line_number, column_number};
            strcpy(token.value, TOKEN);
            free(TOKEN);
            file_offset = ftell(fp);
            return token;
        } else if (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']' || ch == ';' || ch == ',') {
            Token token;
            if (ch == '(') token = (Token){LP, "(", line_number, column_number};
            else if (ch == ')') token = (Token){RP, ")", line_number, column_number};
            else if (ch == '{') token = (Token){BRACKET, "{", line_number, column_number};
            else if (ch == '}') token = (Token){BRACKET, "}", line_number, column_number};
            else if (ch == '[') token = (Token){BRACKET, "[", line_number, column_number};
            else if (ch == ']') token = (Token){BRACKET, "]", line_number, column_number};
            else if (ch == ';') token = (Token){DOT, ";", line_number, column_number};
            else token = (Token){DOT, ",", line_number, column_number};
            column_number++;
            free(TOKEN);
            file_offset = ftell(fp);
            return token;
        } else if (ch == '=' || ch == '!' || ch == '<' || ch == '>' || ch == ':') {
            char next_ch = fgetc(fp);
            column_number++;
            Token token;
            if (ch == '=' && next_ch == '=') token = (Token){EQ, "", line_number, column_number};
            else if (ch == '!' && next_ch == '=') token = (Token){NE, "", line_number, column_number};
            else if (ch == '<' && next_ch == '=') token = (Token){LE, "", line_number, column_number};
            else if (ch == '>' && next_ch == '=') token = (Token){GE, "", line_number, column_number};
            else if (ch == ':' && next_ch == '=') token = (Token){IS, "", line_number, column_number};
            else {
                fseek(fp, -1, SEEK_CUR);
                column_number--;
                if (ch == '=') token = (Token){IS, " ", line_number, column_number};
                else if (ch == '!') token = (Token){NOT, " ", line_number, column_number};
                else if (ch == '<') token = (Token){LT, " ", line_number, column_number};
                else if (ch == '>') token = (Token){GT, " ", line_number, column_number};
                else {
                    report_error("Unknown operator");
                    free(TOKEN);
                    exit(1);
                }
            }
            free(TOKEN);
            file_offset = ftell(fp);
            return token;
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            Token token;
            if (ch == '+') token = (Token){PLUS, " ", line_number, column_number};
            else if (ch == '-') token = (Token){MINUS, " ", line_number, column_number};
            else if (ch == '*') token = (Token){MUL, " ", line_number, column_number};
            else token = (Token){DIV, " ", line_number, column_number};
            column_number++;
            free(TOKEN);
            file_offset = ftell(fp);
            return token;
        } else {
            report_error("Unknown character");
            free(TOKEN);
            exit(1);
        }
        ch = fgetc(fp);
        column_number++;
    }
}

/*
int Lexical2(){
    FILE *fp = fopen("E:\\CODE\\CompileTestEnd\\input.txt", "r");
    if (fp == NULL) {
        printf("Error: Cannot open source file\n");
        return 99;
    }
    output_file = fopen("E:\\CODE\\CompileTestEnd\\output.txt", "w");
    if (output_file == NULL) {
        printf("Error: Cannot open output file\n");
        return 98;
    }
    scanner_example(fp);
    fclose(fp);
    fclose(output_file);
    return 0;
}*/
