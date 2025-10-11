#include "./helper_operador_comparacion.h"

char operadorComparacionActual[50];
int negacionPendienteOperadorComparacionActual = 0;

// Para regla operador_comparacion
void setOperadorComparacionPendienteActual(char *operador)
{
    strncpy(operadorComparacionActual, operador, sizeof(operadorComparacionActual) - 1);
    operadorComparacionActual[sizeof(operadorComparacionActual) - 1] = '\0';
}

char *getOperadorComparacionPendienteActual()
{
    return (char *)operadorComparacionActual;
}

// Para regla condicion - negacion operadores de comparacion
void setNegacionPendienteOperadorComparacion(int valor)
{
    negacionPendienteOperadorComparacionActual = valor;
}

int hayQueNegarOperadorComparacionActual()
{
    return negacionPendienteOperadorComparacionActual;
}

void negarOperadorOperacionActual()
{
    if (strcmp(operadorComparacionActual, "BEQ") == 0)
    {
        strcpy(operadorComparacionActual, "BNE");
        return;
    }

    if (strcmp(operadorComparacionActual, "BNE") == 0)
    {
        strcpy(operadorComparacionActual, "BEQ");
        return;
    }

    if (strcmp(operadorComparacionActual, "BLE") == 0)
    {
        strcpy(operadorComparacionActual, "BGT");
        return;
    }

    if (strcmp(operadorComparacionActual, "BLT") == 0)
    {
        strcpy(operadorComparacionActual, "BGE");
        return;
    }

    if (strcmp(operadorComparacionActual, "BGE") == 0)
    {
        strcpy(operadorComparacionActual, "BLT");
        return;
    }

    if (strcmp(operadorComparacionActual, "BGT") == 0)
    {
        strcpy(operadorComparacionActual, "BLE");
        return;
    }
}

/*
Caso prueba:

init { 
    variableA : float 
    variableB : float
    varIf : float
    valorIf : float
    varFin : float
    valorFin : float
} 

if (! variableA == variableB) {
    varIf := valorIf
}

if (! variableA != variableB) {
    varIf := valorIf
}

varFin := valorFin
*/