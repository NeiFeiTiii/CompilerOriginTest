//
// Created by ddd on 2024/11/8.
//
#include "Lexical.h"

const char* token_names[] = {
        "ID","PLUS", "MINUS", "MUL", "DIV","LP","RP","END",
        "FOR", "IF", "ELSE", "AND", "OR", "NOT",
        "INT", "REAL","STRING","CHAR","OCTAL","HEX",
        "EQ", "NE", "LT", "GT", "LE", "GE",
        "IS", "BRACKET","DOT",
        "UNKNOWN", "ENTER"
};

int line_number = 1;
int column_number = 0;

char *TOKEN;
FILE *output_file;

int lookup(char *token) {
    if (strcmp(token, "for") == 0){
        out(FOR, "");
        return FOR;
    }
    if (strcmp(token, "if") == 0) {
        out(IF, "");
        return IF;
    }
    if (strcmp(token, "else") == 0) {
        out(ELSE, "");
        return ELSE;
    }

    if (strcmp(token, "and") == 0)
    {
        out(AND, "");
        return AND;
    }
    if (strcmp(token, "or") == 0) {
        out(OR, "");
        return OR;
    }
    if (strcmp(token, "not") == 0) {
        out(NOT, "");
        return NOT;
    }
    return ID;
}

void out(int code, char *token) {
    fprintf(output_file, "(%s, %s)\n", token_names[code], token);
}

void report_error(const char *error_type) {
    fprintf(output_file, "Error: %s, in line %d, column %d\n", error_type, line_number,column_number);
}

