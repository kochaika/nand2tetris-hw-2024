(FILL_WHITE) 
    @SCREEN
    D=A
    @offset
    D=D+M
    A=D

    M=0
    
    @offset
    M=M+1
    
    @offset
    D=M
    @8192
    D=D-A

    @LOOP
    D;JEQ

    @FILL_WHITE
    0;JMP

(FILL_BLACK)

    @SCREEN
    D=A
    @offset
    D=D+M
    A=D

    M=-1
    
    @offset
    M=M+1
    
    @offset
    D=M
    @8192
    D=D-A

    @LOOP
    D;JEQ

    @FILL_BLACK
    0;JMP

(LOOP)
    @offset
    M=0
    @KBD
    D=M
    @0
    D=D|A

    @FILL_WHITE
    D;JEQ
    @FILL_BLACK
    0;JMP
