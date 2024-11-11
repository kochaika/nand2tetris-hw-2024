#ifndef HACKASSEMBLER_H
#define HACKASSEMBLER_H

#include <string>
#include "SymbolTable.h"
#include "Parser.h"
#include "Code.h"

class HackAssembler {
private:
    SymbolTable symbols;  // stores predefined and user-defined symbols and labels
    int current_line;     // keeps track of the current valid line during file I/O

    /**
     * Performs the first pass on the file specified by filename, noting only the
     * labels. Adds label to the SymbolTable only at the first occurrence.
     * 
     * @param filename The .asm file to parse.
     */
    void first_pass(const std::string& filename);

    /**
     * Translates a Hack Assembly file (.asm) into machine code (.hack file)
     * according to the Hack Machine Language specifications, after the first pass.
     * 
     * @param filename The assembly file to translate into machine code
     */
    void translate(const std::string& filename);

    /**
     * Pads a binary string with zeros to ensure 16-bit binary format
     * 
     * @param unpadded_binary The binary string without leading zeros
     * @return A 16-bit binary string with leading zeros where necessary
     */
    std::string pad_binary(const std::string& unpadded_binary);

public:
    // Constructor
    HackAssembler();

    /**
     * Interface for running the assembler
     * 
     * @param filename The assembly file to process
     */
    void assemble(const std::string& filename);
};

#endif // HACKASSEMBLER_H
