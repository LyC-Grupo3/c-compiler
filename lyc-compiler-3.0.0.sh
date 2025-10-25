## Script para Unix
flex Lexico.l
bison -dyv Sintactico.y
gcc lex.yy.c y.tab.c utils/Sintactico.c utils/archivos.c utils/informes.c utils/simbolo.c utils/tabla_simbolos.c utils/utils.c utils/validacion.c utils/polaca.c utils/pila.c utils/helper_triangle.c utils/helper_control_flow.c utils/helper_init_variables.c utils/helper_tipos_datos.c utils/helper_operador_comparacion.c utils/helper_operador_logico.c utils/lista_expresiones.c utils/helper_equal_expressions.c utils/assembler.c -o compilador
./compilador prueba.txt
rm lex.yy.c
rm y.tab.c
rm y.output
rm y.tab.h
rm compilador