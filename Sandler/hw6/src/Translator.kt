package com.stas.comparchitecture.hw

object Translator {
    val predefinedSymbols = mapOf(
        "R0" to 0,
        "R1" to 1,
        "R2" to 2,
        "R3" to 3,
        "R4" to 4,
        "R5" to 5,
        "R6" to 6,
        "R7" to 7,
        "R8" to 8,
        "R9" to 9,
        "R10" to 10,
        "R11" to 11,
        "R12" to 12,
        "R13" to 13,
        "R14" to 14,
        "R15" to 15,
        "SCREEN" to 16384,
        "KBD" to 24576,
        "SP" to 0,
        "LCL" to 1,
        "ARG" to 2,
        "THIS" to 3,
        "THAT" to 4
    )

    val labelSymbols = mutableMapOf<String, Int>()

    val variableSymbols = mutableMapOf<String, Int>()
    var currentVariable = 16

    fun String.isAInstruction() = firstOrNull() == '@'

    fun translateA(line: String): String {
        line.drop(1).toUIntOrNull()?.let {
            return "0${it.toString(radix = 2).padStart(15, '0')}"
        }
        predefinedSymbols[line.drop(1)]?.let {
            return "0${it.toString(radix = 2).padStart(15, '0')}"
        }
        labelSymbols[line.drop(1)]?.let {
            return "0${it.toString(radix = 2).padStart(15, '0')}"
        }
        variableSymbols[line.drop(1)]?.let {
            return "0${it.toString(radix = 2).padStart(15, '0')}"
        }
        variableSymbols[line.drop(1)] = currentVariable
        return "0${currentVariable++.toString(radix = 2).padStart(15, '0')}"
    }

    fun String.isLabel() = matches("\\(.*\\)".toRegex())

    fun translateDest(dest: String): String {
        return when(dest) {
            "null" -> "000"
            "M" -> "001"
            "D" -> "010"
            "MD" -> "011"
            "A" -> "100"
            "AM" -> "101"
            "AD" -> "110"
            "AMD" -> "111"
            else -> ""
        }
    }

    fun translateComp(comp: String): String {
        return when(comp) {
            "0" -> "0101010"
            "1" -> "0111111"
            "-1" -> "0111010"
            "D" -> "0001100"
            "A" -> "0110000"
            "!D" -> "0001101"
            "!A" -> "0110001"
            "-D" -> "0001111"
            "-A" -> "0110011"
            "D+1" -> "0011111"
            "A+1" -> "0110111"
            "D-1" -> "0001110"
            "A-1" -> "0110010"
            "D+A" -> "0000010"
            "D-A" -> "0010011"
            "A-D" -> "0000111"
            "D&A" -> "0000000"
            "D|A" -> "0010101"
            "M" -> "1110000"
            "!M" -> "1110001"
            "-M" -> "1110011"
            "M+1" -> "1110111"
            "M-1" -> "1110010"
            "D+M" -> "1000010"
            "D-M" -> "1010011"
            "M-D" -> "1000111"
            "D&M" -> "1000000"
            "D|M" -> "1010101"
            else -> ""
        }
    }

    fun translateJump(jump: String): String {
        return when(jump) {
            "null" -> "000"
            "JGT" -> "001"
            "JEQ" -> "010"
            "JGE" -> "011"
            "JLT" -> "100"
            "JNE" -> "101"
            "JLE" -> "110"
            "JMP" -> "111"
            else -> ""
        }
    }

    fun translateC(line: String): String {
        val equalsIndex = line.indexOf('=')
        val semicolonIndex = line.indexOf(';')
        val dest = if(equalsIndex != -1) line.substring(0, equalsIndex) else "null"
        val comp = line.substring(equalsIndex + 1, if(semicolonIndex == -1) line.length else semicolonIndex)
        val jump = if(semicolonIndex != -1) line.substring(semicolonIndex + 1) else "null"
        return "111" + translateComp(comp) + translateDest(dest) + translateJump(jump)
    }

    fun translateLine(line: String): String {
        return when {
            line.isAInstruction() -> translateA(line)
            else -> translateC(line)
        }
    }
    fun initSymbols(lines: List<String>) {
        var index = 0
        lines.forEach {
            if(it.isLabel()) {
                labelSymbols[it.drop(1).dropLast(1)] = index
            } else index ++
        }
    }
    fun translate(lines: List<String>): String {
        initSymbols(lines)
        return lines.filter{ !it.isLabel() }.joinToString(System.lineSeparator()) {
            translateLine(it)
        }
    }
}

fun main() {
    println(Translator.translateLine("@ponggame.0"))
}