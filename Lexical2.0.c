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
    Token tempToken;
    tempToken.row = line_number;
    tempToken.col = column_number;
    strcpy(tempToken.value, token);

    if (strcmp(token, "for") == 0) {
        tempToken.type = FOR;
        out(tempToken);
        return FOR;
    }
    if (strcmp(token, "if") == 0) {
        tempToken.type = IF;
        out(tempToken);
        return IF;
    }
    if (strcmp(token, "else") == 0) {
        tempToken.type = ELSE;
        out(tempToken);
        return ELSE;
    }
    if (strcmp(token, "and") == 0) {
        tempToken.type = AND;
        out(tempToken);
        return AND;
    }
    if (strcmp(token, "or") == 0) {
        tempToken.type = OR;
        out(tempToken);
        return OR;
    }
    if (strcmp(token, "not") == 0) {
        tempToken.type = NOT;
        out(tempToken);
        return NOT;
    }
    tempToken.type = ID;
    return ID;
}

void out(Token token) {
    output_file = fopen("Lex.txt", "a"); // Open in append mode
    fprintf(output_file, "(%s, %s)\n", token_names[token.type], token.value);
    tokens[currentToken] = token;
    currentToken ++;
    fclose(output_file); // Close the file after writing
}

void report_error(const char *error_type) {
    printf( "Error: %s, in line %d, column %d\n", error_type, line_number,column_number);
}