void scanner_example(FILE *fp) {
    char ch;
    int i, c;
    TOKEN = (char *)malloc(MAX_TOKEN_LENGTH * sizeof(char));
    if (TOKEN == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return;
    }
    ch = fgetc(fp);
    while (1) {
        if (ch == EOF) {
            out(END, " ");
            free(TOKEN);
            return;
        }
        fseek(fp, -1, SEEK_CUR);
        ch = fgetc(fp);
        if (ch == EOF) {
            out(END, " ");
            free(TOKEN);
            return;
        }
        if (isspace(ch)) {  /* jump the space characters */
            if (ch == '\n') {
                out(ENTER, " ");
                line_number++;
                column_number = 0;
            } else {
                column_number++;
            }
            ch = fgetc(fp);
            continue;
        }
        if (ch == '#') {        /* jump the comment */
            while (ch != '\n') {
                ch = fgetc(fp);
                column_number++;
            }
            continue;
        }
        if (isalpha(ch)) {          /* it must be an identifier or keyword */
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
            fseek(fp, -1, SEEK_CUR);  /* retract */
            column_number--;
            c = lookup(TOKEN);
            if(c == ID) {
                out(ID, TOKEN);
            }
        } else if (isdigit(ch)) {       /* it must be a number */
            TOKEN[0] = ch;
            ch = fgetc(fp);
            column_number++;
            i = 1;
            int is_real = 0;
            int is_octal = (TOKEN[0] == '0');
            int is_hex = 0;
            if (is_octal && (ch == 'x' || ch == 'X')) { // hex
                is_octal = 0;
                is_hex = 1;
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
                column_number++;
                while ((ch <= 70 && ch>= 65) || (ch<=102 && ch>= 97) ||isdigit(ch)) {
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                    column_number++;
                }
            }
            if (is_octal && ch != 'x' && ch != 'X') {   // octal
                while (isdigit(ch)) {
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                    column_number++;
                }
            }
            while(isdigit(ch)){
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
                column_number++;
            }
            if (ch == '.' ){
                TOKEN[i] = ch;
                is_real = 1;
                i++;
                ch = fgetc(fp);
                while(isdigit(ch)){
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                    column_number++;
                }
            }
            if(ch == 'e' || ch == 'E'){
                is_real = 1;
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
                column_number++;
                if(ch == '+' || ch == '-'){
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                    column_number++;
                    while(isdigit(ch)){
                        TOKEN[i] = ch;
                        i++;
                        ch = fgetc(fp);
                        column_number++;
                    }
                }
                while(isdigit(ch)){
                    TOKEN[i] = ch;
                    i++;
                    ch = fgetc(fp);
                    column_number++;
                }
            }
            TOKEN[i] = '\0';
            fseek(fp, -1, SEEK_CUR);  /* retract */
            if (is_real) {
                out(REAL, TOKEN);
                /*double value = strtod(TOKEN, NULL);
                char buffer[MAX_TOKEN_LENGTH];
                sprintf(buffer, "%g", value);
                out(REAL, buffer);*/
            } else if (is_hex) {
                out(HEX, TOKEN);
            } else if (is_octal) {
                out(OCTAL, TOKEN);
            } else {
                out(INT, TOKEN);
            }
        } else if (ch == '"') {  /* it must be a string */
            ch = fgetc(fp);
            column_number++;
            i = 0;
            while (ch != '"') {
                TOKEN[i] = ch;
                i++;
                ch = fgetc(fp);
                column_number++;
                if (ch == EOF) {
                    out(END, " ");
                    report_error("String not closed");
                    free(TOKEN);
                    return;
                }
            }
            TOKEN[i] = '\0';
            out(STRING, TOKEN);
        } else if (ch == '\'') {  /* it must be a character */
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
            out(CHAR, TOKEN);
        } else if (ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']' || ch == ';' || ch == ',') {  /* it must be a separator */
            if (ch == '(') out(LP, "(");
            else if (ch == ')') out(RP, ")");
            else if (ch == '{') out(BRACKET, "{");
            else if (ch == '}') out(BRACKET, "}");
            else if (ch == '[') out(BRACKET, "[");
            else if (ch == ']') out(BRACKET, "]");
            else if (ch == ';') out(DOT, ";");
            else out(DOT, ","); //ch == ','
            column_number++;
        }
        else if (ch == '=' || ch == '!' || ch == '<' || ch == '>'|| ch == ':') {  /* it must be a relational operator or assignment */
            char next_ch = fgetc(fp);
            column_number++;
            if (ch == '=' && next_ch == '=') {
                out(EQ, "");
            } else if (ch == '!' && next_ch == '=') {
                out(NE, "");
            } else if (ch == '<' && next_ch == '=') {
                out(LE, "");
            } else if (ch == '>' && next_ch == '=') {
                out(GE, "");
            } else if (ch == ':' && next_ch == '=') {
                out(IS, "");
            } else {
                fseek(fp, -1, SEEK_CUR);  /* retract */
                column_number--;
                if (ch == '=') out(IS, " ");
                else if (ch == '!') out(NOT, " ");
                else if (ch == '<') out(LT, " ");
                else if (ch == '>') out(GT, " ");
                else report_error("Unknown operator");
            }
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {  /* it must be an arithmetic operator */
            if (ch == '+') out(PLUS, " ");
            else if (ch == '-') out(MINUS, " ");
            else if (ch == '*') out(MUL, " ");
            else out(DIV, " "); // ch == '/'
            column_number++;
        } else {
            report_error("Unknown character");
        }
        if (ch == EOF) {
            out(END, "");
            free(TOKEN);
            return;
        }
        ch = fgetc(fp);
        column_number++;
    }
}

int Lexical(){
    FILE *fp = fopen("E:\\CODE\\ComplieTest2\\input.txt", "r");
    if (fp == NULL) {
        printf("Error: Cannot open source file\n");
        return 99;
    }
    output_file = fopen("E:\\CODE\\ComplieTest2\\output.txt", "w");
    if (output_file == NULL) {
        printf("Error: Cannot open output file\n");
        return 98;
    }
    scanner_example(fp);
    fclose(fp);
    fclose(output_file);
    return 0;
}
