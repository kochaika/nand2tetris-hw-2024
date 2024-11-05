DESCRIPTION:

+ HackAssembler.cpp: Translate code written in Hack assembly langueg into machine code

+ Parser.cpp: Unpacks a line of assembly instructions into underlying fields.

+ Code.cpp: Translates a given field into its corresponding value.

+ SymbolTable.cpp: Manages a table of symbols including predefined variables, user-defined variables and labels.

USAGE:

g++ -Wall -o HackAssembler HackAssembler.cpp Parser.cpp SymbolTable.cpp Code.cpp 
./HackAssembler <input file> (for example: Add.asm)