use crate::ADDR_UPPER_BOUND;
use anyhow::{anyhow, Result};
use regex::Regex;

pub enum AInstr {
    Symbol(String),
    Address(u16),
}

pub enum Instruction {
    A(AInstr),
    C {
        dest: String,
        comp: String,
        jump: String,
    },
    L(String),
}

pub struct Parser {
    c_instr_regex: Regex,
}

impl Parser {
    pub fn new() -> Self {
        Self {
            c_instr_regex: Regex::new(r"^(?:(.+?)=)?(.+?)(?:;(.+))?$").unwrap(),
        }
    }

    fn parse_a_instruction(line: &str) -> Result<AInstr> {
        let value = line[1..].to_string();
        if value.chars().all(|c| char::is_ascii_digit(&c)) {
            let err = || anyhow!("Invalid address: {}", value);
            let addr = value.parse::<u16>().map_err(|_| err())?;
            if addr > ADDR_UPPER_BOUND {
                Err(err())
            } else {
                Ok(AInstr::Address(addr))
            }
        } else {
            Ok(AInstr::Symbol(value))
        }
    }

    fn parse_c_instruction(&self, line: &str) -> Result<Instruction> {
        self.c_instr_regex
            .captures(line)
            .ok_or(anyhow!("Invalid C instruction: {}", line))
            .map(|m| {
                let dest = m.get(1).map_or("null", |m| m.as_str()).to_string();
                let comp = m.get(2).map_or("", |m| m.as_str()).to_string();
                let jump = m.get(3).map_or("null", |m| m.as_str()).to_string();
                Instruction::C { dest, comp, jump }
            })
    }

    pub fn parse(&self, code: &str) -> Result<Vec<Instruction>> {
        let mut result = vec![];
        for line in code.lines() {
            let line = line.trim();
            if line.is_empty() || line.starts_with("//") {
                continue;
            }

            let instr = if line.starts_with("@") {
                Instruction::A(Self::parse_a_instruction(line)?)
            } else if line.starts_with("(") {
                Instruction::L(line[1..line.len() - 1].to_string())
            } else {
                self.parse_c_instruction(line)?
            };
            result.push(instr);
        }
        Ok(result)
    }
}
