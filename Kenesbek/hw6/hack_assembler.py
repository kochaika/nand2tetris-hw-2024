comp_table = {
    "0": "0101010", "1": "0111111", "-1": "0111010",
    "D": "0001100", "A": "0110000", "M": "1110000",
    "!D": "0001101", "!A": "0110001", "!M": "1110001",
    "-D": "0001111", "-A": "0110011", "-M": "1110011",
    "D+1": "0011111", "A+1": "0110111", "M+1": "1110111",
    "D-1": "0001110", "A-1": "0110010", "M-1": "1110010",
    "D+A": "0000010", "D+M": "1000010", "D-A": "0010011",
    "D-M": "1010011", "A-D": "0000111", "M-D": "1000111",
    "D&A": "0000000", "D&M": "1000000", "D|A": "0010101", "D|M": "1010101"
}

dest_table = {
    "": "000", "M": "001", "D": "010", "MD": "011",
    "A": "100", "AM": "101", "AD": "110", "AMD": "111"
}

jump_table = {
    "": "000", "JGT": "001", "JEQ": "010", "JGE": "011",
    "JLT": "100", "JNE": "101", "JLE": "110", "JMP": "111"
}

# Initialize symbol table with predefined symbols
symbol_table = {
    "SP": 0, "LCL": 1, "ARG": 2, "THIS": 3, "THAT": 4,
    "SCREEN": 16384, "KBD": 24576
}
symbol_table.update({f"R{i}": i for i in range(16)})  # R0 to R15

def clean_line(line):
    """Remove comments and whitespace."""
    line = line.split("//")[0].strip()  # Remove comments and trailing whitespace
    return line

def translate_a_instruction(instruction):
    """Translate A-instruction (e.g., @2) to binary."""
    value = int(instruction[1:])  # Get number after "@"
    return "0" + format(value, "015b")  # Convert to 15-bit binary prefixed by 0

def parse_c_instruction(instruction):
    """Separate comp, dest, and jump parts from a C-instruction."""
    if "=" in instruction:
        dest, remainder = instruction.split("=")
    else:
        dest, remainder = "", instruction

    if ";" in remainder:
        comp, jump = remainder.split(";")
    else:
        comp, jump = remainder, ""

    return dest, comp, jump

def translate_c_instruction(instruction):
    """Translate C-instruction to binary using lookup tables."""
    dest, comp, jump = parse_c_instruction(instruction)
    return "111" + comp_table[comp] + dest_table[dest] + jump_table[jump]

def first_pass(lines):
    """Process labels and add them to the symbol table."""
    line_number = 0
    for line in lines:
        if line.startswith("(") and line.endswith(")"):
            label = line[1:-1]
            symbol_table[label] = line_number  # Label points to this line
        else:
            line_number += 1

def second_pass(lines):
    """Replace symbols (variables) with addresses in the symbol table."""
    variable_address = 16
    output_lines = []
    for line in lines:
        # Skip labels
        if line.startswith("(") and line.endswith(")"):
            continue

        if line.startswith("@"):
            symbol = line[1:]
            if not symbol.isdigit():
                if symbol not in symbol_table:
                    symbol_table[symbol] = variable_address  # Assign new variable address
                    variable_address += 1
                address = symbol_table[symbol]
                output_lines.append(f"@{address}")
            else:
                output_lines.append(line)
        else:
            output_lines.append(line)
    return output_lines

def assemble(input_file, output_file):
    with open(input_file, "r") as file:
        lines = [clean_line(line) for line in file if clean_line(line)]

    first_pass(lines)

    processed_lines = second_pass(lines)

    with open(output_file, "w") as file:
        for line in processed_lines:
            if line.startswith("@"):
                binary_code = translate_a_instruction(line)
            else:
                binary_code = translate_c_instruction(line)
            file.write(binary_code + "\n")