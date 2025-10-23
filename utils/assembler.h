#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include "./tabla_simbolos.h"
#include "./polaca.h"
#include "./pila.h"

#define VALOR_ASM_OP_IGUAL "JNE"
#define VALOR_ASM_OP_DISTINTO "JE"
#define VALOR_ASM_OP_MAYOR "JBE"
#define VALOR_ASM_OP_MAYOR_IGUAL "JB"
#define VALOR_ASM_OP_MENOR "JAE"
#define VALOR_ASM_OP_MENOR_IGUAL "JA"

void generarAssembler();
void escribirCabecera(FILE *archivo);
void escribirSegmentoData(FILE *archivo, t_tabla_simbolos *tabla);
void escribirSegmentCode(FILE *archivo, t_polaca *polaca);
void escribirSegmentoEnd(FILE *archivo);

int esOperando(t_nodo_polaca *celda);
char *getInstruccionASMOperador(t_nodo_polaca *celda);
void escribirASMOperacionAritmetica(FILE *archivo, const char *operadorASM);
void escribirASMAsignacion(FILE *archivo);
void escribirASMEtiquetaSalto(FILE *archivo, const char *contenidoCelda);
FILE *escribirVariablesAuxiliaresASM(FILE *archivo);
void escribirASMFuncionRead(FILE *archivo);
void escribirASMFuncionABS(FILE *archivo);
void escribirASMFuncionWrite(FILE *archivo);
void eliminarEtiquetaDeCelda(char *contenidoCelda);
void escribirASMSaltoIncondicional(FILE *archivo, const char *contenidoCelda);
void escribirASMSaltoCondicional(FILE *archivo, const char *operadorComparacion, const char *nroSalto);
void formatearPolacaParaAssembler(t_polaca *polaca);
void actualizarContenidoCeldaPorNombreSimbolo(t_nodo_polaca *celda, const char *valorSimboloBuscar);
char *limpiarComillas(char *str);
int esCeldaNumero(t_nodo_polaca *celda);
int esCeldaString(t_nodo_polaca *celda);
int esCeldaSalto(t_nodo_polaca *celda);

#endif // ASSEMBLER_H