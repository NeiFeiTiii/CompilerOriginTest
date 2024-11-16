#include <stdio.h>
#include "Lexical.h"
#include "Recursive.h"
#include "SLR1.h"

int main() {
    // Lexical analysis
    int LexicalAnalysisFlag = Lexical();
    if (LexicalAnalysisFlag != 0) {
        printf("Lexical analysis failed\n");
        return LexicalAnalysisFlag;
    }
    // Grammatical analysis
    readTokens("E:\\CODE\\ComplieTest2\\output.txt");
    // （递归下降分析法）
    int RecursiveFlag = Recursive();
    if (RecursiveFlag != 0) {
        printf("Grammatical analysis failed\n");
        return RecursiveFlag;
    }
    // （SLR1分析法）
    int SLR1Flag = SLR1();
    if (SLR1Flag != 0) {
        printf("Grammatical analysis failed\n");
        return SLR1Flag;
    }
    return 0;
}