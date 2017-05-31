#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "assembleBranch.h"

uint8_t computeCond(char* operation) {
    if (!strcmp(operation, BEQ)) {
        return BEQ_BYTES;
    } else if (!strcmp(operation, BNE)) {
        return BNE_BYTES;
    } else if (!strcmp(operation, BGE)) {
        return BGE_BYTES;
    } else if (!strcmp(operation, BLT)) {
        return BLT_BYTES;
    } else if (!strcmp(operation, BGT)) {
        return BGT_BYTES;
    } else if (!strcmp(operation, BLE)) {
        return BLE_BYTES;
    } else if (!strcmp(operation, BAL)) {
        //TODO: Might need to add another check
        return BAL_BYTES;
    } else {
        perror("Branch - computeCond");
        exit(EXIT_FAILURE);
    }
}

int32_t getLabelAddress(char *searchedLabel, Map* labels) {
    for (int i = 0 ; labels[i].value != END_OF_MAP ; i++) {
        if (strstr(searchedLabel, labels[i].key) != NULL) {
            return (uint32_t)labels[i].value;
        }
    }
}

int32_t twosComplement24bit(int32_t number) {
    if (number > 0) {
        return number;
    } else {
        return (int32_t) (~((number + 1) | PADDING) | SIGN_BIT);
    }
}

int32_t calculateOffset(int32_t target, int32_t source) {
    return twosComplement24bit(((target - source) * MEMORY_MULTIPLIER - PIPELINE_OFFSET) >> 2);
}

int32_t encodeBranch(int argc, char**argv, Map* labels, int32_t currentOperationNumber) {
    int32_t result = computeCond(argv[0]);
    result <<= CONDITION_OFFSET;
    result |= BRANCH_OPCODE;
    result <<= OFFSET_OFFSET;
    return result | calculateOffset(getLabelAddress(argv[1], labels), currentOperationNumber);
}

