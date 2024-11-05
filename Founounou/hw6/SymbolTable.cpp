/**
 * SymbolTable.cpp
 * 
 * Implementation of the SymbolTable class.
 */

#include "SymbolTable.h"

#include <iostream> // Optional: For debug purposes

// Constructor
SymbolTable::SymbolTable() : current_register(16) {
    // Initialize predefined symbols
    for (int i = 0; i <= 15; i++) {
        std::string key = "R" + std::to_string(i);
        sym_table[key] = i;
    }

    sym_table["SCREEN"] = 16384;
    sym_table["KBD"]    = 24576;
    sym_table["SP"]     = 0;
    sym_table["LCL"]    = 1;
    sym_table["ARG"]    = 2;
    sym_table["THIS"]   = 3;
    sym_table["THAT"]   = 4;
}

// Adds a variable to the symbol table
bool SymbolTable::addVariable(const std::string& symbol) {
    if (!contains(symbol)) {
        sym_table[symbol] = current_register;
        current_register++;
        return true;
    }
    return false;
}

// Puts a key-value pair into the symbol table
void SymbolTable::put(const std::string& symbol, int value) {
    sym_table[symbol] = value;
}

// Checks if the symbol table contains the symbol
bool SymbolTable::contains(const std::string& symbol) const {
    return sym_table.find(symbol) != sym_table.end();
}

// Retrieves the value associated with the symbol
int SymbolTable::get(const std::string& symbol) const {
    auto it = sym_table.find(symbol);
    if (it != sym_table.end()) {
        return it->second;
    } else {
        // Symbol not found. You can choose to handle this differently (e.g., throw an exception)
        return -1;
    }
}