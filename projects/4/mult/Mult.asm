// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

// Initialize i to 0
	@i
	M=0
	@R2
	M=0
// Start Looping
(MULT)
	// If (i == R0) goto END
	@i
	D=M
	@R0
	D=D-M
	@END
	D;JEQ

	// Start Multiplication
	@R1
	D=M
	@R2
	M=M+D
	
	// i = i + 1
	@i
	M=M+1

	// Goto loop
	@MULT
	0;JMP
(END)
	@END
	0;JMP
			


