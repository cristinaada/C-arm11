#include <stdlib.h>
#include <stdio.h>

struct ARM11 {
    /**
     * Memory contains 32 bit words.
     * Memory locations are only byte addressable
     * All the instruction addresses are multiples of 4
     */
    //TODO: need to add macro for 65536, once we learn what a macro is
    uint8_t memory[65536];

    /**
    * registers[0] .. registers[12] represent Register 0 - 12
    * registers[13] represents the Program Counter(PC)
    * registers[14] represents the Flags Register(CPSR)
    */
    uint32_t registers[15];
};

/**
 * !!Tested!!
 * Initializes the memory and the registers of the given arm11 to 0
 * @param arm11
 */
void initialize(struct ARM11 *arm11) {
  //TODO: replace 65536 and 15 with macros
  int i;
  for (i = 0; i < 65536; i++) {
    arm11->memory[i] = 0;
  }
  for (i = 0; i < 15; i++) {
    arm11->registers[i] = 0;
  }
}

/**
 * X Not Tested X
 * Takes a memory address and converts the next 4 bytes following the address from Little Endian to Big Endian
 * and returns the resulting 32-bit int
 * @param i The memory address
 * @param arm11
 * @return The Big Endian number
 */
uint32_t littleToBig(int i, struct ARM11 *arm11) {
  uint32_t value = 0;
  int j;
  for (j = 0; j < 4; j++) {
    value <<= 8;
    value += arm11->memory[i - j];
  }
  return value;
}

/**
 * !!Tested!!
 * Takes a memory address and returns the value of the next 4 bytes following the address, representing an instruction
 * @param i The memory address
 * @param arm11
 * @return value of 32 bit instruction
 */
uint32_t getMemoryValue(int i, struct ARM11 *arm11) {
  uint32_t value = 0;
  int j;
  for (j = 0; j < 4; j++) {
    value <<= 8;
    value += arm11->memory[i + j];
  }
  return value;
}

/**
 * !!Tested!!
 * Prints the registers and the non-zero memory locations of the given arm11
 * @param arm11
 */
void print(struct ARM11 *arm11) {
  //TODO: replace 65536 and 15 with macros
  int i;
  printf("%s\n", "Registers:");
  for (i = 0; i < 12; i++) {
    printf("$%-2i :%8i (0x%08x) \n", i, arm11->registers[i], arm11->registers[i]);
  }
  printf("PC  :%8i (0x%08x) \n", arm11->registers[13], arm11->registers[13]);
  printf("CSPR:%8i (0x%08x) \n", arm11->registers[14], arm11->registers[14]);

  printf("%s\n", "Non-zero memory:");
  for (i = 0; i < 65536; i += 4) {
    uint32_t value = getMemoryValue(i, arm11);
    if (value != 0) {
      printf("%08x:  0x%08x \n", i, value);
    }
  }
}

/**
 * X Not Tested X
 * Fetches the next instruction from memory
 * @param arm11
 * @return The fetched 32 bit instruction in Big Endian format
 */
uint32_t fetch(struct ARM11 *arm11) {
  return littleToBig(arm11->registers[13], arm11);
}

/**
 * !!Tested!!
 * Prints the binary value of a byte
 * @param The byte we wish to print
 */
void printByte_inBinary(uint8_t byte) {
  uint8_t mask = 0;

  for (int i = 7; i >= 0; i--) {
    mask = (uint8_t) (1 << i);
    if ((mask & byte) > 0)
      printf("1");
    else
      printf("0");
  }
  printf("\n");
}

/**
 * !!Tested!!
 * Reads that contents of a file and transfers them to memory
 * @param fileName The path of the file we wish to read
 */
void readFile(char *fileName, struct ARM11 *arm11) {
  FILE *file = fopen(fileName, "r");
  char byte;
  int memoryLocation = 0;

  while ((byte = (char) fgetc(file)) != EOF) {
    //printf("%d - ",c);
    //printByte_inBinary(charToByte(c));
    arm11->memory[memoryLocation++] = (uint8_t) byte;
  }

  fclose(file);
}


int main(int argc, char **argv) {
  struct ARM11 arm11;
  initialize(&arm11);
  readFile(argv[1], &arm11);

  /*do {
     stop = execute(decoded, *arm11);
     decoded = decode(instr);
     instr = fetch(*arm11);
  } while(!stop);
   */

  print(&arm11);
  return EXIT_SUCCESS;
}

