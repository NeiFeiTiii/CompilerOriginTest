// 代码内容来自于我一年前写的题目解答：表达式转换，内容有微调
// Created by ddd on 2024/11/22.
//  取消了main，改为本项目的main.c调用

#include "ToPoland.h"
//
// Created by DDD on 2023/10/24.
//
#include <stdio.h>
#include<string.h>

#define MAXSIZE 50

int flag = 0;   //打空格问题

typedef struct{
    char data[MAXSIZE];
    int top;
    int bracket;
    char num[MAXSIZE];
    int NumTop;
    int NumFront;
}Stack;

void InitStack(Stack *S){   //构造新栈
    S->top = -1;
    S->NumTop = 0;
    S->NumFront = 0;
    S->bracket = 0;
}

char SymJudge(char ch,char fch){
    if((ch == '*' || ch == '/') && (fch == '+' || fch == '-'))
        return '<';
    if((ch == '+' || ch == '-') && (fch == '*' || fch == '/'))
        return '>';
    if((ch == '*' || ch == '/') && (fch == '*' || fch == '/'))
        return '=';
    if((ch == '+' || ch == '-') && (fch == '+' || fch == '-'))
        return '=';
    return '=';
}

void Pop(Stack *S){                                  //运算符出栈
    if(S->top == -1)
        printf("栈为空，无法出栈");
    char ele;
    ele = S->data[S->top];
    S->top--;
    if(ele != '(')
        printf("%c ",ele);
}


void Push(Stack *S, char e){                                             //入栈
    if((e >= '(' && e <= '+')||(e == '-')||(e == '/')){                 //运算符入栈,去掉小数点判断
        while(S->NumFront < S->NumTop){
            printf("%c",S->num[S->NumFront]);
            S->NumFront++;
        }
        if(flag == 1) {
            printf(" ");
            flag = 0;
        }

        if(e >= '*') {
            if (S->top == -1 || S->bracket == 1){                       //有括号时
                if((S->NumTop == 0) || ((S->data[S->top] == '(') &&(S->bracket != 2)))    {
                    if(e == '+')
                        return;
                    else if(e == '-') {
                        S->num[S->NumTop] = e;
                        S->NumTop++;
                        return;
                    }
                }
                else{
                    S->top++;
                    S->data[S->top] = e;
                }
                return;
            }

            if (S->top >= 0) {
                switch (SymJudge(e, S->data[S->top])) {
                    case '<': {
                        S->top++;
                        S->data[S->top] = e;
                        break;
                    }
                    case '>':{
                        Pop(S);
                        while (S->top > -1 &&
                               (SymJudge(e, S->data[S->top]) == '>' || SymJudge(e, S->data[S->top]) == '=')
                               && S->data[S->top] !='(') {
                            Pop(S);
                        }
                        S->top++;
                        S->data[S->top] = e;
                        break;
                    }
                    case '=': {
                        if(S->bracket == 1) {
                            S->num[S->NumTop] = e;
                            S->NumTop++;
                        }
                        else{
                            S->top++;
                            S->data[S->top] = e;
                        }
                        break;
                    }
                }
            }
        }
        if(e <=')'){
            if(e == '('){
                S->top++;
                S->data[S->top] = e;
                S->bracket = 1;
            }
            if(e == ')'){
                while (S->data[S->top] != '('){
                    Pop(S);
                }
                S->top--;
                S->bracket = 0;
            }
        }
    }

    if((e >= '1' && e <= '9' )||(e == '.')) {          //数字栈
        S->num[S->NumTop] = e;
        S->NumTop++;
        flag = 1;
        S->bracket = 2;
        return;
    }
}

void Print(Stack* S){  //置空读取
    while(S->top != -1) {
        if(S->top > 0)
            printf("%c ", S->data[S->top]);
        else printf("%c", S->data[S->top]);
        S->top--;
    }
}

int Poland(){
    Stack stk;
    InitStack(&stk);
    char s[21];
    scanf("%s",s);
    int l = strlen(s);
    int size = 0;
    for(int i = 0;i<l;i++){
        Push(&stk,s[i]);
        size++;
    }
    while (stk.NumTop != stk.NumFront) {
        printf("%c", stk.num[stk.NumFront]);
        stk.NumFront++;
    }
    if(stk.top != -1 && flag != 0)
        printf(" ");
    Print(&stk);
    return 0;
}