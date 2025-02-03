MODEL SMALL
STACK 256
CODESEG

get_char	macro
  	mov	ah,1h
        int	21h
endm

operations	macro num1, num2
	add	ax, num1
	sub	ax, num2
endm

Start:

	mov ax,@data
 	mov ds,ax
  	mov	ah,1h
        int	21h
	add	ax, 9
	sub	ax, 5
Exit:
	CALL ExitProgramm

End Start


