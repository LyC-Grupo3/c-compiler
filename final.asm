include macros2.asm
include number.asm

.MODEL LARGE  ; Modelo de Memoria
.386          ; Tipo de Procesador
.STACK 200h   ; Bytes en el Stack

MAXTEXTSIZE EQU 256


.DATA
; variables tabla simbolos
varFloat dd ?
varFloat1 dd ?
varFloat2 dd ?
a dd ?
b dd ?
c dd ?
d dd ?
e dd ?
f dd ?
g dd ?
h dd ?
i dd ?
j dd ?
k dd ?
l dd ?
m dd ?
n dd ?
o dd ?
p dd ?
varInt dd ?
varInt1 dd ?
varInt2 dd ?
varFloat3 dd ?
varFloat4 dd ?
varString db MAXTEXTSIZE dup (?),'$'
varString1 db MAXTEXTSIZE dup (?),'$'
varString2 db MAXTEXTSIZE dup (?),'$'
resultEqualExpressions dd ?
resultAreamax dd ?
x1 dd ?
y1 dd ?
x2 dd ?
y2 dd ?
x3 dd ?
y3 dd ?
a1 dd ?
b1 dd ?
a2 dd ?
b2 dd ?
a3 dd ?
b3 dd ?
_1 dd 1.0
_2 dd 2.0
_99999_99 dd 99999.99
_99_0 dd 99.0
_0_9999 dd 0.9999
_cte_str_0 db "string repetido",'$', 15 dup (?)
_cte_str_2 db "string diferente",'$', 16 dup (?)
_27 dd 27.0
_500 dd 500.0
_4_0 dd 4.0
_34_4 dd 34.4
_3_1 dd 3.1
_0 dd 0.0
_cte_str_3 db "1. verdad IF simple",'$', 19 dup (?)
_cte_str_4 db "1. verdad IF-ELSE simple",'$', 24 dup (?)
_cte_str_5 db "falso IF-ELSE simple",'$', 20 dup (?)
_cte_str_6 db "verdad IF-ELSE simple",'$', 21 dup (?)
_cte_str_7 db "2. falso IF-ELSE simple",'$', 23 dup (?)
_cte_str_8 db "3. verdad IF AND",'$', 16 dup (?)
_cte_str_9 db "4. verdad IF-ELSE AND",'$', 21 dup (?)
_cte_str_10 db "falso IF-ELSE AND",'$', 17 dup (?)
_cte_str_11 db "verdad IF-ELSE AND",'$', 18 dup (?)
_cte_str_12 db "5. falso IF-ELSE AND",'$', 20 dup (?)
_cte_str_13 db "6. verdad OR",'$', 12 dup (?)
_cte_str_14 db "7. verdad IF-ELSE OR",'$', 20 dup (?)
_cte_str_15 db "falso IF-ELSE OR",'$', 16 dup (?)
_cte_str_16 db "verdad IF-ELSE OR",'$', 17 dup (?)
_cte_str_17 db "8. falso IF-ELSE OR",'$', 19 dup (?)
_cte_str_18 db "9. dentro WHILE simple",'$', 22 dup (?)
_cte_str_19 db "10. dentro WHILE compuesto AND",'$', 30 dup (?)
_cte_str_20 db "11. dentro WHILE compuesto OR",'$', 29 dup (?)
_cte_str_21 db "12. negado SIMPLE",'$', 17 dup (?)
_9999 dd 9999.0
_cte_str_22 db "13. dentro WHILE compuesto NOT",'$', 30 dup (?)
_cte_str_23 db "14. true a>b",'$', 12 dup (?)
_cte_str_24 db "15. true anidado",'$', 16 dup (?)
_cte_str_25 db "16. dentro WHILE anidado",'$', 24 dup (?)
_123_456 dd 123.456
_cte_str_26 db "17. write varFloat valor: ",'$', 26 dup (?)
_789 dd 789.0
_cte_str_27 db "18. write varInt valor: ",'$', 24 dup (?)
_cte_str_28 db "cadena",'$', 6 dup (?)
_cte_str_29 db "19. write varString valor: ",'$', 27 dup (?)
_cte_str_30 db "20. write cadena literal",'$', 24 dup (?)
_1_5 dd 1.5
@expActual dd ?
@expAux dd ?
@equal dd ?
_cte_str_31 db "21. EXPRESIONES IGUALES",'$', 23 dup (?)
_cte_str_32 db "EXPRESIONES DISTINTAS",'$', 21 dup (?)
_100 dd 100.0
_cte_str_33 db "22. EXPRESIONES IGUALES",'$', 23 dup (?)
_n2 dd -2.0
_3 dd 3.0
_5 dd 5.0
@area_t1 dd ?
@area_t2 dd ?
@result_max dd ?
_8_5 dd 8.5
_cte_str_35 db "23. Resultado correcto triangleAreaMaximum",'$', 42 dup (?)
_cte_str_36 db "Resultado incorrecto triangleAreaMaximum",'$', 40 dup (?)
_cte_str_37 db "24. Resultado correcto triangleAreaMaximum",'$', 42 dup (?)
_cte_str_39 db "17. Ingrese texto para varInt1",'$', 30 dup (?)
_cte_str_40 db "18. Valor ingresado:",'$', 20 dup (?)

