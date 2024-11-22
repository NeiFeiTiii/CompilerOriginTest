#include <stdio.h>
#include "Lexical.h"
#include "Recursive.h"
#include "SLR1.h"
#include "ToPoland.h"
#include "PraseWithRecursive.h"

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
/*    int PolandFlag = Poland();
    if (PolandFlag != 0) {
        printf("Poland analysis failed\n");
        return PolandFlag;
    }*/
    // （递归下降分析 + 语法制导四元式输出）
    int parseExpressionFlag = PraseWithRecursive();
    if (parseExpressionFlag != 0) {
        printf("Expression parsing failed\n");
        return parseExpressionFlag;
    }
    return 0;
}