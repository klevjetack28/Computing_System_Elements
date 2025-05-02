// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

// Sizes
	@256
	D=A
	@rows
	M=D
	@32
	D=A
	@cols
	M=D	

// Indexes
	@rowIndex
	M=0
	@colIndex
	M=0

// Start loop
(LOOP)
	// Check key is pressed
	@KBD
	D=M
	@CONTINUE
	D;JEQ

	@rowPos
	M=0

(ROWS)
	// If (rowIndex == rows) goto END_ROWS
	@rowIndex
	D=M
	@rows
	D=D-M
	@END_ROWS
	D;JEQ

(COLS)	
	// If (colIndex == cols) goto END_COLS
	@colIndex
	D=M
	@cols
	D=D-M
	@END_COLS
	D;JEQ

	// Turn column index black
	@rowPos
	D=M
	@colIndex
	D=D+M
	@SCREEN
	A=D+A
	M=-1

	// colIndex = colIndex + 1
	@colIndex
	M=M+1

	// Jump to start
	@COLS
	0;JMP

(END_COLS)
	// Reset column index
	@colIndex
	M=0;

	// rowPos = rowPos + 32
	@32	
	D=A
	@rowPos
	M=M+D

	// rowIndex = rowIndex + 1
	@rowIndex
	M=M+1

	// Jump to start
	@ROWS
	0;JMP

(END_ROWS)
	// Reset row index
	@rowIndex
	M=0;		

(CONTINUE)
	// Jump to LOOP
	@LOOP
	0;JMP