Token getNextToken() {
    static FILE *fp = NULL;
    static int initialized = 0;
    static long file_offset = 0;
    if (!initialized) {
        fp = fopen("input.txt", "r");
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
            out(token);
            free(TOKEN);
            file_offset = ftell(fp);
            return token;
        }
        fseek(fp, -1, SEEK_CUR);
        ch = fgetc(fp);

        if (ch == '#') {
            while (ch != '\n') {
                ch = fgetc(fp);
                column_number++;
            }
            continue;
        }
        if (isalpha(ch) || ch == '_') {  // ID
            TOKEN[0] = ch;
            column_number++;
            i = 1;
            ch = fgetc(fp);
            if (ch == EOF) {
                fseek(fp, 0, SEEK_END);
                file_offset = ftell(fp);
            }
            else{
                while (isalnum(ch) || ch == '_') {
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                    column_number++;
                    if (ch == EOF) {
                        fseek(fp, 0, SEEK_END);
                        file_offset = ftell(fp);
                    }
                }
                if (ch != EOF) {
                    fseek(fp, -1, SEEK_CUR);
                    ch = fgetc(fp);
                    column_number++;
                }
            }
            TOKEN[i] = '\0';

            file_offset = ftell(fp);
            c = lookup(TOKEN);      // 查找保留字，鉴定是否是特殊的保留字
            if (c == ID) {
                fseek(fp, -1, SEEK_CUR);
                file_offset = ftell(fp);
                Token token = {ID, "", line_number, column_number};
                strcpy(token.value, TOKEN);
                out(token);
                free(TOKEN);
                return token;
            }
        }
        if (isdigit(ch)) {
            TOKEN[0] = ch;
            ch = fgetc(fp);
            column_number++;
            i = 1;

            int is_real = 0;
            int is_octal = (TOKEN[0] == '0');
            int is_hex = 0;
            if (ch == EOF) {
                fseek(fp, 0, SEEK_END);
                file_offset = ftell(fp);
            } else {
                if (is_octal && (ch == 'x' || ch == 'X')) {     // 16进制
                    is_octal = 0;
                    is_hex = 1;
                    TOKEN[i] = ch;
                    i++;
                    column_number++;
                    ch = fgetc(fp);
                    if (ch == EOF) {
                        fseek(fp, 0, SEEK_END);
                        file_offset = ftell(fp);
                    } else {
                        while ((ch <= 70 && ch >= 65) || (ch <= 102 && ch >= 97) || isdigit(ch)) {
                            TOKEN[i] = ch;
                            i++;
                            ch = fgetc(fp);
                            column_number++;
                        }
                        if (ch != EOF)
                            fseek(fp, -1, SEEK_CUR);
                    }
                }
                if (is_octal && ch != 'x' && ch != 'X') {       // 8进制
                    is_octal = 0;
                    while (isdigit(ch)) {
                        TOKEN[i] = ch;
                        i++;
                        ch = fgetc(fp);
                        column_number++;
                        is_octal = 1;
                    }
                    if (ch == EOF) {
                        fseek(fp, 0, SEEK_END);
                        file_offset = ftell(fp);
                    } else
                        fseek(fp, -1, SEEK_CUR);
                }
                while (isdigit(ch)) {       // 10进制
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                    column_number++;
                }
                if (ch == EOF) {
                    fseek(fp, 0, SEEK_END);
                    file_offset = ftell(fp);
                } else {
                    if (ch == '.') {
                        TOKEN[i] = ch;
                        is_real = 1;
                        i++;
                        ch = fgetc(fp);
                        column_number++;
                        while (isdigit(ch)) {
                            TOKEN[i] = ch;
                            i++;
                            ch = fgetc(fp);
                            column_number++;
                        }
                        if (ch == EOF) {
                            fseek(fp, 0, SEEK_END);
                            file_offset = ftell(fp);
                        } else
                            fseek(fp, -1, SEEK_CUR);
                    }
                    if (ch == 'e' || ch == 'E') {
                        is_real = 1;
                        TOKEN[i] = ch;
                        i++;
                        ch = fgetc(fp);
                        column_number++;

                        if (ch == EOF) {
                            fseek(fp, 0, SEEK_END);
                            file_offset = ftell(fp);
                            ErrorPrint("Real number not complete,In the end of file");
                        } else {
                            if (ch == '+' || ch == '-') {
                                TOKEN[i] = ch;
                                i++;
                                ch = fgetc(fp);
                                column_number++;
                                if (ch == EOF) {
                                    fseek(fp, 0, SEEK_END);
                                    file_offset = ftell(fp);
                                    ErrorPrint("Real number not complete,In the end of file");
                                }
                                while (isdigit(ch)) {
                                    TOKEN[i] = ch;
                                    i++;
                                    ch = fgetc(fp);
                                    column_number++;
                                }
                                if (ch == EOF) {
                                    fseek(fp, 0, SEEK_END);
                                    file_offset = ftell(fp);
                                } else
                                    fseek(fp, -1, SEEK_CUR);
                            }
                            else if(isdigit(ch)) {
                                while (isdigit(ch)) {
                                    TOKEN[i] = ch;
                                    i++;
                                    ch = fgetc(fp);
                                    column_number++;
                                }
                                if (ch == EOF) {
                                    fseek(fp, 0, SEEK_END);
                                    file_offset = ftell(fp);
                                } else
                                    fseek(fp, -1, SEEK_CUR);
                            }
                        }
                    }
                    if  (ch != EOF)
                        fseek(fp, -1, SEEK_CUR);
                }
            }
            TOKEN[i] = '\0';
            if (is_real) {
                Token token = {REAL, "", line_number, column_number};
                strcpy(token.value, TOKEN);
                out(token);
                free(TOKEN);
                file_offset = ftell(fp)+1;
                return token;
            } else if (is_hex) {
                Token token = {HEX, "", line_number, column_number};
                strcpy(token.value, TOKEN);
                out(token);
                free(TOKEN);
                file_offset = ftell(fp);
                return token;
            } else if (is_octal) {
                Token token = {OCTAL, "", line_number, column_number};
                strcpy(token.value, TOKEN);
                out(token);
                free(TOKEN);
                file_offset = ftell(fp);
                return token;
            } else {
                Token token = {INT, "", line_number, column_number};
                strcpy(token.value, TOKEN);
                out(token);
                free(TOKEN);
                file_offset = ftell(fp);
                return token;
            }
        }
        if (ch == '"') {
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
                    out(token);
                    free(TOKEN);
                    return token;
                }
            }
            TOKEN[i] = '\0';
            Token token = {STRING, "", line_number, column_number};
            strcpy(token.value, TOKEN);
            out(token);
            free(TOKEN);
            file_offset = ftell(fp);
            return token;
        }
        if (ch == '\'') {        // 字符
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
            out(token);
            free(TOKEN);
            file_offset = ftell(fp);
            return token;
        }
        if (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']' || ch == ';' || ch == ',') {
            Token token;
            if (ch == '(') token = (Token){LP, "(", line_number, column_number};
            else if (ch == ')') token = (Token){RP, ")", line_number, column_number};
            else if (ch == '{') token = (Token){BRACKET, "{", line_number, column_number};
            else if (ch == '}') token = (Token){BRACKET, "}", line_number, column_number};
            else if (ch == '[') token = (Token){BRACKET, "[", line_number, column_number};
            else if (ch == ']') token = (Token){BRACKET, "]", line_number, column_number};
            else if (ch == ';') token = (Token){DOT, ";", line_number, column_number};
            else token = (Token){DOT, ",", line_number, column_number};
            out(token);
            column_number++;
            free(TOKEN);
            file_offset = ftell(fp);
            return token;
        }
        if (ch == '=' || ch == '!' || ch == '<' || ch == '>' || ch == ':') {
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
                    out(token);
                    free(TOKEN);
                    exit(1);
                }
            }
            out(token);
            free(TOKEN);
            file_offset = ftell(fp);
            return token;
        }
        if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            Token token;
            if (ch == '+') token = (Token){PLUS, " ", line_number, column_number};
            else if (ch == '-') token = (Token){MINUS, " ", line_number, column_number};
            else if (ch == '*') token = (Token){MUL, " ", line_number, column_number};
            else token = (Token){DIV, " ", line_number, column_number};
            out(token);
            column_number++;
            free(TOKEN);
            file_offset = ftell(fp);
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
        else {
            report_error("Unknown character");
            Token token = {UNKNOWN, " ", line_number, column_number};
            out(token);
            free(TOKEN);
            exit(1);
        }
    }
}
