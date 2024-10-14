// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

//// Replace this comment with your code.



//screenBytes is the number of bytes we need 
//to change to cover the whole screen
@8192
D=A
@screenBytes
M=D

(MAIN_LOOP)
    @KBD
    D=M
    @DRAW_BLACK
    D;JGT

    (DRAW_WHITE)
    @counter   
    M=0

    (PAINT_LOOP_WHITE_START)
        @counter
        D=M
        @screenBytes
        D=D-M
        @PAINT_LOOP_WHITE_END
        D;JGE

        @SCREEN
        D=A
        @counter
        A=D+M
        M=0

        @counter
        M=M+1
        @PAINT_LOOP_WHITE_START
        0;JMP
    (PAINT_LOOP_WHITE_END)

    @MAIN_LOOP
    0;JMP



    (DRAW_BLACK)

    @counter   
    M=0

    (PAINT_LOOP_BLACK_START)
        @counter
        D=M
        @screenBytes
        D=D-M
        @PAINT_LOOP_BLACK_END
        D;JGE

        @SCREEN
        D=A
        @counter
        A=D+M
        M=-1

        @counter
        M=M+1
        @PAINT_LOOP_BLACK_START
        0;JMP
    (PAINT_LOOP_BLACK_END)


    @MAIN_LOOP
    0;JMP
