#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include "./tabla_simbolos.h"
#include "./polaca.h"

void generarAssembler();
void escribirCabecera(FILE *archivo);
void escribirSegmentoData(FILE *archivo, t_tabla_simbolos *tabla);
void escribirSegmentCode(FILE *archivo);
void escribirSegmentoEnd(FILE *archivo);

void formatearPolacaParaAssembler(t_polaca *polaca);
void actualizarContenidoCeldaPorNombreSimbolo(t_nodo_polaca *celda, const char *valorSimboloBuscar);
char *limpiarComillas(char *str);
int esCeldaNumero(t_nodo_polaca *celda);
int esCeldaString(t_nodo_polaca *celda);
int esCeldaSalto(t_nodo_polaca *celda);

#endif // ASSEMBLER_H