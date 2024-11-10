use crate::parser::{AInstr, Instruction, Parser};
use crate::symbol_table::SymbolTable;
use anyhow::Result;
use crate::code::Code;

const ADDR_UPPER_BOUND: u16 = 0b1000_0000_0000_0000;


pub struct Assembler {
    symbols: SymbolTable,
    parser: Parser,
}

impl Default for Assembler {
    fn default() -> Self {
        Self::new()
    }
}

impl Assembler {
    pub fn new() -> Self {
        Self {
            symbols: SymbolTable::new(),
            parser: Parser::new(),
        }
    }

    fn pass1(&mut self, code: &str) -> Result<()> {
        let mut line = 0;
        for instr in self.parser.parse(code)? {
            match instr {
                Instruction::L(symbol) => {
                    self.symbols.add(symbol, line);
                }
                _ => {
                    line += 1;
                }
            }
        }
        Ok(())
    }

    fn pass2(&mut self, code: &str) -> Result<Vec<String>> {
        let mut result = vec![];
        for instr in self.parser.parse(code)? {
            match instr {
                Instruction::A(AInstr::Symbol(symbol)) => {
                    let addr = self.symbols.get(&symbol)?;
                    result.push(format!("{:016b}", addr));
                }
                Instruction::A(AInstr::Address(addr)) => {
                    result.push(format!("{:016b}", addr));
                }
                Instruction::C { dest, comp, jump } => {
                    let dest_bin = Code::match_dest(&dest)?;
                    let comp_bin = Code::match_comp(&comp)?;
                    let jump_bin = Code::match_jump(&jump)?;
                    result.push(format!("111{}{}{}", comp_bin, dest_bin, jump_bin));
                }
                _ => {}
            }
        }
        Ok(result)
    }

    pub fn translate(&mut self, code: &str) -> Result<String> {
        self.pass1(code)?;
        let compiled = self.pass2(code)?;
        Ok(compiled.join("\n") + "\n")
    }
}

#[cfg(test)]
mod test;
mod symbol_table;
mod parser;
mod code;
