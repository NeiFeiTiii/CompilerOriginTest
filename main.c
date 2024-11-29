#include "PraseWithRecursive.h"

void clearFile(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        fclose(file);
    }
}

int main() {
    clearFile("Lex.txt");
    clearFile("output.txt");
    PraseWithRecursive();
    return 0;
}
