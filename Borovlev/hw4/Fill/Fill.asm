// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

//// Replace this comment with your code.

(LOOP)
    @KBD
    D=M
    @FILL
    D;JNE
    @CLEAR
    0;JMP
@LOOP
0;JMP

(FILL)
    @SCREEN
    D=A
    @index
    M=D
    (LOOP1)
        D=M
        @24576
        D=D-A
        @LOOP
        D;JEQ

        @index
        A=M
        M=-1

        @index
        M=M+1
    @LOOP1
    0;JMP

(CLEAR)
    @SCREEN
    D=A
    @index
    M=D
    (LOOP2)
        D=M
        @24576
        D=D-A
        @LOOP
        D;JEQ

        @index
        A=M
        M=0

        @index
        M=M+1
    @LOOP2
    0;JMP





