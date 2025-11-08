#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/polaca.h"
#include "utils/tabla_simbolos.h"

void generar_asm(const char* output_file);
void write_data_section(FILE* fp);
void write_code_section(FILE* fp);
void process_intermediate_code(FILE* fp);
void write_header(FILE* fp);
void write_footer(FILE* fp);

#endif