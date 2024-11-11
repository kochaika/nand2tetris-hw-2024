/**
 * Code.cpp
 *
 * Implementation of the Code class methods.
 */

#include "Code.h"

// Initialize static variables
std::unordered_map<std::string, std::string> Code::dest_table;
std::unordered_map<std::string, std::string> Code::jump_table;
std::unordered_map<std::string, std::string> Code::comp_table;

void Code::init_dest_table() {
    if (!dest_table.empty())
        return; // Already initialized

    dest_table["null"] = "000";
    dest_table["M"]    = "001"; // Memory (RAM)
    dest_table["D"]    = "010"; // D-register
    dest_table["MD"]   = "011";
    dest_table["A"]    = "100"; // A-register
    dest_table["AM"]   = "101";
    dest_table["AD"]   = "110";
    dest_table["AMD"]  = "111";
}

void Code::init_jump_table() {
    if (!jump_table.empty())
        return; // Already initialized

    jump_table["null"] = "000"; // No jump
    jump_table["JGT"]  = "001"; // Jump if greater than zero
    jump_table["JEQ"]  = "010"; // Jump if equal to zero
    jump_table["JGE"]  = "011"; // Jump if greater than or equal to zero
    jump_table["JLT"]  = "100"; // Jump if less than zero
    jump_table["JNE"]  = "101"; // Jump if not equal to zero
    jump_table["JLE"]  = "110"; // Jump if less than or equal to zero
    jump_table["JMP"]  = "111"; // Unconditional jump
}

void Code::init_comp_table() {
    if (!comp_table.empty())
        return; // Already initialized

    comp_table["0"]   = "0101010";
    comp_table["1"]   = "0111111";
    comp_table["-1"]  = "0111010";
    comp_table["D"]   = "0001100";
    comp_table["A"]   = "0110000";
    comp_table["!D"]  = "0001101";
    comp_table["!A"]  = "0110001";
    comp_table["-D"]  = "0001111";
    comp_table["-A"]  = "0110011";
    comp_table["D+1"] = "0011111";
    comp_table["A+1"] = "0110111";
    comp_table["D-1"] = "0001110";
    comp_table["A-1"] = "0110010";
    comp_table["D+A"] = "0000010";
    comp_table["D-A"] = "0010011";
    comp_table["A-D"] = "0000111";
    comp_table["D&A"] = "0000000";
    comp_table["D|A"] = "0010101";
    comp_table["M"]   = "1110000";
    comp_table["!M"]  = "1110001";
    comp_table["-M"]  = "1110011";
    comp_table["M+1"] = "1110111";
    comp_table["M-1"] = "1110010";
    comp_table["D+M"] = "1000010";
    comp_table["D-M"] = "1010011";
    comp_table["M-D"] = "1000111";
    comp_table["D&M"] = "1000000";
    comp_table["D|M"] = "1010101";
}

std::string Code::get_comp_code(const std::string& key) {
    init_comp_table();
    auto it = comp_table.find(key);
    if (it != comp_table.end())
        return it->second;
    else
        return ""; // Return an empty string or handle the error as needed
}

std::string Code::get_dest_code(const std::string& key) {
    init_dest_table();
    auto it = dest_table.find(key);
    if (it != dest_table.end())
        return it->second;
    else
        return ""; // Return an empty string or handle the error as needed
}

std::string Code::get_jump_code(const std::string& key) {
    init_jump_table();
    auto it = jump_table.find(key);
    if (it != jump_table.end())
        return it->second;
    else
        return ""; // Return an empty string or handle the error as needed
}