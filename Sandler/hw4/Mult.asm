@mask
M=1

// a = R0
@R0
D=M
@a
M=D

// b = R1
@R1
D=M
@b
M=D

// R2 = 0
@R2
M=0

(LOOP)
    // if (mask == 32768) goto END
    @mask
    D=M
    @32768
    D=D-A
    @END
    D;JEQ

    // if (a & mask == 0) goto SKIP_ADD
    @a
    D=M
    @mask
    D=D&M
    @SKIP_ADD
    D;JEQ

    // R2 = R2 + b 
    @b
    D=M
    @R2
    D=D+M
    @R2
    M=D
    (SKIP_ADD)
    
    // mask = mask + mask
    @mask 
    D=M
    @mask
    M=D+M

    // b = b + b
    @b
    D=M
    @b
    M=D+M
    @LOOP
    0;JMP
(END)
    @END
    0;JMP