from collections.abc import Iterable


class Translator:
    SYMBOL_TABLE = {
        'SCREEN': 16384,
        'KBD': 24576,
        'SP': 0,
        'LCL': 1,
        'ARG': 2,
        'THIS': 3,
        'THAT': 4
    } | {
        f'R{i}': i for i in range(16)
    }

    DEST_TO_BINARY = {
        key.strip(): value for key, value in {
            'null': '000',
            'M   ': '001',
            'D   ': '010',
            'MD  ': '011',
            'A   ': '100',
            'AM  ': '101',
            'AD  ': '110',
            'AMD ': '111'
        }.items()
    }

    JUMP_TO_BINARY = {
        key.strip(): value for key, value in {
            'null': '000',
            'JGT ': '001',
            'JEQ ': '010',
            'JGE ': '011',
            'JLT ': '100',
            'JNE ': '101',
            'JLE ': '110',
            'JMP ': '111'
        }.items()
    }

    COMP_TO_BINARY = {
        key.strip(): value for key, value in {
            '0  ': '0101010',
            '1  ': '0111111',
            '-1 ': '0111010',
            'D  ': '0001100',
            'A  ': '0110000',
            'M  ': '1110000',
            '!D ': '0001101',
            '!A ': '0110001',
            '!M ': '1110001',
            '-D ': '0001111',
            '-A ': '0110011',
            '-M ': '1110011',
            'D+1': '0011111',
            'A+1': '0110111',
            'M+1': '1110111',
            'D-1': '0001110',
            'A-1': '0110010',
            'M-1': '1110010',
            'D+A': '0000010',
            'D+M': '1000010',
            'D-A': '0010011',
            'D-M': '1010011',
            'A-D': '0000111',
            'M-D': '1000111',
            'D&A': '0000000',
            'D&M': '1000000',
            'D|A': '0010101',
            'D|M': '1010101'
        }.items()
    }

    def __init__(self, filepath: str):
        self.filepath = filepath
        with open(filepath, 'r') as file:
            self.lines = list(filter(None, (
                line.split('//')[0].strip()
                for line in file.readlines()
            )))

        self.symbol_table = Translator.SYMBOL_TABLE.copy()
        self.running_address = 16

        self.parse_labels()
        self.instructions = list(self.translate())

    def parse_labels(self):
        instruction_no = 0
        for line in self.lines:
            if line.startswith('(') and line.endswith(')'):
                self.symbol_table[line.removeprefix('(').removesuffix(')')] = instruction_no
            else:
                instruction_no += 1

    def translate(self) -> Iterable[str]:
        for line in self.lines:
            if line.startswith('@'):
                yield self.translate_a_instruction(line)
            elif line.startswith('(') and line.endswith(')'):
                pass
            else:
                yield self.translate_c_instruction(line)

    def translate_a_instruction(self, line: str):
        line = line.removeprefix('@')
        if not line.isdigit():
            if line not in self.symbol_table:
                self.symbol_table[line] = self.running_address
                self.running_address += 1
            line = self.symbol_table[line]
        return f'0{int(line):015b}'

    def translate_c_instruction(self, line: str):
        comp = line

        dest = jump = 'null'
        if ';' in comp:
            comp, jump = comp.split(';')
        if '=' in comp:
            dest, comp = comp.split('=')

        return f'111{Translator.COMP_TO_BINARY[comp]}{Translator.DEST_TO_BINARY[dest]}{Translator.JUMP_TO_BINARY[jump]}'


if __name__ == '__main__':  # Quick tests
    instructions = Translator('../asm/Pong.asm').instructions
    print(*instructions, sep='\n')