; variables auxiliares para resultados intermedios de operaciones aritméticas
@aux0 dd ?
@aux1 dd ?
@aux2 dd ?
@aux3 dd ?
@aux4 dd ?
@aux5 dd ?
@aux6 dd ?
@aux7 dd ?
@aux8 dd ?
@aux9 dd ?
@aux10 dd ?
@aux11 dd ?
@aux12 dd ?
@aux13 dd ?
@aux14 dd ?
@aux15 dd ?
@aux16 dd ?
@aux17 dd ?
@aux18 dd ?
@aux19 dd ?
@aux20 dd ?
@aux21 dd ?
@aux22 dd ?
@aux23 dd ?
@aux24 dd ?
@aux25 dd ?
@aux26 dd ?
@aux27 dd ?
@aux28 dd ?
@aux29 dd ?
@aux30 dd ?
@aux31 dd ?
@aux32 dd ?
@aux33 dd ?
@aux34 dd ?
@aux35 dd ?
@aux36 dd ?
@aux37 dd ?
@aux38 dd ?
@aux39 dd ?
@aux40 dd ?
@aux41 dd ?
@aux42 dd ?
@aux43 dd ?
@aux44 dd ?
@aux45 dd ?
@aux_abs dd ?
@aux46 dd ?


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
	FLD _1
	FSTP varInt1
; asignacion
	FLD _1
	FSTP varInt1
; asignacion
	FLD _2
	FSTP varInt2
; asignacion
	FLD _99999_99
	FSTP varFloat1
; asignacion
	FLD _99999_99
	FSTP varFloat1
; asignacion
	FLD _99_0
	FSTP varFloat2
; asignacion
	FLD _99_0
	FSTP varFloat2
; asignacion
	FLD _0_9999
	FSTP varFloat3
; asignacion
	MOV SI, OFFSET _cte_str_0
	MOV DI, OFFSET varString1
	CALL COPIAR
; asignacion
	MOV SI, OFFSET _cte_str_0
	MOV DI, OFFSET varString1
	CALL COPIAR
; asignacion
	MOV SI, OFFSET _cte_str_2
	MOV DI, OFFSET varString2
	CALL COPIAR
; operacion arimetica
	FLD _27
	FLD varInt2
	FSUB
	FSTP @aux0
; asignacion
	FLD @aux0
	FSTP varInt1
; operacion arimetica
	FLD _500
	FLD varInt1
	FADD
	FSTP @aux1
; asignacion
	FLD @aux1
	FSTP varInt2
; operacion arimetica
	FLD varFloat2
	FLD _4_0
	FDIV
	FSTP @aux2
; asignacion
	FLD @aux2
	FSTP varFloat1
; operacion arimetica
	FLD _3_1
	FLD _34_4
	FMUL
	FSTP @aux3
; asignacion
	FLD @aux3
	FSTP varFloat2
; asignacion
	FLD _1
	FSTP a
; asignacion
	FLD _0
	FSTP b
; condicional
	FLD a
	FCOMP b
	FSTSW ax
	SAHF
	JBE ET_66
; funcion write
	displayString _cte_str_3
	newLine
ET_66:
; asignacion
	FLD _1
	FSTP a
; asignacion
	FLD _0
	FSTP b
