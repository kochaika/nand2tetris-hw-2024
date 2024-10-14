@16384
D=A
@SCREEN
M=D

@24576
D=A
@KEYBOARD
M=D

(LOOP)
    @KEYBOARD
    D=M
    @CLEAR_SCREEN
    D;JEQ

    @BLACKEN_SCREEN
    0;JMP

(BLACKEN_SCREEN)
    @SCREEN
    D=M
    @24576
    D=A
    @BLACK_LOOP
    0;JMP

(BLACK_LOOP)
    @SCREEN
    A=M
    M=-1
    @SCREEN
    M=M+1
    @24576
    D=M
    @BLACK_LOOP
    D;JLT
    @LOOP
    0;JMP

(CLEAR_SCREEN)
    @SCREEN
    D=M
    @16384
    D=A
    @CLEAR_LOOP
    0;JMP

(CLEAR_LOOP)
    @SCREEN
    A=M
    M=0
    @SCREEN
    M=M+1
    @24576
    D=M
    @CLEAR_LOOP
    D;JLT
    @LOOP
    0;JMP
