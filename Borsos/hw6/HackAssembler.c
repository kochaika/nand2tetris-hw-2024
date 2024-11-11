#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256
#define SYMBOL_TABLE_SIZE 100

// Instruction Types
typedef enum { A_INSTRUCTION, C_INSTRUCTION, L_INSTRUCTION } InstructionType;

// Structure for a symbol in the symbol table
typedef struct {
    char symbol[MAX_LINE_LENGTH];
    int address;
} Symbol;

// Symbol table array and current index
Symbol symbolTable[SYMBOL_TABLE_SIZE];
int symbolCount = 0;
int variableAddress = 16;  // Start variables at address 16

// Function to remove whitespace and comments from a line
void clean_line(char *line) {
    char *comment = strchr(line, '/');
    if (comment) *comment = '\0';  // Remove comments
    // Trim whitespace
    char *end = line + strlen(line) - 1;
    while (end > line && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
}

// Add a symbol to the symbol table
void add_symbol(const char *symbol, int address) {
    strcpy(symbolTable[symbolCount].symbol, symbol);
    symbolTable[symbolCount].address = address;
    symbolCount++;
}

// Get the address of a symbol from the table, or -1 if not found
int get_symbol_address(const char *symbol) {
    for (int i = 0; i < symbolCount; i++) {
        if (strcmp(symbolTable[i].symbol, symbol) == 0) {
            return symbolTable[i].address;
        }
    }
    return -1; // Symbol not found
}

// Initialize predefined symbols in Hack assembly
void initialize_symbol_table() {
    for (int i = 0; i <= 15; i++) {
        char reg[4];
        sprintf(reg, "R%d", i);
        add_symbol(reg, i);
    }
    add_symbol("SCREEN", 16384);
    add_symbol("KBD", 24576);
    add_symbol("SP", 0);
    add_symbol("LCL", 1);
    add_symbol("ARG", 2);
    add_symbol("THIS", 3);
    add_symbol("THAT", 4);
}

void print_symbol_table() {
    for (int i = 0; i < symbolCount; i++) {
        printf("%s %d\n", symbolTable[i].symbol, symbolTable[i].address);
    }
}

// Determine the type of instruction (A, C, or L)
InstructionType get_instruction_type(char *line) {
    if (line[0] == '@') return A_INSTRUCTION;
    if (line[0] == '(' && line[strlen(line) - 1] == ')') return L_INSTRUCTION;
    return C_INSTRUCTION;
}

// Parse the symbol from an A-instruction or L-instruction
void parse_symbol(char *line, char *symbol) {
    if (line[0] == '@') {  // A-instruction
        strcpy(symbol, line + 1);
    } else {               // L-instruction
        strncpy(symbol, line + 1, strlen(line) - 2);
        symbol[strlen(line) - 2] = '\0';
    }
}

// Translate an A-instruction to binary
void translate_a_instruction(char *line, FILE *output) {
    char symbol[MAX_LINE_LENGTH];
    parse_symbol(line, symbol);
    int address;

    // Check if symbol is a number or a variable
    if (isdigit(symbol[0])) {
        address = atoi(symbol);
    } else {
        int symbolAddress = get_symbol_address(symbol);
        if (symbolAddress == -1) {  // Not in symbol table
            address = variableAddress;
            add_symbol(symbol, variableAddress);
            variableAddress++;
        } else {
            address = symbolAddress;
        }
    }

    // Convert address to 16-bit binary
    char binary[17] = "0000000000000000";
    for (int i = 15; i >= 0; i--) {
        binary[i] = (address % 2) ? '1' : '0';
        address /= 2;
    }
    fprintf(output, "%s\n", binary);
}

// Function to get binary code for dest
const char* get_dest_code(const char *dest) {
    if (strcmp(dest, "") == 0)          return "000";
    else if (strcmp(dest, "M") == 0)    return "001";
    else if (strcmp(dest, "D") == 0)    return "010";
    else if (strcmp(dest, "MD") == 0)   return "011";
    else if (strcmp(dest, "A") == 0)    return "100";
    else if (strcmp(dest, "AM") == 0)   return "101";
    else if (strcmp(dest, "AD") == 0)   return "110";
    else if (strcmp(dest, "AMD") == 0)  return "111";
    return "000"; // default if no dest part
}

// Function to get binary code for jump
const char* get_jump_code(const char *jump) {
    if (strcmp(jump, "") == 0)         return "000";
    else if (strcmp(jump, "JGT") == 0) return "001";
    else if (strcmp(jump, "JEQ") == 0) return "010";
    else if (strcmp(jump, "JGE") == 0) return "011";
    else if (strcmp(jump, "JLT") == 0) return "100";
    else if (strcmp(jump, "JNE") == 0) return "101";
    else if (strcmp(jump, "JLE") == 0) return "110";
    else if (strcmp(jump, "JMP") == 0) return "111";
    return "000"; // default if no jump part
}

// Function to get binary code for comp
const char* get_comp_code(const char *comp) {
    if (strcmp(comp, "0") == 0)              return "0101010";
    else if (strcmp(comp, "1") == 0)         return "0111111";
    else if (strcmp(comp, "-1") == 0)        return "0111010";
    else if (strcmp(comp, "D") == 0)         return "0001100";
    else if (strcmp(comp, "A") == 0)         return "0110000";
    else if (strcmp(comp, "!D") == 0)        return "0001101";
    else if (strcmp(comp, "!A") == 0)        return "0110001";
    else if (strcmp(comp, "-D") == 0)        return "0001111";
    else if (strcmp(comp, "-A") == 0)        return "0110011";
    else if (strcmp(comp, "D+1") == 0)       return "0011111";
    else if (strcmp(comp, "A+1") == 0)       return "0110111";
    else if (strcmp(comp, "D-1") == 0)       return "0001110";
    else if (strcmp(comp, "A-1") == 0)       return "0110010";
    else if (strcmp(comp, "D+A") == 0)       return "0000010";
    else if (strcmp(comp, "D-A") == 0)       return "0010011";
    else if (strcmp(comp, "A-D") == 0)       return "0000111";
    else if (strcmp(comp, "D&A") == 0)       return "0000000";
    else if (strcmp(comp, "D|A") == 0)       return "0010101";
    else if (strcmp(comp, "M") == 0)         return "1110000";
    else if (strcmp(comp, "!M") == 0)        return "1110001";
    else if (strcmp(comp, "-M") == 0)        return "1110011";
    else if (strcmp(comp, "M+1") == 0)       return "1110111";
    else if (strcmp(comp, "M-1") == 0)       return "1110010";
    else if (strcmp(comp, "D+M") == 0)       return "1000010";
    else if (strcmp(comp, "D-M") == 0)       return "1010011";
    else if (strcmp(comp, "M-D") == 0)       return "1000111";
    else if (strcmp(comp, "D&M") == 0)       return "1000000";
    else if (strcmp(comp, "D|M") == 0)       return "1010101";
    return "0101010"; // default if comp part is not recognized
}

void translate_c_instruction(char *line, FILE *output) {
    char dest[4] = "", comp[8] = "", jump[4] = "";

    // Check for destination (dest=comp;jump)
    char *equal_sign = strchr(line, '=');
    char *semicolon = strchr(line, ';');

    if (equal_sign) {
        // Parse destination and computation parts
        strncpy(dest, line, equal_sign - line);
        dest[equal_sign - line] = '\0';
        if (semicolon) {
            strncpy(comp, equal_sign + 1, semicolon - equal_sign - 1);
            comp[semicolon - equal_sign - 1] = '\0';
            strcpy(jump, semicolon + 1);
        } else {
            strcpy(comp, equal_sign + 1);
        }
    } else {
        // No destination, just computation and possibly jump
        if (semicolon) {
            strncpy(comp, line, semicolon - line);
            comp[semicolon - line] = '\0';
            strcpy(jump, semicolon + 1);
        } else {
            strcpy(comp, line);
        }
    }

    // Get binary codes for each part
    const char *comp_code = get_comp_code(comp);
    const char *dest_code = get_dest_code(dest);
    const char *jump_code = get_jump_code(jump);

    // Construct the full binary C-instruction
    fprintf(output, "111%s%s%s\n", comp_code, dest_code, jump_code);
}

int main() {
    char* input_files[7] = {"./Add/Add.asm", "./Max/Max.asm", "./Max/MaxL.asm", "./Rect/Rect.Asm", "./Rect/RectL.Asm", "./Pong/Pong.asm", "./Pong/PongL.asm"};
    char* output_files[7] = {"./Add/Add.hack", "./Max/Max.hack", "./Max/MaxL.hack", "./Rect/Rect.hack", "./Rect/RectL.hack", "./Pong/Pong.hack", "./Pong/PongL.hack"};

    for( int i=0; i<5; i++ ) 
    {
        FILE *input = fopen(input_files[i], "r");
        FILE *output = fopen(output_files[i], "w");

        if (!input || !output) {
            fprintf(stderr, "Error opening files.\n");
            return 1;
        }

        initialize_symbol_table();

        // First pass: process labels (L-instructions)
        int romAddress = 0;
        char line[MAX_LINE_LENGTH];
        while (fgets(line, sizeof(line), input)) {
            clean_line(line);
            if (line[0] == '\0' || line[1] == '\0') continue;
            InstructionType type = get_instruction_type(line);
            if (type == L_INSTRUCTION) {
                char symbol[MAX_LINE_LENGTH];
                parse_symbol(line, symbol);
                add_symbol(symbol, romAddress);
            } else {
                romAddress++;
            }
        }

        // Rewind file for second pass
        rewind(input);

        // Second pass: translate instructions
        while (fgets(line, sizeof(line), input)) {
            clean_line(line);
            if (line[0] == '\0' || line[1] == '\0') continue;

            InstructionType type = get_instruction_type(line);
            if (type == A_INSTRUCTION) {
                translate_a_instruction(line, output);
            } else if (type == C_INSTRUCTION) {
                translate_c_instruction(line, output);
            }
        }

        fclose(input);
        fclose(output);
    }
    return 0;
}
