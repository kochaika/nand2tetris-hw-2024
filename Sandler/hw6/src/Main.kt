package com.stas.comparchitecture.hw

import java.io.File

fun main(args: Array<String>) {
    val fileName = args.first()
    val fileNameWithoutExt = fileName.substringBeforeLast(".")

    val inputFile = File(fileName)
    if(!inputFile.exists()) {
        System.err.println("File $fileName not found")
        return
    }
    val outputFile = File("$fileNameWithoutExt.hack")
    if(outputFile.exists()) {
        outputFile.delete()
    }
    outputFile.createNewFile()
    println("Success!")
    System.err.println("Writing the result to $fileNameWithoutExt.hack ...")
    val lines = inputFile.readLines()
    val processedLines = Preprocessor.process(lines)
    outputFile.writeText(Translator.translate(processedLines))
}