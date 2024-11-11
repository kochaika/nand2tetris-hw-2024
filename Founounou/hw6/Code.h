#ifndef CODE_H
#define CODE_H

#include <string>
#include <unordered_map>

class Code {
private:
    // Unordered maps to store the specifications for the destination, jump, and computation bits
    static std::unordered_map<std::string, std::string> dest_table;
    static std::unordered_map<std::string, std::string> jump_table;
    static std::unordered_map<std::string, std::string> comp_table;

    /**
     * Stores the destination codes
     */
    static void init_dest_table();

    /**
     * Stores the jump codes
     */
    static void init_jump_table();

    /**
     * Stores the computation codes
     */
    static void init_comp_table();

public:
    /**
     * Retrieves the binary code representing the computation bits given by the symbolic key
     *
     * @param key The symbolic representation of the computation bits to be retrieved
     *
     * @return A 7-bit binary string representing the computation instruction
     */
    static std::string get_comp_code(const std::string& key);

    /**
     * Retrieves the binary code representing the destination instruction given by the symbolic key
     *
     * @param key The symbolic representation of the destination bits to be retrieved
     *
     * @return A 3-bit binary string representing the destination instruction
     */
    static std::string get_dest_code(const std::string& key);

    /**
     * Retrieves the binary code representing the jump instruction given by the symbolic key
     *
     * @param key The symbolic representation of the jump bits to be retrieved
     *
     * @return A 3-bit binary string representing the jump instruction
     */
    static std::string get_jump_code(const std::string& key);
};

#endif // CODE_H