; condicional
	FLD a
	FCOMP b
	FSTSW ax
	SAHF
	JBE ET_81
; funcion write
	displayString _cte_str_4
	newLine
; salto incondicional
	JMP ET_83
ET_81:
; funcion write
	displayString _cte_str_5
	newLine
ET_83:
; asignacion
	FLD b
	FSTP a
; condicional
	FLD a
	FCOMP b
	FSTSW ax
	SAHF
	JBE ET_95
; funcion write
	displayString _cte_str_6
	newLine
; salto incondicional
	JMP ET_97
ET_95:
; funcion write
	displayString _cte_str_7
	newLine
ET_97:
; asignacion
	FLD _1
	FSTP a
; asignacion
	FLD _0
	FSTP b
; asignacion
	FLD _1
	FSTP c
; asignacion
	FLD _0
	FSTP d
; condicional
	FLD a
	FCOMP b
	FSTSW ax
	SAHF
	JBE ET_121
; condicional
	FLD c
	FCOMP b
	FSTSW ax
	SAHF
	JBE ET_121
; funcion write
	displayString _cte_str_8
	newLine
ET_121:
; asignacion
	FLD _1
	FSTP a
; asignacion
	FLD _0
	FSTP b
; asignacion
	FLD _1
	FSTP c
; asignacion
	FLD _0
	FSTP d
; condicional
	FLD a
	FCOMP b
	FSTSW ax
	SAHF
	JBE ET_147
; condicional
	FLD c
	FCOMP b
	FSTSW ax
	SAHF
	JBE ET_147
; funcion write
	displayString _cte_str_9
	newLine
; salto incondicional
	JMP ET_149
ET_147:
; funcion write
	displayString _cte_str_10
	newLine
ET_149:
; asignacion
	FLD b
	FSTP c
; condicional
	FLD a
	FCOMP b
	FSTSW ax
	SAHF
	JBE ET_166
; condicional
	FLD c
	FCOMP b
	FSTSW ax
	SAHF
	JBE ET_166
; funcion write
	displayString _cte_str_11
	newLine
; salto incondicional
	JMP ET_168
ET_166:
; funcion write
	displayString _cte_str_12
	newLine
ET_168:
; asignacion
	FLD _0
	FSTP a
; asignacion
	FLD _1
	FSTP b
; asignacion
	FLD _1
	FSTP c
; asignacion
	FLD _0
	FSTP d
; condicional
	FLD a
	FCOMP b
	FSTSW ax
	SAHF
	JBE ET_187
; salto incondicional
	JMP ET_192
ET_187:
; condicional
	FLD c
	FCOMP d
	FSTSW ax
	SAHF
	JBE ET_194
ET_192:
; funcion write
	displayString _cte_str_13
	newLine
ET_194:
; asignacion
	FLD _0
	FSTP a
; asignacion
	FLD _1
	FSTP b
; asignacion
	FLD _1
	FSTP c
; asignacion
	FLD _0
	FSTP d
; condicional
	FLD a
	FCOMP b
	FSTSW ax
	SAHF
	JBE ET_213
; salto incondicional
	JMP ET_218
ET_213:
; condicional
	FLD c
	FCOMP d
	FSTSW ax
	SAHF
	JBE ET_222
ET_218:
; funcion write
	displayString _cte_str_14
	newLine
; salto incondicional
	JMP ET_224
ET_222:
; funcion write
	displayString _cte_str_15
	newLine
ET_224:
; asignacion
	FLD b
	FSTP a
; asignacion
	FLD d
	FSTP c
; condicional
	FLD a
	FCOMP b
	FSTSW ax
	SAHF
	JBE ET_237
; salto incondicional
	JMP ET_242
ET_237:
; condicional
	FLD c
	FCOMP d
	FSTSW ax
	SAHF
	JBE ET_246
ET_242:
; funcion write
	displayString _cte_str_16
	newLine
; salto incondicional
	JMP ET_248
ET_246:
; funcion write
	displayString _cte_str_17
	newLine
ET_248:
; asignacion
	FLD _1
	FSTP a
; asignacion
	FLD _0
	FSTP b
ET_254:
; condicional
	FLD a
	FCOMP b
	FSTSW ax
	SAHF
	JBE ET_267
