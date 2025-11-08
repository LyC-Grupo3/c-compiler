#include "assembler.h"
#include "utils/polaca.h"

// Contador para etiquetas únicas
static int label_count = 0;

void generar_asm(const char* output_file) {
    FILE* fp = fopen(output_file, "w");
    if (!fp) {
        printf("Error al crear el archivo ASM\n");
        return;
    }

    write_header(fp);
    write_data_section(fp);
    write_code_section(fp);
    write_footer(fp);

    fclose(fp);
}

void write_header(FILE* fp) {
    fprintf(fp, ".8086\n");
    fprintf(fp, ".model small\n");
    fprintf(fp, ".stack 100h\n\n");
}

void write_data_section(FILE* fp) {
    fprintf(fp, ".data\n");
    fprintf(fp, "msgerror db \"Error de ejecucion\", 13, 10, \"$\"\n");
    fprintf(fp, "newline db 13, 10, \"$\"\n");
    fprintf(fp, "aux dw ?\n");           // Variable auxiliar para operaciones
    fprintf(fp, "aux2 dw ?\n");          // Segunda variable auxiliar
    
    // Buffer para strings temporales
    fprintf(fp, "stringbuf db 256 dup(?), \"$\"\n");
    
    // Constantes string del código
    FILE* intermediate = fopen("intermediate-code.txt", "r");
    if (intermediate) {
        char line[256];
        int str_count = 0;
        
        while (fgets(line, sizeof(line), intermediate)) {
            char* content = strchr(line, ']');
            if (content) content += 2;
            else content = line;
            
            content[strcspn(content, "\n")] = 0;
            
            if (content[0] == '\"') {
                fprintf(fp, "str_%d db %s, \"$\"\n", str_count++, content);
            }
        }
        fclose(intermediate);
    }

    // Recorrer la tabla de símbolos y escribir las variables
    TablaSimbolos* tabla = obtener_tabla_simbolos();
    Simbolo* actual = tabla->primero;
    
    while (actual != NULL) {
        if (actual->tipo == VARIABLE) {
            // Variables siempre como word (16 bits)
            fprintf(fp, "%s dw ?\n", actual->nombre);
        } else if (actual->tipo == CONSTANTE) {
            if (strchr(actual->valor, '.') != NULL) {
                // Si es flotante, lo convertimos a entero multiplicando por 100
                double valor = atof(actual->valor);
                int valor_entero = (int)(valor * 100);
                fprintf(fp, "%s dw %d  ; %.2f * 100\n", actual->nombre, valor_entero, valor);
            } else if (actual->valor[0] == '\"') {
                fprintf(fp, "%s db %s, \"$\"\n", actual->nombre, actual->valor);
            } else {
                fprintf(fp, "%s dw %s\n", actual->nombre, actual->valor);
            }
        }
        actual = actual->siguiente;
    }
    fprintf(fp, "\n");
}

