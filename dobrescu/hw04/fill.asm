(LOOP)
    //load keyboard address
    @24576
    D=M        

    @BLACK       
    D;JNE

    @WHITE   
    0;JMP

(BLACK)
    // fill screen with black 
    @16384
    //screen base address       
    D=A       

(FILL_BLACK_LOOP)
    A=D          
    M=-1         
    // set current screen address to black

    D=D+1        
    @24576
    //check if we've reached end of screen      
    D;JEQ       

    @FILL_BLACK_LOOP 
    0;JMP

(WHITE)
    //fill screen with white
    @16384       
    D=A          

(FILL_WHITE_LOOP)
    A=D          
    M=0          
    //set current address to white

    D=D+1        
    @24576       
    D;JEQ        

    @FILL_WHITE_LOOP
    0;JMP
