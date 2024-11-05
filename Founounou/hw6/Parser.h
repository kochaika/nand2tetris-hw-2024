#ifndef PARSER_H
#define PARSER_H

#include <string>

class Parser {
private:
    std::string dest;    // holds the destination instruction
    std::string comp;    // holds the computation instruction
    std::string jump;    // holds the jump instruction
    std::string addr;    // holds a 16-bit address

    /**
     * A helper function that checks for and handles the presence of a jump field
     * 
     * @param str A string that potentially contains a jump instruction
     */
    void split_jump(const std::string& str);

public:
    // Constructor
    Parser();

    /**
     * Parses a line of assembly instructions into different fields according to
     * the Hack Language specification.
     * 
     * @param line An instruction to be parsed
     * 
     * @return True if the line was successfully parsed; False otherwise.
     */
    bool parse_command(const std::string& line);

    /**
     * Returns the dest field
     * 
     * @return The destination field of the instruction
     */
    std::string dest_() const;

    /**
     * Returns the comp field
     * 
     * @return The computation field of the instruction
     */
    std::string comp_() const;

    /**
     * Returns the jump field
     * 
     * @return The jump field of the instruction
     */
    std::string jump_() const;

    /**
     * In the case of an A-instruction, returns a variable, label, or address
     * 
     * @return A 16-bit address
     */
    std::string addr_() const;
};

#endif // PARSER_H