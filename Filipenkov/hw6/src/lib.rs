use anyhow::{anyhow, Result};
use regex::Regex;
use std::collections::HashMap;

const ADDR_UPPER_BOUND: u16 = 0b1000_0000_0000_0000;


pub struct Translator {
    symbols: HashMap<String, u16>,
    symbol_counter: u16,
    c_instr_regex: Regex,
}

impl Default for Translator {
    fn default() -> Self {
        Self::new()
    }
}

impl Translator {
    pub fn new() -> Self {
        Self {
            symbols: HashMap::from([
                ("R0".to_string(), 0),
                ("R1".to_string(), 1),
                ("R2".to_string(), 2),
                ("R3".to_string(), 3),
                ("R4".to_string(), 4),
                ("R5".to_string(), 5),
                ("R6".to_string(), 6),
                ("R7".to_string(), 7),
                ("R8".to_string(), 8),
                ("R9".to_string(), 9),
                ("R10".to_string(), 10),
                ("R11".to_string(), 11),
                ("R12".to_string(), 12),
                ("R13".to_string(), 13),
                ("R14".to_string(), 14),
                ("R15".to_string(), 15),
                ("SCREEN".to_string(), 16384),
                ("KBD".to_string(), 24576),
                ("SP".to_string(), 0),
                ("LCL".to_string(), 1),
                ("ARG".to_string(), 2),
                ("THIS".to_string(), 3),
                ("THAT".to_string(), 4),
            ]),
            symbol_counter: 16,
            c_instr_regex: Regex::new(r"^(?:(.+?)=)?(.+?)(?:;(.+))?$").unwrap(),
        }
    }

    fn pass1(&mut self, code: &str) -> Vec<String> {
        let mut result = vec![];
        for line in code.lines() {
            let line = line.trim();
            if line.is_empty() || line.starts_with("//") {
                continue;
            }
            if line.starts_with("(") {
                let symbol = line[1..line.len() - 1].to_string();
                self.symbols.insert(symbol, result.len() as u16);
            } else {
                result.push(line.to_string());
            }
        }
        result
    }

    fn parse_a_instruction(&mut self, line: &str) -> Result<String> {
        let value = &line[1..];
        if value.chars().all(|c| char::is_ascii_digit(&c)) {
            match value.parse::<u16>() {
                Ok(addr) =>
                    if addr >= ADDR_UPPER_BOUND {
                        Err(anyhow!("Invalid address: {}", value))
                    } else {
                        Ok(format!("{:016b}", addr))
                    },
                Err(_) => Err(anyhow!("Invalid address: {}", value))
            }
        } else {
            match self.symbols.get(value) {
                Some(addr) => Ok(format!("{:016b}", addr)),
                None => {
                    let addr = self.symbol_counter;
                    if addr > ADDR_UPPER_BOUND {
                        return Err(anyhow!("Symbol table overflow"));
                    }
                    self.symbols.insert(value.to_string(), addr);
                    self.symbol_counter += 1;
                    Ok(format!("{:016b}", addr))
                }
            }
        }
    }

    fn match_dest(dest: &str) -> Result<&str> {
        match dest {
            "M" => Ok("001"),
            "D" => Ok("010"),
            "MD" => Ok("011"),
            "A" => Ok("100"),
            "AM" => Ok("101"),
            "AD" => Ok("110"),
            "AMD" => Ok("111"),
            "null" => Ok("000"),
            _ => Err(anyhow!("Invalid dest: {}", dest))
        }
    }

    fn match_comp(comp: &str) -> Result<&str> {
        match comp {
            "0" => Ok("0101010"),
            "1" => Ok("0111111"),
            "-1" => Ok("0111010"),
            "D" => Ok("0001100"),
            "A" => Ok("0110000"),
            "!D" => Ok("0001101"),
            "!A" => Ok("0110001"),
            "-D" => Ok("0001111"),
            "-A" => Ok("0110011"),
            "D+1" => Ok("0011111"),
            "A+1" => Ok("0110111"),
            "D-1" => Ok("0001110"),
            "A-1" => Ok("0110010"),
            "D+A" => Ok("0000010"),
            "D-A" => Ok("0010011"),
            "A-D" => Ok("0000111"),
            "D&A" => Ok("0000000"),
            "D|A" => Ok("0010101"),
            "M" => Ok("1110000"),
            "!M" => Ok("1110001"),
            "-M" => Ok("1110011"),
            "M+1" => Ok("1110111"),
            "M-1" => Ok("1110010"),
            "D+M" => Ok("1000010"),
            "D-M" => Ok("1010011"),
            "M-D" => Ok("1000111"),
            "D&M" => Ok("1000000"),
            "D|M" => Ok("1010101"),
            _ => Err(anyhow!("Invalid comp: {}", comp))
        }
    }

    fn match_jump(jump: &str) -> Result<&str> {
        match jump {
            "JGT" => Ok("001"),
            "JEQ" => Ok("010"),
            "JGE" => Ok("011"),
            "JLT" => Ok("100"),
            "JNE" => Ok("101"),
            "JLE" => Ok("110"),
            "JMP" => Ok("111"),
            "null" => Ok("000"),
            _ => Err(anyhow!("Invalid jump: {}", jump))
        }
    }

    fn parse_c_instruction(&self, line: &str) -> Result<String> {
        self.c_instr_regex
            .captures(line)
            .ok_or(anyhow!("Invalid C instruction: {}", line))
            .and_then(|m| {
                let dest = Self::match_dest(m.get(1).map_or("null", |m| m.as_str()))?;
                let comp = Self::match_comp(m.get(2).map_or("", |m| m.as_str()))?;
                let jump = Self::match_jump(m.get(3).map_or("null", |m| m.as_str()))?;
                Ok(format!("111{}{}{}", comp, dest, jump))
            })
    }

    fn pass2(&mut self, code: Vec<String>) -> Result<Vec<String>> {
        let mut result = vec![];
        for line in code {
            let instr = if line.starts_with("@") {
                self.parse_a_instruction(&line)?
            } else {
                self.parse_c_instruction(&line)?
            };
            result.push(instr);
        }
        Ok(result)
    }

    pub fn translate(&mut self, code: &str) -> Result<String> {
        let code = self.pass1(code);
        let code = self.pass2(code)?;
        Ok(code.join("\n"))
    }
}

#[cfg(test)]
mod test;
