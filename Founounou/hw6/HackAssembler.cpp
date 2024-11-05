/**
 * HackAssembler.cpp
 * 
 * Implementation of the HackAssembler class.
 */

#include "HackAssembler.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <bitset>
#include <regex>

// Constructor
HackAssembler::HackAssembler() : current_line(0), symbols() {
    // SymbolTable is initialized in its own constructor
}

// Performs the first pass to record labels
void HackAssembler::first_pass(const std::string& filename) {
    std::ifstream input(filename);
    if (!input.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << " for reading." << std::endl;
        return;
    }

    std::string line;
    int line_number = 0; // Keeps track of the current line number in the output

    while (std::getline(input, line)) {
        Parser parser;
        if (parser.parse_command(line)) {
            std::string trimmed_line = line;
            trimmed_line.erase(0, trimmed_line.find_first_not_of(" \t")); // Trim leading whitespace

            if (!trimmed_line.empty() && trimmed_line[0] == '(') { // Label declaration
                // Extract label name between parentheses
                std::string label = trimmed_line.substr(1, trimmed_line.find(')') - 1);

                // Add label to symbol table if not already present
                if (!symbols.contains(label)) {
                    symbols.put(label, line_number);
                }
                // Labels do not increment the line number
            } else {
                line_number++; // Valid instruction increments the line number
            }
        }
    }

    input.close();
}

// Translates the assembly code into machine code
void HackAssembler::translate(const std::string& filename) {
    std::ifstream input(filename);
    if (!input.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << " for reading." << std::endl;
        return;
    }

    // Output filename with .hack extension
    std::string output_filename = filename.substr(0, filename.find_last_of('.')) + ".hack";
    std::ofstream output(output_filename);
    if (!output.is_open()) {
        std::cerr << "Error: Unable to open file " << output_filename << " for writing." << std::endl;
        return;
    }

    std::string line;

    while (std::getline(input, line)) {
        Parser parser;
        if (parser.parse_command(line)) {
            std::string trimmed_line = line;
            trimmed_line.erase(0, trimmed_line.find_first_not_of(" \t")); // Trim leading whitespace

            if (trimmed_line.empty() || trimmed_line[0] == '(') {
                // Skip label declarations and empty lines
                continue;
            }

            if (!parser.addr_().empty()) { // A-instruction
                std::string var = parser.addr_();
                int address;

                // Check if var is an integer (numeric address)
                if (std::regex_match(var, std::regex("\\d+"))) {
                    address = std::stoi(var);
                } else {
                    // If the symbol is not in the symbol table, add it
                    if (!symbols.contains(var)) {
                        symbols.addVariable(var);
                    }
                    address = symbols.get(var);
                }

                // Convert address to 16-bit binary
                std::bitset<16> binary_address(address);
                output << binary_address.to_string() << '\n';

            } else { // C-instruction
                std::string comp = Code::get_comp_code(parser.comp_());
                std::string dest = Code::get_dest_code(parser.dest_());
                std::string jump = Code::get_jump_code(parser.jump_());

                if (comp.empty() || dest.empty() || jump.empty()) {
                    std::cerr << "Error: Invalid C-instruction at line " << current_line << std::endl;
                    continue;
                }

                // Write the machine code instruction
                output << "111" << comp << dest << jump << '\n';
            }
            current_line++;
        }
    }

    input.close();
    output.close();
}

// Pads a binary string to ensure it's 16 bits
std::string HackAssembler::pad_binary(const std::string& unpadded_binary) {
    std::string padded_binary = unpadded_binary;
    while (padded_binary.length() < 16) {
        padded_binary = '0' + padded_binary;
    }
    return padded_binary;
}

// Interface method to assemble the file
void HackAssembler::assemble(const std::string& filename) {
    first_pass(filename);
    translate(filename);
}

// Main function
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: HackAssembler <filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];
    HackAssembler assembler;
    assembler.assemble(filename);

    return 0;
}