; funcion write
	displayString _cte_str_18
	newLine
; asignacion
	FLD a
	FSTP b
; salto incondicional
	JMP ET_254
ET_267:
; asignacion
	FLD _1
	FSTP a
; asignacion
	FLD _0
	FSTP b
; asignacion
	FLD _1
	FSTP c
; asignacion
	FLD _0
	FSTP d
ET_279:
; condicional
	FLD a
	FCOMP b
	FSTSW ax
	SAHF
	JBE ET_300
; condicional
	FLD c
	FCOMP d
	FSTSW ax
	SAHF
	JBE ET_300
; funcion write
	displayString _cte_str_19
	newLine
; asignacion
	FLD a
	FSTP b
; asignacion
	FLD c
	FSTP d
; salto incondicional
	JMP ET_279
ET_300:
; asignacion
	FLD _0
	FSTP a
; asignacion
	FLD _1
	FSTP b
; asignacion
	FLD _1
	FSTP c
; asignacion
	FLD _0
	FSTP d
ET_312:
; condicional
	FLD a
	FCOMP b
	FSTSW ax
	SAHF
	JBE ET_320
; salto incondicional
	JMP ET_325
ET_320:
; condicional
	FLD c
	FCOMP d
	FSTSW ax
	SAHF
	JBE ET_332
ET_325:
; funcion write
	displayString _cte_str_20
	newLine
; asignacion
	FLD d
	FSTP c
; salto incondicional
	JMP ET_312
ET_332:
; asignacion
	FLD _0
	FSTP a
; asignacion
	FLD _1
	FSTP b
; condicional
	FLD a
	FCOMP b
	FSTSW ax
	SAHF
	JA ET_348
; funcion write
	displayString _cte_str_21
	newLine
; asignacion
	FLD _9999
	FSTP a
ET_348:
; asignacion
	FLD _0
	FSTP a
; asignacion
	FLD _1
	FSTP b
ET_354:
; condicional
	FLD a
	FCOMP b
	FSTSW ax
	SAHF
	JA ET_367
; funcion write
	displayString _cte_str_22
	newLine
; asignacion
	FLD _9999
	FSTP a
; salto incondicional
	JMP ET_354
ET_367:
; asignacion
	FLD _1
	FSTP a
; asignacion
	FLD _0
	FSTP b
; asignacion
	FLD _1
	FSTP c
; asignacion
	FLD _0
	FSTP d
; condicional
	FLD a
	FCOMP b
	FSTSW ax
	SAHF
	JBE ET_406
; funcion write
	displayString _cte_str_23
	newLine
; condicional
	FLD c
	FCOMP d
	FSTSW ax
	SAHF
	JBE ET_393
; funcion write
	displayString _cte_str_24
	newLine
ET_393:
; condicional
	FLD e
	FCOMP f
	FSTSW ax
	SAHF
	JAE ET_406
; funcion write
	displayString _cte_str_25
	newLine
; asignacion
	FLD f
	FSTP e
; salto incondicional
	JMP ET_393
ET_406:
; asignacion
	FLD _123_456
	FSTP varFloat
; funcion write
	displayString _cte_str_26
	newLine
; funcion write
	DisplayFloat varFloat, 2
	newLine
; asignacion
	FLD _789
	FSTP varInt
; funcion write
	displayString _cte_str_27
	newLine
; funcion write
	DisplayFloat varInt, 0
	newLine
; asignacion
	MOV SI, OFFSET _cte_str_28
	MOV DI, OFFSET varString
	CALL COPIAR
; funcion write
	displayString _cte_str_29
	newLine
; funcion write
	displayString varString
	newLine
; funcion write
	displayString _cte_str_30
	newLine
; operacion arimetica
	FLD _1_5
	FLD _1_5
	FADD
	FSTP @aux4
; asignacion
	FLD @aux4
	FSTP @expActual
; operacion arimetica
	FLD _2
	FLD _1
	FADD
	FSTP @aux5
; asignacion
	FLD @aux5
	FSTP @expAux
; condicional
	FLD @expActual
	FCOMP @expAux
	FSTSW ax
	SAHF
	JE ET_449
; asignacion
	FLD _0
	FSTP @equal
; salto incondicional
	JMP ET_452
ET_449:
; asignacion
	FLD _1
	FSTP @equal
