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

// pour la struct des arg, permet de savoir si on doit clean le token
#   define  TO_CLEAN 1
#   define  NO_CLEAN 0

// pour la struct minishell, permet de savoir si on stop la main loop
#   define KILL_SIM 1
#   define DONT_KILL 0


///////////////////
/*  Structs     */
/////////////////

// la linked list des token (args -> token)
typedef struct  s_minishell
{
    char    *input;
    int     status;
}               t_minishell;

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