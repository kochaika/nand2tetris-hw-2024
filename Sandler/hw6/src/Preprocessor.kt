package com.stas.comparchitecture.hw

object Preprocessor {
    fun String.isComment() = startsWith("//")

    fun process(lines: List<String>): List<String> {
        return lines.map {
            it.filter { !it.isWhitespace() }
        }.filter { !it.isComment() && it.isNotBlank() }
    }
}