void write_code_section(FILE* fp) {
    fprintf(fp, ".code\n\n");
    
    // Procedimientos auxiliares para entrada/salida
    fprintf(fp, "; Procedimiento para imprimir un string terminado en $\n");
    fprintf(fp, "imprimir_string PROC\n");
    fprintf(fp, "    mov ah, 09h\n");
    fprintf(fp, "    int 21h\n");
    fprintf(fp, "    ret\n");
    fprintf(fp, "imprimir_string ENDP\n\n");

    fprintf(fp, "; Procedimiento para imprimir un entero\n");
    fprintf(fp, "imprimir_entero PROC\n");
    fprintf(fp, "    push ax\n");
    fprintf(fp, "    push bx\n");
    fprintf(fp, "    push cx\n");
    fprintf(fp, "    push dx\n");

    fprintf(fp, "    mov bx, 10      ; Base decimal\n");
    fprintf(fp, "    xor cx, cx      ; Contador de dígitos\n");

    fprintf(fp, "    ; Si es negativo, imprimir el signo\n");
    fprintf(fp, "    test ax, ax\n");
    fprintf(fp, "    jns %%positive\n");
    fprintf(fp, "    push ax\n");
    fprintf(fp, "    mov dl, '-'\n");
    fprintf(fp, "    mov ah, 02h\n");
    fprintf(fp, "    int 21h\n");
    fprintf(fp, "    pop ax\n");
    fprintf(fp, "    neg ax\n");

    fprintf(fp, "%%positive:\n");
    fprintf(fp, "    ; Convertir a string\n");
    fprintf(fp, "%%divide_loop:\n");
    fprintf(fp, "    xor dx, dx\n");
    fprintf(fp, "    div bx\n");
    fprintf(fp, "    push dx         ; Guardar el dígito\n");
    fprintf(fp, "    inc cx\n");
    fprintf(fp, "    test ax, ax\n");
    fprintf(fp, "    jnz %%divide_loop\n");

    fprintf(fp, "    ; Imprimir dígitos\n");
    fprintf(fp, "%%print_loop:\n");
    fprintf(fp, "    pop dx\n");
    fprintf(fp, "    add dl, '0'\n");
    fprintf(fp, "    mov ah, 02h\n");
    fprintf(fp, "    int 21h\n");
    fprintf(fp, "    loop %%print_loop\n");

    fprintf(fp, "    pop dx\n");
    fprintf(fp, "    pop cx\n");
    fprintf(fp, "    pop bx\n");
    fprintf(fp, "    pop ax\n");
    fprintf(fp, "    ret\n");
    fprintf(fp, "imprimir_entero ENDP\n\n");

    fprintf(fp, "; Procedimiento para leer un entero\n");
    fprintf(fp, "leer_entero PROC\n");
    fprintf(fp, "    push bx\n");
    fprintf(fp, "    push cx\n");
    fprintf(fp, "    push dx\n");

    fprintf(fp, "    mov bx, 0       ; Valor acumulado\n");
    fprintf(fp, "    mov cx, 0       ; Signo (0 positivo, 1 negativo)\n");

    fprintf(fp, "    ; Leer primer carácter\n");
    fprintf(fp, "%%read_first:\n");
    fprintf(fp, "    mov ah, 01h\n");
    fprintf(fp, "    int 21h\n");
    
    fprintf(fp, "    ; Verificar signo\n");
    fprintf(fp, "    cmp al, '-'\n");
    fprintf(fp, "    jne %%check_digit\n");
    fprintf(fp, "    mov cx, 1\n");
    fprintf(fp, "    jmp %%read_first\n");

    fprintf(fp, "%%read_loop:\n");
    fprintf(fp, "    mov ah, 01h\n");
    fprintf(fp, "    int 21h\n");

    fprintf(fp, "%%check_digit:\n");
    fprintf(fp, "    cmp al, 13      ; Enter?\n");
    fprintf(fp, "    je %%end_read\n");
    fprintf(fp, "    cmp al, '0'\n");
    fprintf(fp, "    jb %%read_loop\n");
    fprintf(fp, "    cmp al, '9'\n");
    fprintf(fp, "    ja %%read_loop\n");

    fprintf(fp, "    ; Convertir y acumular dígito\n");
    fprintf(fp, "    sub al, '0'\n");
    fprintf(fp, "    xor ah, ah\n");
    fprintf(fp, "    mov dx, ax      ; Guardar dígito\n");
    fprintf(fp, "    mov ax, bx      ; Recuperar valor acumulado\n");
    fprintf(fp, "    mov bx, 10\n");
    fprintf(fp, "    mul bx          ; AX = AX * 10\n");
    fprintf(fp, "    add ax, dx      ; Sumar nuevo dígito\n");
    fprintf(fp, "    mov bx, ax      ; Guardar nuevo valor\n");
    fprintf(fp, "    jmp %%read_loop\n");

    fprintf(fp, "%%end_read:\n");
    fprintf(fp, "    mov ax, bx      ; Poner resultado en AX\n");
    fprintf(fp, "    test cx, cx     ; Verificar signo\n");
    fprintf(fp, "    jz %%positive_end\n");
    fprintf(fp, "    neg ax          ; Negar si es negativo\n");
    
    fprintf(fp, "%%positive_end:\n");
    fprintf(fp, "    pop dx\n");
    fprintf(fp, "    pop cx\n");
    fprintf(fp, "    pop bx\n");
    fprintf(fp, "    ret\n");
    fprintf(fp, "leer_entero ENDP\n\n");

    fprintf(fp, "; Procedimiento para nueva línea\n");
    fprintf(fp, "nueva_linea PROC\n");
    fprintf(fp, "    push dx\n");
    fprintf(fp, "    push ax\n");
    fprintf(fp, "    mov dl, 13\n");
    fprintf(fp, "    mov ah, 02h\n");
    fprintf(fp, "    int 21h\n");
    fprintf(fp, "    mov dl, 10\n");
    fprintf(fp, "    mov ah, 02h\n");
    fprintf(fp, "    int 21h\n");
    fprintf(fp, "    pop ax\n");
    fprintf(fp, "    pop dx\n");
    fprintf(fp, "    ret\n");
    fprintf(fp, "nueva_linea ENDP\n\n");
    
    // Inicio del programa principal
    fprintf(fp, "START:\n");
    fprintf(fp, "    mov AX, @data\n");
    fprintf(fp, "    mov DS, AX\n\n");
    
    process_intermediate_code(fp);
    
    // Fin del programa
    fprintf(fp, "    mov AX, 4C00h\n");
    fprintf(fp, "    int 21h\n");
}

