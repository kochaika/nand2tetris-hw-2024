use std::collections::HashMap;
use anyhow::{anyhow, Result};
use crate::ADDR_UPPER_BOUND;

pub struct SymbolTable {
    symbols: HashMap<String, u16>,
    symbol_counter: u16,
}

impl Default for SymbolTable {
    fn default() -> Self {
        Self::new()
    }
}

impl SymbolTable {
    pub fn new() -> Self {
        Self{
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
        }
    }
    
    pub fn add(&mut self, symbol: String, address: u16) {
        self.symbols.insert(symbol, address);
    }
    
    pub fn get(&mut self, symbol: &str) -> Result<u16> {
        match self.symbols.get(symbol) {
            Some(addr) => Ok(*addr),
            None => {
                let addr = self.symbol_counter;
                if addr > ADDR_UPPER_BOUND {
                    return Err(anyhow!("Symbol table overflow"));
                }
                self.symbols.insert(symbol.to_string(), addr);
                self.symbol_counter += 1;
                Ok(addr)
            }
        }
    }
}