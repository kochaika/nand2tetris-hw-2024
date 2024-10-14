// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, the
// program clears the screen, i.e. writes "white" in every pixel.

// Put your code here.

(START)
@SCREEN
D=A
@0
M=D

@KBD
D=M
@BLACK
D;JGT
@WHITE
D;JEQ

///////////////////////////
(BLACK)
@1
M=-1	//WHAT TO FILL SCREEN WITH (-1=11111111111111)
@FILL
0;JMP

(WHITE)
@1
M=0	//WHAT TO FILL SCREEN WITH
@FILL
0;JMP

//////////////////////////

(FILL)
@1
D=M

@0
A=M
M=D

@0
D=M+1
@24576
D=A-D

@0
M=M+1
A=M

@CHANGE
D;JGT
/////////////////////////
@START
0;JMP