ET_452:
; asignacion
	FLD @equal
	FSTP resultEqualExpressions
; condicional
	FLD resultEqualExpressions
	FCOMP _1
	FSTSW ax
	SAHF
	JNE ET_464
; funcion write
	displayString _cte_str_31
	newLine
; salto incondicional
	JMP ET_466
ET_464:
; funcion write
	displayString _cte_str_32
	newLine
ET_466:
; operacion arimetica
	FLD _2
	FLD _1
	FADD
	FSTP @aux6
; asignacion
	FLD @aux6
	FSTP @expActual
; operacion arimetica
	FLD _100
	FLD _1
	FADD
	FSTP @aux7
; asignacion
	FLD @aux7
	FSTP @expAux
; condicional
	FLD @expActual
	FCOMP @expAux
	FSTSW ax
	SAHF
	JE ET_511
; operacion arimetica
	FLD _1_5
	FLD _1_5
	FADD
	FSTP @aux8
; asignacion
	FLD @aux8
	FSTP @expAux
; condicional
	FLD @expActual
	FCOMP @expAux
	FSTSW ax
	SAHF
	JE ET_511
; operacion arimetica
	FLD _100
	FLD _1
	FADD
	FSTP @aux9
; asignacion
	FLD @aux9
	FSTP @expActual
; operacion arimetica
	FLD _1_5
	FLD _1_5
	FADD
	FSTP @aux10
; asignacion
	FLD @aux10
	FSTP @expAux
; condicional
	FLD @expActual
	FCOMP @expAux
	FSTSW ax
	SAHF
	JE ET_511
; asignacion
	FLD _0
	FSTP @equal
; salto incondicional
	JMP ET_514
ET_511:
; asignacion
	FLD _1
	FSTP @equal
ET_514:
; asignacion
	FLD @equal
	FSTP resultEqualExpressions
; condicional
	FLD resultEqualExpressions
	FCOMP _1
	FSTSW ax
	SAHF
	JNE ET_526
; funcion write
	displayString _cte_str_33
	newLine
; salto incondicional
	JMP ET_528
ET_526:
; funcion write
	displayString _cte_str_32
	newLine
ET_528:
; operacion arimetica
	FLD _1
	FLD _1
	FSUB
	FSTP @aux11
; operacion arimetica
	FLD _1
	FLD @aux11
	FMUL
	FSTP @aux12
; operacion arimetica
	FLD _1
	FLD _1
	FSUB
	FSTP @aux13
; operacion arimetica
	FLD _1
	FLD @aux13
	FMUL
	FSTP @aux14
; operacion arimetica
	FLD @aux14
	FLD @aux12
	FADD
	FSTP @aux15
; operacion arimetica
	FLD _1
	FLD _1
	FSUB
	FSTP @aux16
; operacion arimetica
	FLD _1
	FLD @aux16
	FMUL
	FSTP @aux17
; operacion arimetica
	FLD @aux17
	FLD @aux15
	FADD
	FSTP @aux18
; funcion abs
	FLD @aux18
	FABS
	FSTP @aux_abs
; operacion arimetica
	FLD @aux_abs
	FLD _2
	FDIV
	FSTP @aux19
; asignacion
	FLD @aux19
	FSTP @area_t1
; operacion arimetica
	FLD _2
	FLD _5
	FSUB
	FSTP @aux20
; operacion arimetica
	FLD _n2
	FLD @aux20
	FMUL
	FSTP @aux21
; operacion arimetica
	FLD _5
	FLD _1
	FSUB
	FSTP @aux22
; operacion arimetica
	FLD _3
	FLD @aux22
	FMUL
	FSTP @aux23
; operacion arimetica
	FLD @aux23
	FLD @aux21
	FADD
	FSTP @aux24
; operacion arimetica
	FLD _1
	FLD _2
	FSUB
	FSTP @aux25
; operacion arimetica
	FLD _1
	FLD @aux25
	FMUL
	FSTP @aux26
; operacion arimetica
	FLD @aux26
	FLD @aux24
	FADD
	FSTP @aux27
; funcion abs
	FLD @aux27
	FABS
	FSTP @aux_abs
; operacion arimetica
	FLD @aux_abs
	FLD _2
	FDIV
	FSTP @aux28
