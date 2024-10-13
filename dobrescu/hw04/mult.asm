@0
D=A
@i
M=D

@R1
D=M
//R2 will store the result temporarily
@R2
M=0

(LOOP)
    //if (i==R1) goto END
    @i      
    D=M
    @R1
    D=D-M
    @END
    D;JEQ

    //R2=R2+R0
    @R0
    D=M
    @R2
    M=D+M

    //i=i+1
    @i
    M=M+1
    //goto LOOP
    @LOOP
    0;JMP

(END)
    @R2
    D=M
    @R1
    M=D
    @END
    0;JMP

