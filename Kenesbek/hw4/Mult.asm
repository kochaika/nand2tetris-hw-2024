// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

@2              // Prepare R2 to store the result
M=0              // Initialize R2 to 0
@counter
M=0              // Initialize counter (i) to 0

(START)
    @counter     // Load the counter value
    D=M          // D = counter
    @0           // Load R0
    D=D-M        // D = counter - R0
    @HALT
    D;JGE         // If counter >= R0, jump to HALT

    @1           // Load R1 (the multiplicand)
    D=M          // D = R1
    @2           // Address for R2
    M=D+M        // Add R1 to R2 (R2 = R2 + R1)
    @counter     // Update counter
    M=M+1        // Increment counter
    @START       // Loop back to the start
    0;JMP        // Jump back

(HALT)
    @HALT
    0;JMP        // Infinite loop