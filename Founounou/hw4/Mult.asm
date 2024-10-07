// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.
//// Replace this comment with your code.


// Mult.asm
// Multiplies R0 and R1 and stores the result in R2.

@R0
D=M
@R2
M=0          // initialize result to 0

// Check if R0 is zero (terminate if true)
(LOOP)
@R0
D=M
@END
D;JEQ

// Add R1 to R2
@R1
D=M          // D = R1 (multiplier)
@R2
M=D+M        // R2 = R2 + R1

// Decrement R0
@R0
M=M-1        // R0 = R0 - 1

// Repeat loop
@LOOP
0;JMP        // Go back to the beginning of the loop

(END)
@END
0;JMP
