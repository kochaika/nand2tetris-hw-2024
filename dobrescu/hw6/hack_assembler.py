import sys
from parser import Parser
from code import Code
from symbol_table import SymbolTable

class HackAssembler:
    def __init__(self, input_file):
        self.input_file = input_file
        self.output_file = input_file.replace(".asm", ".hack")
        self.symbol_table = SymbolTable()

    def assemble(self):
        parser = Parser(self.input_file)
        address = 0
        while parser.has_more_commands():
            parser.advance()
            if parser.command_type() == "L_COMMAND":
                self.symbol_table.add_entry(parser.symbol(), address)
            else:
                address += 1

        parser = Parser(self.input_file)
        code = Code()
        variable_address = 16
        with open(self.output_file, "w") as f:
            while parser.has_more_commands():
                parser.advance()
                if parser.command_type() == "A_COMMAND":
                    symbol = parser.symbol()
                    if symbol.isdigit():
                        address = int(symbol)
                    else:
                        if not self.symbol_table.contains(symbol):
                            self.symbol_table.add_entry(symbol, variable_address)
                            variable_address += 1
                        address = self.symbol_table.get_address(symbol)
                    f.write(f"{address:016b}\n")
                elif parser.command_type() == "C_COMMAND":
                    comp = code.comp(parser.comp())
                    dest = code.dest(parser.dest())
                    jump = code.jump(parser.jump())
                    f.write(f"111{comp}{dest}{jump}\n")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python hack_assembler.py Prog.asm")
        sys.exit(1)
    assembler = HackAssembler(sys.argv[1])
    assembler.assemble()
