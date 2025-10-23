include macros2.asm
include number.asm

.MODEL LARGE  ; Modelo de Memoria
.386          ; Tipo de Procesador
.STACK 200h   ; Bytes en el Stack

MAXTEXTSIZE EQU 256


.DATA
; variables tabla simbolos
varString db MAXTEXTSIZE dup (?),'$'
varInt dd ?
varFloat dd ?
_cte_str_0 db "Hola",'$', 4 dup (?)


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
	MOV SI, OFFSET _cte_str_0
	MOV DI, OFFSET varString
	CALL COPIAR
; funcion write
	displayString varString
	newLine


; indica que debe finalizar la ejecución
MOV AX,4C00H
INT 21H
END START
