use anyhow::{anyhow, Result};

pub struct Code {}

impl Code {
    pub fn match_dest(dest: &str) -> Result<&str> {
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

    pub fn match_comp(comp: &str) -> Result<&str> {
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

    pub fn match_jump(jump: &str) -> Result<&str> {
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
}