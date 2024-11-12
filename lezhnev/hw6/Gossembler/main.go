package main

import (
	"fmt"
	"log"
	"os"
	"strconv"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: go run main.go <filepathInput> <filepathOutput>")
		os.Exit(1)
	}
	filePathInput := os.Args[1]
	filePathOutput := os.Args[2]
	parser := Parser{filePathInput, nil, nil, 0}
	parser.open()
	symbolTable := SymbolTable{}.newSymbolTable()
	binaryResult := ""
	address := 0
	for parser.hasMoreLines() {
		instruction := parser.advance()
		if instruction == "" {
			continue
		}
		if instruction[0] == '(' {
			symbol := instruction[1 : len(instruction)-1]
			symbolTable.addEntry(symbol, address)
			continue
		}
		address++
	}
	address = 16
	count := 0
	parser.open()
	for parser.hasMoreLines() {
		instruction := parser.advance()
		if instruction == "" {
			continue
		}
		if instruction[0] == '(' {
			continue
		}
		count++
		if instruction[0] == '@' {
			symbol := instruction[1:]
			if symbol[0] >= '0' && symbol[0] <= '9' {
				num, err := strconv.Atoi(symbol)
				if err != nil {
					log.Fatalf("Invalid number: %s", symbol)
				}
				binaryResult += "0" + fmt.Sprintf("%015b", num) + "\n"
			} else {
				if !symbolTable.contains(symbol) {
					symbolTable.addEntry(symbol, address)
					address++
				}
				binaryResult += "0" + fmt.Sprintf("%015b", symbolTable.getAddress(symbol)) + "\n"
			}
		} else {
			destMnemonic, compMnemonic, jumpMnemonic := parseCInstruction(instruction)
			binaryResult += "111" + compMnemonic + destMnemonic + jumpMnemonic + "\n"
		}
	}
	file, err := os.Create(filePathOutput)
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	defer func(file *os.File) {
		err := file.Close()
		if err != nil {
			fmt.Println(err)
			os.Exit(1)
		}
	}(file)
	_, err = file.WriteString(binaryResult)
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}
