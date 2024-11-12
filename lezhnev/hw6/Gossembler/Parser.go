package main

import (
	"fmt"
	"os"
	"strings"
)

type Parser struct {
	filepath           string
	currentInstruction *string
	codeString         *string
	codeIndex          int
}

func (p *Parser) open() {
	p.codeString = new(string)
	p.codeIndex = 0
	codeBytes, err := os.ReadFile(p.filepath)
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	codeString := string(codeBytes)
	p.codeString = &codeString
}

func (p *Parser) hasMoreLines() bool {
	for p.codeIndex < len(*p.codeString) {
		for p.codeIndex < len(*p.codeString) && (*p.codeString)[p.codeIndex] == ' ' {
			p.codeIndex++
		}
		if (*p.codeString)[p.codeIndex] == '/' {
			for (*p.codeString)[p.codeIndex] != '\n' {
				p.codeIndex++
			}
		}
		if (*p.codeString)[p.codeIndex] == '\n' {
			p.codeIndex++
			continue
		}
		break
	}
	if p.codeIndex >= len(*p.codeString) {
		return false
	}
	return true
}

func (p *Parser) advance() string {
	if !p.hasMoreLines() {
		return ""
	}
	startIndex := p.codeIndex
	for p.codeIndex < len(*p.codeString) {
		if (*p.codeString)[p.codeIndex] == '\n' {
			break
		}
		p.codeIndex++
	}
	instruction := (*p.codeString)[startIndex:p.codeIndex]
	p.currentInstruction = &instruction
	return *p.currentInstruction
}

func (p *Parser) instructionType() string {
	if p.currentInstruction == nil {
		return ""
	}
	if (*p.currentInstruction)[0] == '@' {
		return "A_COMMAND"
	}
	if (*p.currentInstruction)[0] == '(' {
		return "L_COMMAND"
	}
	return "C_COMMAND"
}

func (p *Parser) symbol() string {
	if p.instructionType() == "A_COMMAND" {
		return (*p.currentInstruction)[1:]
	}
	if p.instructionType() == "L_COMMAND" {
		return (*p.currentInstruction)[1 : len(*p.currentInstruction)-1]
	}
	return ""
}

func (p *Parser) dest() string {
	if p.instructionType() != "C_COMMAND" {
		return ""
	}
	if p.currentInstruction == nil {
		return ""
	}
	if !strings.Contains(*p.currentInstruction, "=") {
		return ""
	}
	return strings.Split(*p.currentInstruction, "=")[0]
}

func (p *Parser) comp() string {
	if p.instructionType() != "C_COMMAND" {
		return ""
	}
	if p.currentInstruction == nil {
		return ""
	}
	if strings.Contains(*p.currentInstruction, "=") {
		return strings.Split(*p.currentInstruction, "=")[1]
	}
	return strings.Split(*p.currentInstruction, ";")[0]
}

func (p *Parser) jump() string {
	if p.instructionType() != "C_COMMAND" {
		return ""
	}
	if p.currentInstruction == nil {
		return ""
	}
	if !strings.Contains(*p.currentInstruction, ";") {
		return ""
	}
	return strings.Split(*p.currentInstruction, ";")[1]
}
