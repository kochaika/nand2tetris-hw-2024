/**
 * Parser.cpp
 * 
 * Implementation of the Parser class.
 */

#include "Parser.h"

#include <algorithm> // for std::find_if, std::remove_if
#include <cctype>    // for std::isspace
#include <sstream>
#include <vector>

// Helper function to trim leading and trailing whitespace
static std::string trim(const std::string& s) {
    auto start = s.begin();
    while (start != s.end() && std::isspace(*start)) {
        start++;
    }

    auto end = s.end();
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace(*end));

    return std::string(start, end + 1);
}

// Helper function to split a string by a delimiter
static std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// Constructor
Parser::Parser() {
    dest = "null";
    jump = "null";
}

// Parses a line of assembly instructions
bool Parser::parse_command(const std::string& lineInput) {
    // Reset fields
    dest = "null";
    comp.clear();
    jump = "null";
    addr.clear();

    std::string line = lineInput;

    // Remove comments starting with "//"
    auto comment_pos = line.find("//");
    if (comment_pos != std::string::npos) {
        line = line.substr(0, comment_pos);
    }

    // Trim leading and trailing whitespace
    line = trim(line);

    if (!line.empty() && line[0] != '/') {
        if (line.find('@') != std::string::npos) { // A-instruction
            addr = trim(line.substr(line.find('@') + 1));
        } else { // C-instruction
            if (line.find('=') != std::string::npos) { // dest=comp;jump
                auto fields = split(line, '=');
                dest = trim(fields[0]);
                if (fields.size() > 1) {
                    auto rest = fields[1];
                    if (rest.find(';') != std::string::npos) {
                        split_jump(rest);
                    } else {
                        // Remove any comments and trim
                        auto pos = rest.find('/');
                        if (pos != std::string::npos) {
                            rest = rest.substr(0, pos);
                        }
                        comp = trim(rest);
                    }
                }
            } else if (line.find('+') != std::string::npos || line.find('-') != std::string::npos) {
                // comp[;jump]
                if (line.find(';') != std::string::npos) {
                    split_jump(line);
                } else {
                    auto rest = line;
                    // Remove comments
                    auto pos = rest.find('/');
                    if (pos != std::string::npos) {
                        rest = rest.substr(0, pos);
                    }
                    comp = trim(rest);
                }
            } else if (line.find(';') != std::string::npos) { // comp;jump
                split_jump(line);
            } else {
                // Possible jump-only command
                auto rest = line;
                // Remove comments
                auto pos = rest.find('/');
                if (pos != std::string::npos) {
                    rest = rest.substr(0, pos);
                }
                jump = trim(rest);
            }
        }
        return true; // Successfully parsed
    }

    return false; // Parsing unsuccessful
}

// Helper function to handle jump instructions
void Parser::split_jump(const std::string& strInput) {
    // Remove comments
    auto line = strInput;
    auto comment_pos = line.find('/');
    if (comment_pos != std::string::npos) {
        line = line.substr(0, comment_pos);
    }

    auto parts = split(line, ';');
    if (!parts.empty()) {
        comp = trim(parts[0]);
        if (parts.size() > 1) {
            jump = trim(parts[1]);
        }
    }
}

// Getter for dest field
std::string Parser::dest_() const {
    return dest;
}

// Getter for comp field
std::string Parser::comp_() const {
    return comp;
}

// Getter for jump field
std::string Parser::jump_() const {
    return jump;
}

// Getter for addr field
std::string Parser::addr_() const {
    return addr;
}