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
#include "../libft/libft.h"

///////////////////
/*  Define      */
/////////////////


// Types of files, sert au tokenizer pour arg.type
#   define  FONCTION 1
#   define  OPERATOR 2
#   define  FILE 3
#   define  ERROR 0

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
    char    **env;
}               t_minishell;

// la linked list des token (args -> token)
typedef struct  s_args
{
    char    *arg;
    int     clean;
    int     type;
    struct s_args  *next;
}               t_args;



///////////////////
/*  Fonctions   */
/////////////////


// Expand
char *do_expand(t_minishell *minishell, char *str);
int check_for_expand(char *str);
char	*find_path(char *cmd, char **envp);


// Parsing
int parsinette(t_minishell *minishell, t_args **args);

// SRC


// Token
int is_space(char c);
int is_sep(char c);
int is_operator(char c);
t_args *create_and_add_token(t_args **head, t_args **current, char *content);
void clear_buff(char *buffer);
int tokenizer(t_minishell *minishell, t_args **args);

// Utils
int tab_len(char **tab);
int	print_token(t_args *head);
void print_env(char **env);
int set_struct_minishell(t_minishell *minishell, char **env);

#endif