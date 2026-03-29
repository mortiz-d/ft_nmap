# Libft - Implementación de funciones de la biblioteca estándar de C

Libft es una biblioteca que implementa un conjunto de funciones de la biblioteca estándar de C (libc) y algunas funciones adicionales que manipulan cadenas de caracteres y listas. Este proyecto tiene como objetivo reproducir las funciones más comunes de libc y crear nuevas funciones útiles que no están presentes en ella.

1. **Funciones de la biblioteca estándar de C (libc)**:
    - `ft_isalpha`: Comprueba si un carácter es alfabético.
    - `ft_isdigit`: Comprueba si un carácter es un dígito numérico.
    - `ft_isalnum`: Comprueba si un carácter es alfanumérico.
    - `ft_isascii`: Comprueba si un carácter es ASCII.
    - `ft_isprint`: Comprueba si un carácter es imprimible.
    - `ft_strlen`: Devuelve la longitud de una cadena de caracteres.
    - `ft_memset`: Llena un bloque de memoria con un valor específico.
    - `ft_bzero`: Establece un bloque de memoria a cero.
    - `ft_memcpy`: Copia un bloque de memoria a otro.
    - `ft_memmove`: Mueve un bloque de memoria a otro.
    - `ft_strlcpy`: Copia una cadena de caracteres con un tamaño limitado.
    - `ft_strlcat`: Concatena una cadena de caracteres con un tamaño limitado.
    - `ft_toupper`: Convierte un carácter a mayúscula.
    - `ft_tolower`: Convierte un carácter a minúscula.
    - `ft_strchr`: Busca la primera ocurrencia de un carácter en una cadena.
    - `ft_strrchr`: Busca la última ocurrencia de un carácter en una cadena.
    - `ft_strncmp`: Compara dos cadenas de caracteres hasta un número específico de caracteres.
    - `ft_memchr`: Busca un valor en un bloque de memoria.
    - `ft_memcmp`: Compara dos bloques de memoria.
    - `ft_strnstr`: Busca una subcadena en una cadena, limitando el número de caracteres.
    - `ft_atoi`: Convierte una cadena de caracteres a un número entero.


2. **Funciones de manipulación de cadenas**:
    - `ft_substr`: Crea una subcadena a partir de una cadena dada.
    - `ft_strjoin`: Concatena dos cadenas de caracteres.
    - `ft_strtrim`: Elimina los caracteres especificados desde el principio y el final de una cadena.
    - `ft_split`: Divide una cadena en un array de cadenas, usando un delimitador.
    - `ft_itoa`: Convierte un número entero a una cadena de caracteres.
    - `ft_strmapi`: Aplica una función a cada carácter de una cadena, devolviendo una nueva cadena con los resultados.
    - `ft_striteri`: Aplica una función a cada carácter de una cadena, pasando la posición de cada carácter.
    - `ft_putchar_fd`: Imprime un carácter en un descriptor de archivo.
    - `ft_putstr_fd`: Imprime una cadena de caracteres en un descriptor de archivo.
    - `ft_putendl_fd`: Imprime una cadena de caracteres seguida de una nueva línea en un descriptor de archivo.
    - `ft_putnbr_fd`: Imprime un número entero en un descriptor de archivo.

3. **Funciones de manipulación de listas** :
    - `ft_lstnew`: Crea un nuevo nodo para una lista.
    - `ft_lstadd_front`: Añade un nodo al principio de la lista.
    - `ft_lstsize`: Devuelve el tamaño de la lista.
    - `ft_lstlast`: Devuelve el último nodo de la lista.
    - `ft_lstadd_back`: Añade un nodo al final de la lista.
    - `ft_lstdelone`: Elimina un nodo de la lista.
    - `ft_lstclear`: Elimina todos los nodos de la lista.
    - `ft_lstiter`: Aplica una función a cada elemento de la lista.
    - `ft_lstmap`: Crea una nueva lista a partir de la aplicación de una función a cada elemento de la lista original.

## Estructura de la lista

La estructura de la lista es la siguiente:

```c
typedef struct s_list
{
    void *content;
    struct s_list *next;
} t_list;
```

## Compilacion
 - `make`        Para compilar la biblioteca libft.a
 - `make bonus`  Para compilar la biblioteca con las funciones adicionales(bonus)
 - `make clean`  Para limpiar los archivos objetos
 - `make fclean` Para limpiar los archivos objetos y la biblioteca
