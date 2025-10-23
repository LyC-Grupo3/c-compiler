include macros2.asm
include number.asm

.MODEL LARGE  ; Modelo de Memoria
.386          ; Tipo de Procesador
.STACK 200h   ; Bytes en el Stack

MAXTEXTSIZE EQU 256


.DATA
; variables tabla simbolos
varInt dd ?
_666 dd 666.0
_4 dd 4.0
_cte_str_0 db "El valor es 4",'$', 13 dup (?)
_cte_str_1 db "El valor es distinto de 4",'$', 25 dup (?)
_cte_str_2 db "Fin del programa",'$', 16 dup (?)


.CODE
; ------------------------ utiles ------------------------ 
; devuelve en BX la cantidad de caracteres que tiene un string
; DS:SI apunta al string.
STRLEN PROC
	mov bx,0
STRL01:
	cmp BYTE PTR [SI+BX],'$'
	je STREND
	inc BX
	jmp STRL01
STREND:
	ret
STRLEN ENDP
; copia DS:SI a ES:DI; busca la cantidad de caracteres
COPIAR PROC
	call STRLEN
	cmp bx,MAXTEXTSIZE
	jle COPIARSIZEOK
	mov bx,MAXTEXTSIZE
COPIARSIZEOK:
	mov cx,bx
	cld
	rep movsb
	mov al,'$'
	mov BYTE PTR [DI],al
	ret
COPIAR ENDP

; ------------------------ mi programa ------------------------ 
START:
; inicializa el segmento de datos
	MOV AX,@DATA
	MOV DS,AX
	MOV es,ax ;

; asignacion
	FLD _666
	FSTP varInt
; condicional
	FLD _4
	FCOMP varInt
	FSTSW ax
	SAHF
	JNE ET_12
; funcion write
	displayString _cte_str_0
	newLine
; salto incondicional
	JMP ET_14
ET_12:
; funcion write
	displayString _cte_str_1
	newLine
ET_14:
; funcion write
	displayString _cte_str_2
	newLine


; indica que debe finalizar la ejecución
MOV AX,4C00H
INT 21H
END START
