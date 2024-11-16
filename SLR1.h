//
// Created by ddd on 2024/11/8.
//
#ifndef COMPLIETEST2_SLR1_H
#define COMPLIETEST2_SLR1_H

#include "GramAnalysis.h"

typedef struct {
    int action; // 0: shift, 1: reduce, 2: accept, -1: error
    int state;
} Action;

void parse();
int SLR1();

#endif //COMPLIETEST2_SLR1_H
