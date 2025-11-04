#ifndef MINISHELL_H
 #define MINISHELL_H

///////////////////
/*  Includes	*/
/////////////////

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "../libft/libft.h"

///////////////////
/*  Define	  */
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
#   define RED	  "\033[31m"
#   define GREEN	"\033[32m"
#   define YELLOW   "\033[33m"
#   define BLUE	 "\033[34m"
#   define MAGENTA  "\033[35m"
#   define CYAN	 "\033[36m"
#   define RESET	"\033[0m"
#   define BOLD	 "\033[1m"
#   define PINK	 "\033[38;5;218m"

///////////////////
/*  Structs	 */
/////////////////

// simulation struct
typedef struct  s_minishell
{
	char	*input;
	int	 status;
	char	**env;
}			   t_minishell;

// la linked list des token (args -> token)
typedef struct  s_args
{
	char	*arg;
	int	 clean;
	int	 type;
	struct s_args  *next;
}			   t_args;



typedef enum e_quote
{
	NONE,
	SINGLE,
	DOUBLE
}			t_quote;

typedef enum e_token_type 
{
	WORD,
	PIPE,		   // |
	REDIRECT_IN,	// <
	REDIRECT_OUT,   // >
	REDIRECT_APPEND,// >>
	REDIRECT_HEREDOC,// <<
	EOF_TOKEN
} 			t_token_type;

typedef struct s_token 
{
	t_token_type type;
	char *value;
	struct s_token *next;
} 				t_token;


typedef struct 	s_cmd
{
	char **args;		// ["echo", "hello", NULL] 
	int fd_in;   // "input.txt" ou NULL
	int fd_out;  // "output.txt" ou NULL
	int append_mode;	// 1 si >>, 0 si >
	char *heredoc;
	struct s_cmd *next; // Commande après |
}				t_cmd;

///////////////////
/*  Fonctions   */
/////////////////

// Expand
char *do_expand(t_minishell *minishell, char *str);
int check_for_expand(char *str);
char	*find_path(char *cmd, char **envp);
char *do_expandV2(t_minishell *minishell, char *str);

// Parsing
int parsinette(t_minishell *minishell);

// SRC


// Token
int is_space(char c);
int is_sep(char c);
int is_operator(char c);
t_token *create_and_add_token(t_token **head, t_token **current, char *content);
void clear_buff(char *buffer);
int tokenizer(t_minishell *minishell, t_args **args);

// Utils
int tab_len(char **tab);
int	print_token(t_token *head);
void print_env(char **env);
int set_struct_minishell(t_minishell *minishell, char **env);
int	is_char(char suspect);
int is_number(char mightbeanint);
int is_uppercase(char suspect);
int is_lowercase(char suspect);

// Prototypes
t_token	*tokenize(char *input);
void	free_tokens(t_token *tokens);

// New Expand
char *do_expand_simple(t_minishell *minishell, char *str);
char *extract_var_name(char *str, int start, int *end);
char *get_env_value(t_minishell *minishell, char *var_name);
char *remove_quotes(char *str);
void	set_quote_status(char c, t_quote *status);
int check_syntax_errors(t_token *head);
int check_parentheses_syntax(char *input);
int check_unclosed_quotes(char *str);
char	**split_field(char const *s, char c);
int lexer(t_token *token, t_cmd *cmd);

// syntaxe 
int check_syntax_errors(t_token *head);
void	print_lexer(t_cmd *cmd);
#endif