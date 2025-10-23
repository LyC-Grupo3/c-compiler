#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include "./tabla_simbolos.h"
#include "./polaca.h"
#include "./pila.h"

void generarAssembler();
void escribirCabecera(FILE *archivo);
void escribirSegmentoData(FILE *archivo, t_tabla_simbolos *tabla);
void escribirSegmentCode(FILE *archivo, t_polaca *polaca);
void escribirSegmentoEnd(FILE *archivo);

int esOperando(t_nodo_polaca *celda);
char *getInstruccionASMOperador(t_nodo_polaca *celda);
void escribirASMOperacionAritmetica(FILE *archivo, const char *operadorASM);
void escribirASMAsignacion(FILE *archivo);

void formatearPolacaParaAssembler(t_polaca *polaca);
void actualizarContenidoCeldaPorNombreSimbolo(t_nodo_polaca *celda, const char *valorSimboloBuscar);
char *limpiarComillas(char *str);
int esCeldaNumero(t_nodo_polaca *celda);
int esCeldaString(t_nodo_polaca *celda);
int esCeldaSalto(t_nodo_polaca *celda);

#endif // ASSEMBLER_H