#ifndef MINISHELL_H
 #define MINISHELL_H

///////////////////
/*  Includes    */
/////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include "Libft/libft.h"

///////////////////
/*  Define      */
/////////////////

// buffer de 300 pour le tokenizer
#   define  BUFF_SIZE 300

// pour la struct des arg, permet de savoir si on doit clean le token
#   define  TO_CLEAN 1
#   define  NO_CLEAN 0

// pour la struct minishell, permet de savoir si on stop la main loop
#   define KILL_SIM 1
#   define DONT_KILL 0

// because it's nicer when it's shiny
#   define RED      "\033[31m"
#   define GREEN    "\033[32m"
#   define YELLOW   "\033[33m"
#   define BLUE     "\033[34m"
#   define MAGENTA  "\033[35m"
#   define CYAN     "\033[36m"
#   define RESET    "\033[0m"
#   define BOLD     "\033[1m"
#   define PINK     "\033[38;5;218m"

///////////////////
/*  Structs     */
/////////////////

// simulation struct
typedef struct  s_minishell
{
    char    *input;
    int     status;
}               t_minishell;

// la linked list des token (args -> token)
typedef struct  s_args
{
    char    *arg;
    int     clean;
    struct s_args  *next;
}               t_args;



///////////////////
/*  Fonctions   */
/////////////////





#endif