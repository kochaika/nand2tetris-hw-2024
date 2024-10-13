// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

(START)
    @KBD
    D=M

    // No key is pressed
    @NO_KEY_PRESSED
    D;JEQ    // if D==0 then we need to clear the screen

    // if not then we need to black the screen
    @BLACKEN_SCREEN
    0;JMP

(NO_KEY_PRESSED)
    // i = 16384
    @SCREEN
    D=A
    @i
    M=D

(CLEAR_LOOP)
    // if (i == 24575) goto END_ACTION
    @i
    D=M
    @24575
    D=A-D
    @END_ACTION
    D;JEQ
    
    // put white i
    @i
    A=M
    M=0

    // increment i
    @i
    M=M+1
    @CLEAR_LOOP
    0;JMP

(BLACKEN_SCREEN)
    // i = 16384
    @SCREEN
    D=A
    @i
    M=D  
(BLACK_LOOP)
    // if (i == 24575) goto END_ACTION
    @i
    D=M
    @24575
    D=A-D
    @END_ACTION
    D;JEQ
    
    // put black i
    @i
    A=M
    M=-1

    // increment i
    @i
    M=M+1
    @BLACK_LOOP
    0;JMP

(END_ACTION)
    // Go to start ask for input again
    @START   
    0;JMP

