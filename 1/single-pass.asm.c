#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LABELS 100
#define MAX_OPCODES 100
#define MAX_LINE_LENGTH 100

typedef struct {
    char label[20];
    int address;
} Symbol;

typedef struct {
    char mnemonic[10];
    char opcode[10];
} Opcode;

Symbol symbolTable[MAX_LABELS];
Opcode opcodeTable[MAX_OPCODES];
int symbolCount = 0;
int opcodeCount = 0;

void loadOpcodes(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening opcode file.\n");
        exit(1);
    }

    while (fscanf(fp, "%s %s", opcodeTable[opcodeCount].mnemonic, opcodeTable[opcodeCount].opcode) != EOF) {
        opcodeCount++;
    }
    fclose(fp);
}

char* getOpcode(char *mnemonic) {
    for (int i = 0; i < opcodeCount; i++) {
        if (strcmp(opcodeTable[i].mnemonic, mnemonic) == 0) {
            return opcodeTable[i].opcode;
        }
    }
    return NULL;
}

void addSymbol(char *label, int address) {
    strcpy(symbolTable[symbolCount].label, label);
    symbolTable[symbolCount].address = address;
    symbolCount++;
}

int searchSymbol(char *label) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].label, label) == 0) {
            return symbolTable[i].address;
        }
    }
    return -1;
}

void assemble(const char *sourceFile) {
    FILE *fp = fopen(sourceFile, "r");
    if (!fp) {
        printf("Error opening source file.\n");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    char label[20], mnemonic[10], operand[20];
    int locctr = 0;

    while (fgets(line, sizeof(line), fp)) {
        int numTokens = sscanf(line, "%s %s %s", label, mnemonic, operand);

        if (numTokens == 3) { // Label + Instruction + Operand
            if (searchSymbol(label) == -1) {
                addSymbol(label, locctr);
            }
            char *opcode = getOpcode(mnemonic);
            if (opcode) {
                locctr += 1; // Increment location counter (can be adjusted based on instruction size)
            }
        } else if (numTokens == 2) { // Instruction + Operand
            char *opcode = getOpcode(label);
            if (opcode) {
                locctr += 1; // Increment location counter (can be adjusted based on instruction size)
            }
        }
    }
    fclose(fp);
}

void printSymbolTable() {
    printf("Symbol Table:\n");
    printf("Label\tAddress\n");
    for (int i = 0; i < symbolCount; i++) {
        printf("%s\t%d\n", symbolTable[i].label, symbolTable[i].address);
    }
}

int main() {
    loadOpcodes("opcode.txt");
    assemble("source.asm");
    printSymbolTable();
    return 0;
}
