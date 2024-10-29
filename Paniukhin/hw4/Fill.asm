// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed,
// the screen should be cleared.

(CHECK_KEYPRESS)
    @KBD
    D=M

    // If key is pressed, we set the color to black
    @SETCOLOR_BLACK
    D;JNE

    // Otherwise we set the color to white
    // @SETCOLOR_WHITE
    // 0;JMP

(SETCOLOR_WHITE)
    @COLOR
    M=0

    @INIT_DRAWING
    0;JMP

(SETCOLOR_BLACK)
    @COLOR
    M=-1

    // @INIT_DRAWING
    // 0;JMP

(INIT_DRAWING)
    // Start with cur_pixel = @SCREEN+8192 pixel
    @SCREEN
    D=A
    // @10 // For debugging
    @8192
    D=D+A
    @cur_pixel
    M=D

    // @LOOP
    // 0;JMP

(LOOP)
    // Exit loop when cur_pixel < @SCREEN
    @cur_pixel
    D=M
    @SCREEN
    D=D-A
    @CHECK_KEYPRESS
    D;JLT

    // Loop body: draw pixel
    @COLOR
    D=M
    @cur_pixel
    A=M
    M=D

    // Loop iteration: transision to the previous pixel
    @cur_pixel
    D=M-1
    M=D

@LOOP
0;JMP