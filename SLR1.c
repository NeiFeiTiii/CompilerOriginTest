//
// Created by ddd on 2024/11/8.
//
#include "SLR1.h"

// 0: shift, 1: reduce, 2: accept, -1: error
Action parsingTable[16][8] = {
        // ID, PLUS, MINUS, MUL, DIV, LP, RP, END
        {{0, 5}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {0, 4}, {-1, -1}, {-1, -1}}, // state 0
        {{-1, -1}, {0, 6}, {0, 7}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {2, -1}}, // state 1
        {{-1, -1}, {1, 3}, {1, 3}, {0, 8}, {0, 9}, {-1, -1}, {1, 3}, {1, 3}}, // state 2
        {{-1, -1}, {1, 6}, {1, 6}, {1, 6}, {1, 6}, {-1, -1}, {1, 6}, {1, 6}}, // state 3
        {{0, 5}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {0, 4}, {-1, -1}, {-1, -1}}, // state 4
        {{-1, -1}, {1, 8}, {1, 8}, {1, 8}, {1, 8}, {-1, -1}, {1, 8}, {1, 8}}, // state 5
        {{0, 5}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {0, 4}, {-1, -1}, {-1, -1}}, // state 6
        {{0, 5}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {0, 4}, {-1, -1}, {-1, -1}}, // state 7
        {{0, 5}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {0, 4}, {-1, -1}, {-1, -1}}, // state 8
        {{0, 5}, {-1, -1}, {-1, -1}, {-1, -1}, {-1, -1}, {0, 4}, {-1, -1}, {-1, -1}}, // state 9
        {{-1, -1}, {0, 6}, {0, 7}, {-1, -1}, {-1, -1}, {-1, -1}, {0, 15}, {-1, -1}}, // state 10
        {{-1, -1}, {1, 1}, {1, 1}, {0, 8}, {0, 9}, {-1, -1}, {1, 1}, {1, 1}}, // state 11
        {{-1, -1}, {1, 2}, {1, 2}, {0, 8}, {0, 9}, {-1, -1}, {1, 2}, {1, 2}}, // state 12
        {{-1, -1}, {1, 4}, {1, 4}, {1, 4}, {1, 4}, {-1, -1}, {1, 4}, {1, 4}}, // state 13
        {{-1, -1}, {1, 5}, {1, 5}, {1, 5}, {1, 5}, {-1, -1}, {1, 5}, {1, 5}}, // state 14
        {{-1, -1}, {1, 7}, {1, 7}, {1, 7}, {1, 7}, {-1, -1}, {1, 7}, {1, 7}}  // state 15
};

int gotoTable[16][3] = {
        // E, T, F
        {1, 2, 3}, // state 0
        {-1, -1, -1}, // state 1
        {-1, -1, -1}, // state 2
        {-1, -1, -1}, // state 3
        {10, 2, 3}, // state 4
        {-1, -1, -1}, // state 5
        {-1, 11, 3}, // state 6
        {-1, 12, 3}, // state 7
        {-1, -1, 13}, // state 8
        {-1, -1, 14}, // state 9
        {-1, -1, -1}, // state 10
        {-1, -1, -1}, // state 11
        {-1, -1, -1}, // state 12
        {-1, -1, -1}, // state 13
        {-1, -1, -1}, // state 14
        {-1, -1, -1}  // state 15
};

void parse() {
    currentToken = 0;
    int stack[100];
    int top = 0;
    stack[top] = 0; // Initial state
    Token token = getNextToken();

    while (1) {
        int state = stack[top];
        int flag = token.type;
        if (flag == 14|| flag == 15 || flag == 18 || flag == 19 ) {
            flag = 0;
        }
        Action action = parsingTable[state][flag];

        printf("State: %d, Token: %s, Action: %d, Next State: %d\n", state, token_names[token.type], action.action, action.state);

        if (action.action == 0) { // Shift
            stack[++top] = action.state;
            token = getNextToken();
        } else if (action.action == 1) { // Reduce
            int nonTerminalIndex;
            int rule = action.state;
            switch (rule) {
                case 1: // E -> E + T
                case 2: // E -> E - T
                    top -= 3;
                    nonTerminalIndex = 0; // E
                    break;
                case 3: // E -> T
                    top -= 1;
                    nonTerminalIndex = 0; // E
                    break;
                case 4: // T -> T * F
                case 5: // T -> T / F
                    top -= 3;
                    nonTerminalIndex = 1; // T
                    break;
                case 6: // T -> F
                    top -= 1;
                    nonTerminalIndex = 1; // T
                    break;
                case 7: // F -> ( E )
                    top -= 3;
                    nonTerminalIndex = 2; // F
                    break;
                case 8: // F -> id
                    top -= 1;
                    nonTerminalIndex = 2; // F
                    break;
                default:
                    parseError("Unknown reduction rule");
                    return;
            }
            stack[++top] = gotoTable[stack[top]][nonTerminalIndex];
            printf("Reduced using rule %d, new state: %d\n", rule, stack[top]);
        } else if (action.action == 2) { // Accept
            printf("\nSLR1 success! Accept.\n----------------------\n");
            return;
        } else { // Error
            parseError("Syntax error");
        }
    }
}

int SLR1() {
    //readTokens("E:\\CODE\\ComplieTest2\\output.txt");
    parse();
    return 0;
}
