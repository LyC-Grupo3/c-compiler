#ifndef LISTA_EXPRESIONES_H
#define LISTA_EXPRESIONES_H

/* -------------------------------------------------------------------------- */
/*                                   DEFINES                                  */
/* -------------------------------------------------------------------------- */

#define TAM_CONTENIDO_PILA 50

/* -------------------------------------------------------------------------- */
/*                                 ESTRUCTURAS                                */
/* -------------------------------------------------------------------------- */

/**
 * @brief Nodo de la lista de expresiones
 * Cada nodo contiene una expresión como cadena de texto
 */
typedef struct t_nodo_expresion
{
    char expresion[TAM_CONTENIDO_PILA];
    struct t_nodo_expresion *siguiente;
} t_nodo_expresion;

/**
 * @brief Lista de expresiones
 * Cada elemento es una expresión representada como string
 * Ejemplo: "a+b+d", "x*y/z", "m-n"
 */
typedef struct t_lista_expresiones
{
    t_nodo_expresion *inicio;
    t_nodo_expresion *fin;
    int cantidad;
} t_lista_expresiones;

/* -------------------------------------------------------------------------- */
/*                                  FUNCIONES                                 */
/* -------------------------------------------------------------------------- */

/**
 * @brief Crea una nueva lista de expresiones vacía
 * @return Puntero a la lista creada o NULL si hay error
 */
t_lista_expresiones *crearListaExpresiones();

/**
 * @brief Agrega una expresión al final de la lista
 * @param lista Lista donde agregar la expresión
 * @param expresion Expresión (cadena) a agregar
 * @return 1 si se agregó correctamente, 0 si hay error
 */
int agregarExpresion(t_lista_expresiones *lista, const char *expresion);

/**
 * @brief Obtiene una expresión por su índice
 * @param lista Lista de expresiones
 * @param indice Índice de la expresión (empezando desde 0)
 * @return Puntero a la expresión (string) o NULL si el índice es inválido
 */
const char *obtenerExpresion(t_lista_expresiones *lista, int indice);

/**
 * @brief Verifica si la lista de expresiones está vacía
 * @param lista Lista a verificar
 * @return 1 si está vacía, 0 en caso contrario
 */
int listaExpresionesVacia(t_lista_expresiones *lista);

/**
 * @brief Elimina toda la lista de expresiones y libera memoria
 * @param lista Lista a eliminar
 */
void eliminarListaExpresiones(t_lista_expresiones *lista);

/**
 * @brief Imprime el contenido de la lista de expresiones
 * @param lista Lista a imprimir
 */
void imprimirListaExpresiones(t_lista_expresiones *lista);

/**
 * @brief Compara dos expresiones (strings)
 * @param exp1 Primera expresión
 * @param exp2 Segunda expresión
 * @return 1 si son iguales, 0 si son diferentes
 */
int compararExpresiones(const char *exp1, const char *exp2);

#endif // LISTA_EXPRESIONES_H
