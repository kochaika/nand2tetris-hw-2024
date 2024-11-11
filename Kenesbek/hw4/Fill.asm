// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

// Main loop to check for keyboard input
(START)
    @KBD
    D=M
    @BLACK_SCREEN       // Go to black screen if key is pressed
    D;JNE
    @WHITE_SCREEN       // Go to white screen if no key is pressed
    0;JMP

// Fill screen with black
(BLACK_SCREEN)
    @16384
    D=A
    @SCREEN_POINTER
    M=D                 // Set pointer to start of screen

(BLACK_FILL_LOOP)
    @SCREEN_POINTER
    A=M                 // Current screen pixel
    M=-1                // Set pixel to black

    @SCREEN_POINTER
    M=M+1               // Next pixel

    @24576
    D=A
    @SCREEN_POINTER
    D=M-D
    @BLACK_FILL_LOOP
    D;JLT               // Continue until screen is filled

    @START
    0;JMP               // Return to check keyboard

// Fill screen with white
(WHITE_SCREEN)
    @16384
    D=A
    @SCREEN_POINTER
    M=D                 // Set pointer to start of screen

(WHITE_FILL_LOOP)
    @SCREEN_POINTER
    A=M                 // Current screen pixel
    M=0                 // Set pixel to white

    @SCREEN_POINTER
    M=M+1               // Next pixel

    @24576
    D=A
    @SCREEN_POINTER
    D=M-D
    @WHITE_FILL_LOOP
    D;JLT               // Continue until screen is cleared

    @START
    0;JMP               // Return to check keyboard