void write_footer(FILE* fp) {
    fprintf(fp, "\nEND START\n");
}

void process_intermediate_code(FILE* fp) {
    FILE* intermediate = fopen("intermediate-code.txt", "r");
    if (!intermediate) {
        printf("Error al abrir el código intermedio\n");
        return;
    }

    char line[256];
    int current_line = 0;
    char buffer[256];
    int str_label = 0;
    
    // Variables para control de etiquetas
    int label_stack[100];
    int label_top = 0;
    
    // Función ABS implementada como macro
    fprintf(fp, "; Función valor absoluto\n");
    fprintf(fp, "abs_valor PROC\n");
    fprintf(fp, "    test ax, ax\n");
    fprintf(fp, "    jns %%no_negar\n");
    fprintf(fp, "    neg ax\n");
    fprintf(fp, "%%no_negar:\n");
    fprintf(fp, "    ret\n");
    fprintf(fp, "abs_valor ENDP\n\n");

    while (fgets(line, sizeof(line), intermediate)) {
        // Eliminar los corchetes y el número de línea
        char* content = strchr(line, ']');
        if (content) content += 2;
        else content = line;
        
        // Eliminar el salto de línea
        content[strcspn(content, "\n")] = 0;
        
        // Procesar cada instrucción
        if (strstr(content, ":=") != NULL) {
            // Asignación
            char var[50], value[50];
            sscanf(content, "%s := %s", var, value);
            fprintf(fp, "    ; Asignación a %s\n", var);
            fprintf(fp, "    pop ax\n");
            fprintf(fp, "    mov %s, ax\n", var);
        }
        else if (strcmp(content, "READ") == 0) {
            fprintf(fp, "    ; Leer valor\n");
            fprintf(fp, "    call leer_entero\n");
            fprintf(fp, "    push ax\n");  // Guardar el valor leído en la pila
        }
        else if (strcmp(content, "WRITE") == 0) {
            fprintf(fp, "    ; Escribir valor\n");
            fprintf(fp, "    pop ax\n");
            fprintf(fp, "    call imprimir_entero\n");
            fprintf(fp, "    call nueva_linea\n");
        }
        else if (strcmp(content, "+") == 0) {
            fprintf(fp, "    ; Suma\n");
            fprintf(fp, "    pop bx\n");
            fprintf(fp, "    pop ax\n");
            fprintf(fp, "    add ax, bx\n");
            fprintf(fp, "    push ax\n");
        }
        else if (strcmp(content, "-") == 0) {
            fprintf(fp, "    ; Resta\n");
            fprintf(fp, "    pop bx\n");
            fprintf(fp, "    pop ax\n");
            fprintf(fp, "    sub ax, bx\n");
            fprintf(fp, "    push ax\n");
        }
        else if (strcmp(content, "*") == 0) {
            fprintf(fp, "    ; Multiplicación\n");
            fprintf(fp, "    pop bx\n");
            fprintf(fp, "    pop ax\n");
            fprintf(fp, "    imul bx\n");
            fprintf(fp, "    push ax\n");
        }
        else if (strcmp(content, "/") == 0) {
            fprintf(fp, "    ; División\n");
            fprintf(fp, "    pop bx\n");
            fprintf(fp, "    pop ax\n");
            fprintf(fp, "    xor dx, dx\n");
            fprintf(fp, "    idiv bx\n");
            fprintf(fp, "    push ax\n");
        }
        else if (strcmp(content, "CMP") == 0) {
            fprintf(fp, "    ; Comparación\n");
            fprintf(fp, "    pop bx\n");
            fprintf(fp, "    pop ax\n");
            fprintf(fp, "    cmp ax, bx\n");
        }
        else if (strcmp(content, "BLE") == 0) {
            // En la siguiente línea está el número de la etiqueta
            fgets(line, sizeof(line), intermediate);
            char* label = strchr(line, ']');
            if (label) label += 2;
            else label = line;
            label[strcspn(label, "\n")] = 0;
            fprintf(fp, "    jg label_%s\n", label);
            current_line++;
        }
        else if (strcmp(content, "BGE") == 0) {
            fgets(line, sizeof(line), intermediate);
            char* label = strchr(line, ']');
            if (label) label += 2;
            else label = line;
            label[strcspn(label, "\n")] = 0;
            fprintf(fp, "    jl label_%s\n", label);
            current_line++;
        }
        else if (strcmp(content, "BEQ") == 0) {
            fgets(line, sizeof(line), intermediate);
            char* label = strchr(line, ']');
            if (label) label += 2;
            else label = line;
            label[strcspn(label, "\n")] = 0;
            fprintf(fp, "    jne label_%s\n", label);
            current_line++;
        }
        else if (strcmp(content, "BGT") == 0) {
            fgets(line, sizeof(line), intermediate);
            char* label = strchr(line, ']');
            if (label) label += 2;
            else label = line;
            label[strcspn(label, "\n")] = 0;
            fprintf(fp, "    jle label_%s\n", label);
            current_line++;
        }
        else if (strcmp(content, "BLT") == 0) {
            fgets(line, sizeof(line), intermediate);
            char* label = strchr(line, ']');
            if (label) label += 2;
            else label = line;
            label[strcspn(label, "\n")] = 0;
            fprintf(fp, "    jge label_%s\n", label);
            current_line++;
        }
        else if (strcmp(content, "BI") == 0) {
            fgets(line, sizeof(line), intermediate);
            char* label = strchr(line, ']');
            if (label) label += 2;
            else label = line;
            label[strcspn(label, "\n")] = 0;
            fprintf(fp, "    jmp label_%s\n", label);
            current_line++;
        }
        else if (strcmp(content, "ET") == 0) {
            fgets(line, sizeof(line), intermediate);
            char* label = strchr(line, ']');
            if (label) label += 2;
            else label = line;
            label[strcspn(label, "\n")] = 0;
            fprintf(fp, "label_%s:\n", label);
            current_line++;
        }
        else if (strncmp(content, "label_", 6) == 0) {
            fprintf(fp, "%s:\n", content);
        }
        else if (content[0] == '\"') {
            // Es una cadena literal
            fprintf(fp, "    ; Imprimir string\n");
            fprintf(fp, "    lea dx, str_%d\n", str_label++);
            fprintf(fp, "    call imprimir_string\n");
            fprintf(fp, "    lea dx, newline\n");
            fprintf(fp, "    call imprimir_string\n");
        }
        else if (isdigit(content[0]) || content[0] == '-' || content[0] == '.') {
            // Es un número
            fprintf(fp, "    ; Cargar número\n");
            if (strchr(content, '.') != NULL) {
                // Convertir flotante a entero (*100)
                double valor = atof(content);
                int valor_entero = (int)(valor * 100);
                fprintf(fp, "    mov ax, %d  ; %.2f * 100\n", valor_entero, valor);
                fprintf(fp, "    push ax\n");
            } else {
                // Es un número entero
                fprintf(fp, "    mov ax, %s\n", content);
                fprintf(fp, "    push ax\n");
            }
        }
        else if (strlen(content) > 0) {
            // Es una variable o identificador
            fprintf(fp, "    ; Cargar variable %s\n", content);
            fprintf(fp, "    mov ax, %s\n", content);
            fprintf(fp, "    push ax\n");
        }
        
        current_line++;
    }
    
    fclose(intermediate);
}
