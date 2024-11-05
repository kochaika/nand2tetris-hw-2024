/**
 * SymbolTable.h
 * 
 * @author Franklin Ogidi 
 * 
 * @description Manages a table of symbols including predefined variables, user-defined variables, and labels.
 * This program was written as part of the NandToTetris course: https://www.nand2tetris.org/project06
 * 
 * @version 1.0
 * 
 */

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <unordered_map>

class SymbolTable {
private:
    int current_register;   // stores the register address (in decimal) to which the next variable is assigned
    std::unordered_map<std::string, int> sym_table; // stores the symbol table

public:
    // Constructor
    SymbolTable();

    /**
     * Adds a variable defined by 'symbol' if not present in the SymbolTable. New
     * variables get the value of 'current_register' beginning from 16.
     * 
     * @param symbol The symbol to be added to the table
     * 
     * @return True if the symbol was successfully added; False otherwise
     */
    bool addVariable(const std::string& symbol);

    /**
     * Puts a key-value pair into the SymbolTable
     * 
     * @param symbol The symbol to put into the table
     * @param value  The value associated with the symbol
     */
    void put(const std::string& symbol, int value);

    /**
     * Checks if the SymbolTable contains the given key
     * 
     * @param symbol The symbol to check for in the SymbolTable
     * 
     * @return True if the SymbolTable contains the symbol; False otherwise
     */
    bool contains(const std::string& symbol) const;

    /**
     * Retrieves the value of a given symbol
     * 
     * @param symbol The symbol for which the value is to be retrieved
     * 
     * @return The value of the symbol if in the SymbolTable; -1 otherwise
     */
    int get(const std::string& symbol) const;
};

#endif // SYMBOLTABLE_H
