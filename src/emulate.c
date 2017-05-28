#include "emulate.h"

void initialize(ARM11 *arm11) {
    int i;
    for (i = 0; i < MEMORY_SIZE; i++) {
        arm11->memory[i] = DEFAULT_VALUE;
    }
    for (i = 0; i < REGISTER_SIZE; i++) {
        arm11->registers[i] = DEFAULT_VALUE;
    }
}

uint32_t littleToBig(int i, ARM11 *arm11) {
    uint32_t value = 0;
    int j;
    for (j = BYTE_NUMBER; j > 0; j--) {
        value <<= BYTE_VALUE;
        value += arm11->memory[i + j];
    }
    return value;
}

uint32_t getMemoryValue(int i, ARM11 *arm11) {
    uint32_t value = 0;
    int j;
    for (j = 0; j < BYTE_NUMBER; j++) {
        value <<= BYTE_VALUE;
        value += arm11->memory[i + j];
    }
    return value;
}

void print(ARM11 *arm11) {
    int i;
    printf("%s\n", "Registers:");
    for (i = 0; i < GP_REGISTERS; i++) {
        printf("$%-2i :%8i (0x%08x) \n", i, arm11->registers[i], arm11->registers[i]);
    }
    printf("PC  :%8i (0x%08x) \n", arm11->PC, arm11->PC);
    printf("CSPR:%8i (0x%08x) \n", arm11->CSPR, arm11->CSPR);

    printf("%s\n", "Non-zero memory:");
    for (i = 0; i < MEMORY_SIZE; i += 4) {
        uint32_t value = getMemoryValue(i, arm11);
        if (value != 0) {
            printf("%08x:  0x%08x \n", i, value);
        }
    }
}

uint32_t fetch(ARM11 *arm11) {
    //TODO: Might need to add error if not multiple of 4
    return littleToBig(arm11->PC, arm11);
}

void printByte_inBinary(uint8_t byte) {
    uint8_t mask = 0;

    for (int i = BYTE_VALUE; i > 0; i--) {
        mask = (uint8_t) (1 << i);
        if ((mask & byte) > 0)
            printf("1");
        else
            printf("0");
    }
    printf("\n");
}

void readFile(char *fileName, ARM11 *arm11) {
    FILE *file = fopen(fileName, "r");
    char byte;
    int memoryLocation = 0;

    while ((byte = (char) fgetc(file)) != EOF) {
        printByte_inBinary((uint8_t) byte);
        arm11->memory[memoryLocation++] = (uint8_t) byte;
    }

    fclose(file);
}

//TODO: Place holder for real decode
char decode(uint32_t fetched) {
    if (fetched == 0)
        return 'a';
    return 'b';
}

//TODO: check if it goes out of memory
void fillPipeline(char *decoded, uint32_t *fetched, ARM11 *arm11) {

    *fetched = fetch(arm11);
    *decoded = decode(*fetched);
    *fetched = fetch(arm11);
}

int main(int argc, char **argv) {
    ARM11 arm11;
    initialize(&arm11);
    readFile(argv[1], &arm11);

    uint32_t fetched;
    //TODO: Change type to struct from David
    char decoded;
    enum FLAG flagExecute = NORMAL;

    fillPipeline(&decoded, &fetched, &arm11);
    do {
        flagExecute = execute(decoded, &arm11);
        if (flagExecute == BRANCH) {
            //TODO change program counter
            fillPipeline(&decoded, &fetched, &arm11);
        } else {
            decoded = decode(fetched);
            fetched = fetch(&arm11);
        }
    } while (flagExecute != STOP);

    print(&arm11);
    return EXIT_SUCCESS;
}