; asignacion
	FLD @aux28
	FSTP @area_t2
; condicional
	FLD @area_t1
	FCOMP @area_t2
	FSTSW ax
	SAHF
	JBE ET_582
; asignacion
	FLD @area_t1
	FSTP @result_max
; salto incondicional
	JMP ET_585
ET_582:
; asignacion
	FLD @area_t2
	FSTP @result_max
ET_585:
; asignacion
	FLD @result_max
	FSTP resultAreamax
; condicional
	FLD resultAreamax
	FCOMP _8_5
	FSTSW ax
	SAHF
	JNE ET_599
; funcion write
	displayString _cte_str_35
	newLine
; funcion write
	DisplayFloat resultAreamax, 2
	newLine
; salto incondicional
	JMP ET_603
ET_599:
; funcion write
	displayString _cte_str_36
	newLine
; funcion write
	DisplayFloat resultAreamax, 2
	newLine
ET_603:
; operacion arimetica
	FLD _1
	FLD _1
	FSUB
	FSTP @aux29
; operacion arimetica
	FLD _1
	FLD @aux29
	FMUL
	FSTP @aux30
; operacion arimetica
	FLD _1
	FLD _1
	FSUB
	FSTP @aux31
; operacion arimetica
	FLD _1
	FLD @aux31
	FMUL
	FSTP @aux32
; operacion arimetica
	FLD @aux32
	FLD @aux30
	FADD
	FSTP @aux33
; operacion arimetica
	FLD _1
	FLD _1
	FSUB
	FSTP @aux34
; operacion arimetica
	FLD _1
	FLD @aux34
	FMUL
	FSTP @aux35
; operacion arimetica
	FLD @aux35
	FLD @aux33
	FADD
	FSTP @aux36
; funcion abs
	FLD @aux36
	FABS
	FSTP @aux_abs
; operacion arimetica
	FLD @aux_abs
	FLD _2
	FDIV
	FSTP @aux37
; asignacion
	FLD @aux37
	FSTP @area_t1
; operacion arimetica
	FLD _2
	FLD _5
	FSUB
	FSTP @aux38
; operacion arimetica
	FLD _n2
	FLD @aux38
	FMUL
	FSTP @aux39
; operacion arimetica
	FLD _5
	FLD _1
	FSUB
	FSTP @aux40
; operacion arimetica
	FLD _3
	FLD @aux40
	FMUL
	FSTP @aux41
; operacion arimetica
	FLD @aux41
	FLD @aux39
	FADD
	FSTP @aux42
; operacion arimetica
	FLD _1
	FLD _2
	FSUB
	FSTP @aux43
; operacion arimetica
	FLD _1
	FLD @aux43
	FMUL
	FSTP @aux44
; operacion arimetica
	FLD @aux44
	FLD @aux42
	FADD
	FSTP @aux45
; funcion abs
	FLD @aux45
	FABS
	FSTP @aux_abs
; operacion arimetica
	FLD @aux_abs
	FLD _2
	FDIV
	FSTP @aux46
; asignacion
	FLD @aux46
	FSTP @area_t2
; condicional
	FLD @area_t1
	FCOMP @area_t2
	FSTSW ax
	SAHF
	JBE ET_657
; asignacion
	FLD @area_t1
	FSTP @result_max
; salto incondicional
	JMP ET_660
ET_657:
; asignacion
	FLD @area_t2
	FSTP @result_max
ET_660:
; asignacion
	FLD @result_max
	FSTP resultAreamax
; condicional
	FLD resultAreamax
	FCOMP _8_5
	FSTSW ax
	SAHF
	JNE ET_674
; funcion write
	displayString _cte_str_37
	newLine
; funcion write
	DisplayFloat resultAreamax, 2
	newLine
; salto incondicional
	JMP ET_678
ET_674:
; funcion write
	displayString _cte_str_36
	newLine
; funcion write
	DisplayFloat resultAreamax, 2
	newLine
ET_678:
; funcion write
	displayString _cte_str_39
	newLine
; funcion read
	GetFloat varInt1
	newLine
; funcion write
	displayString _cte_str_40
	newLine
; funcion write
	DisplayFloat varInt1, 0
	newLine


; indica que debe finalizar la ejecución
MOV AX,4C00H
INT 21H
END START
