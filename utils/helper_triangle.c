#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./helper_triangle.h"
#include "./polaca.h"

/* -------------------------------------------------------------------------- */
/*                              VARIABLES GLOBALES                            */
/* -------------------------------------------------------------------------- */

static t_pila *pilaTriangulo1;
static t_pila *pilaTriangulo2;
static int contadorTriangulos = 0; // 0 = ninguno, 1 = primer triangulo, 2 = segundo triangulo

// Declaraciones externas necesarias para backpatching
extern t_pila *pilaBase;
extern void apilarNroCeldaActualYAvanzarPolaca(t_pila *pila);
extern int desapilarNroCeldaYEscribirEnEllaNroCeldaActual(t_pila *pila);
extern int desapilarNroCeldaYEscribirEnEllaNroCeldaActualMasUno(t_pila *pila);

/* -------------------------------------------------------------------------- */
/*                          FUNCIONES DE INICIALIZACIÓN                       */
/* -------------------------------------------------------------------------- */

void inicializarTriangleHelper()
{
    pilaTriangulo1 = crearPila();
    pilaTriangulo2 = crearPila();
    contadorTriangulos = 0;
}

void limpiarTriangleHelper()
{
    eliminarPila(pilaTriangulo1);
    eliminarPila(pilaTriangulo2);
}

/* -------------------------------------------------------------------------- */
/*                   FUNCIONES PARA TRIANGLE AREA MAXIMUM                     */
/* -------------------------------------------------------------------------- */

void iniciarTriangulo()
{
    contadorTriangulos++;
}

void apilarCoordenadaTriangulo(const char *valor)
{
    t_pila *pilaTrianguloActual = (contadorTriangulos == 1) ? pilaTriangulo1 : pilaTriangulo2;
    apilar(pilaTrianguloActual, valor);
}

void generarPolacaCalculoAreaTriangulo()
{
    // genera el código polaca para calcular el área de un triángulo
    // fórmula: Área = |x1(y2-y3) + x2(y3-y1) + x3(y1-y2)| / 2

    t_pila *pilaTrianguloActual = (contadorTriangulos == 1) ? pilaTriangulo1 : pilaTriangulo2;

    // Las coordenadas están en la pila en orden: x1, y1, x2, y2, x3, y3 (tope)
    // Necesitamos desapilarlas en orden inverso y hacer copias porque desapilar() usa buffer estático
    char y3[TAM_CONTENIDO_PILA], x3[TAM_CONTENIDO_PILA];
    char y2[TAM_CONTENIDO_PILA], x2[TAM_CONTENIDO_PILA];
    char y1[TAM_CONTENIDO_PILA], x1[TAM_CONTENIDO_PILA];

    strcpy(y3, desapilar(pilaTrianguloActual));
    strcpy(x3, desapilar(pilaTrianguloActual));
    strcpy(y2, desapilar(pilaTrianguloActual));
    strcpy(x2, desapilar(pilaTrianguloActual));
    strcpy(y1, desapilar(pilaTrianguloActual));
    strcpy(x1, desapilar(pilaTrianguloActual));

    // Generar código para: x1 * (y2 - y3)
    insertarEnPolaca(y2);
    insertarEnPolaca(y3);
    insertarEnPolaca("-");
    insertarEnPolaca(x1);
    insertarEnPolaca("*");

    // Generar código para: x2 * (y3 - y1)
    insertarEnPolaca(y3);
    insertarEnPolaca(y1);
    insertarEnPolaca("-");
    insertarEnPolaca(x2);
    insertarEnPolaca("*");

    // Sumar los dos primeros términos
    insertarEnPolaca("+");

    // Generar código para: x3 * (y1 - y2)
    insertarEnPolaca(y1);
    insertarEnPolaca(y2);
    insertarEnPolaca("-");
    insertarEnPolaca(x3);
    insertarEnPolaca("*");

    // Sumar el tercer término
    insertarEnPolaca("+");

    // Valor absoluto y división por 2
    insertarEnPolaca("ABS");
    insertarEnPolaca("2");
    insertarEnPolaca("/");

    // Guardar el resultado en una variable temporal
    char tempVar[20];
    snprintf(tempVar, 20, "@area_t%d", contadorTriangulos);
    insertarEnPolaca(tempVar);
    insertarEnPolaca(":=");
}

void generarPolacaTriangleAreaMaximun()
{
    // comparar las dos áreas y retornar el máximo
    // Piensenlo como
    // if (area1 > area2)
    // then max = area1
    // else  max = area2
    // end if
    // return max

    insertarEnPolaca("@area_t1");
    insertarEnPolaca("@area_t2");
    insertarEnPolaca(VALOR_POLACA_COMPARADOR);
    insertarEnPolaca(VALOR_POLACA_OP_MAYOR); // BLE - salta si area1 <= area2. Osea saltaria al else donde area2 es maximo

    // Apilar posición para backpatching (si es falso, salta al else)
    apilarNroCeldaActualYAvanzarPolaca(pilaBase);

    // Caso verdadero: setear area1 como maximo
    insertarEnPolaca("@area_t1");
    insertarEnPolaca("@result_max");
    insertarEnPolaca(":=");

    // Saltar al retorno, todavia aca nose donde estaria ese final
    insertarEnPolaca("BI");
    desapilarNroCeldaYEscribirEnEllaNroCeldaActualMasUno(pilaBase);
    apilarNroCeldaActualYAvanzarPolaca(pilaBase);

    // Completar el salto del if
    // desapilarNroCeldaYEscribirEnEllaNroCeldaActual(pilaBase);

    // Caso falso: setear area2 como maximo
    insertarEnPolaca("@area_t2");
    insertarEnPolaca("@result_max");
    insertarEnPolaca(":=");

    // Para saltarme el caso falso donde area2 es el maximo
    desapilarNroCeldaYEscribirEnEllaNroCeldaActual(pilaBase);

    // El resultado final está en @result_max, insertarlo en la polaca
    insertarEnPolaca("@result_max");

    // Resetear contador para próximas llamadas
    contadorTriangulos = 0;

    // Agregar las variables temporales a la tabla de símbolos
    insertarVariableASM("@area_t1", TIPO_TOKEN_CONST_FLOAT);
    insertarVariableASM("@area_t2", TIPO_TOKEN_CONST_FLOAT);
    insertarVariableASM("@result_max", TIPO_TOKEN_CONST_FLOAT);

    // Las constantes usadas como retorno las tengo que agregar a la tabla de simbolos porque sino luego no las encuentro para el ASM
    procesarSimbolo("2", TIPO_TOKEN_CONST_INT);
}
