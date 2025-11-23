#ifndef MINISHELL_H
# define MINISHELL_H

///////////////////
/*  Includes	*/
/*  Includes	*/
/////////////////

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h> // may be illegal, sert a strerror
# include <sys/stat.h>
# include <sys/wait.h> // same shit, sert aux pipes
# include <unistd.h>
#include <signal.h>

///////////////////
/*  Define		*/ //<- un peut cheum avec du recul
/*  Define		*/ //<- un peut cheum avec du recul
/////////////////

extern volatile sig_atomic_t	g_signal;

// Types of files, sert au tokenizer pour arg.type
# define FONCTION 1
# define OPERATOR 2
# define FILE 3
# define ERROR 0

// buffer de 300 pour le tokenizer
# define BUFF_SIZE 300

// pour la struct des arg, permet de savoir si on doit clean le token
# define TO_CLEAN 1
# define NO_CLEAN 0

// pour la struct minishell, permet de savoir si on stop la main loop
# define KILL_SIM 1
# define DONT_KILL 0

// because it's nicer when it's shiny
# define RED "\033[31m"
# define GREEN "\033[32m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define RESET "\033[0m"
# define BOLD "\033[1m"
# define PINK "\033[38;5;218m"
# define CYAN "\033[36m"
# define RESET "\033[0m"
# define BOLD "\033[1m"
# define PINK "\033[38;5;218m"

///////////////////
/*  Structs		*/
/*  Structs		*/
/////////////////

// simulation struct
typedef struct s_alloc
{
	void						**adr;
	struct s_alloc				*next;
}								t_alloc;

typedef struct s_minishell
{
	char						*input;
	int							status;
	char						**env;
	t_alloc						*alloc;
}								t_minishell;

// struct de l'execution
typedef struct s_exec
{
	int							fd[2];
	int							prev_fd;
	int							pid;
	char						*path;
}								t_exec;

// la linked list des token (args -> token)
typedef struct s_args
{
	char						*arg;
	int							clean;
	int							type;
	struct s_args				*next;
}								t_args;

typedef enum e_quote
{
	NONE,
	SINGLE,
	DOUBLE
}								t_quote;

//struct expand
typedef struct s_expandinette
{
	int		end;
	char	*var_name;
	char	*var_value;
	char	char_str[2];
	char	*result;
	char	*temp;
	int		i;
}			t_expandinette;

typedef struct s_r_quote
{
	char	*result;
	int		i;
	int		j;
	int		len;
	t_quote	status;
	char	quote_char;
}			t_r_quote;

typedef enum e_token_type
{
	WORD,
	PIPE,             // |
	REDIRECT_IN,      // <
	REDIRECT_OUT,     // >
	REDIRECT_APPEND,  // >>
	REDIRECT_HEREDOC, // <<
	EOF_TOKEN
}								t_token_type;

typedef struct s_token
{
	t_token_type				type;
	char						*value;
	struct s_token				*next;
}								t_token;

typedef struct s_cmd
{
	char **args;     // ["echo", "hello", NULL]
	int fd_in;       // "input.txt" ou NULL
	int fd_out;      // "output.txt" ou NULL
	int append_mode; // 1 si >>, 0 si >
	char						*heredoc;
	int							in_child;
	struct s_cmd *next; // Commande après |
}								t_cmd;

///////////////////
/*  Fonctions   */
/////////////////

// Expand
char							*do_expand(t_minishell *minishell, char *str);
int								check_for_expand(char *str);
char							*find_path(char *cmd, char **envp);
char							*do_expandV2(t_minishell *minishell, char *str);

// Parsing
int								parsinette(t_minishell *minishell);

// Token
int								is_space(char c);
int								is_sep(char c);
int								is_operator(char c);
t_token							*create_and_add_token(t_token **head,
									t_token **current, char *content);
void							clear_buff(char *buffer);
int								tokenizer(t_minishell *minishell,
									t_args **args);

// Utils
int								tab_len(char **tab);
int								print_token(t_token *head);
void							print_env(char **env);
int								set_struct_minishell(t_minishell *minishell,
									char **env);
int								is_char(char suspect);
int								is_number(char mightbeanint);
int								is_uppercase(char suspect);
int								is_lowercase(char suspect);

// Prototypes
t_token							*tokenize(char *input);
void							free_tokens(t_token *tokens);

// New Expand
char							*do_expand_simple(t_minishell *minishell,
									char *str);
char							*extract_var_name(char *str, int start,
									int *end);
char							*get_env_value(t_minishell *minishell,
									char *var_name);
char							*remove_quotes(char *str);
void							set_quote_status(char c, t_quote *status);
int								check_syntax_errors(t_token *head);
int								check_parentheses_syntax(char *input);
int								check_unclosed_quotes(char *str);
char							**split_field(char const *s, char c);
t_cmd							*lexer(t_token *token, t_cmd *cmd);

// syntaxe
int								check_syntax_errors(t_token *head);
void							print_lexer(t_cmd *cmd);

// env built-ins
void							ft_creat_env(t_minishell *mini, char **envp);
void							ft_add_env(t_minishell *mini, char *new);
void							ft_update_env(t_minishell *mini, char *new);
int								ft_env(t_minishell *mini);

// built-ins utils
void							ft_error(const char *str, const char *error);
int								ft_strcmp_equal(char *s1, char *s2);
int								does_it_exist(t_minishell *mini, char *src);

int								ft_echo(t_minishell mini, char **argv);

char							**ft_create_export(t_minishell *mini);
void							ft_sort_export(char **exp);
void							ft_export(t_minishell *mini, char **cmd);

int								execute(t_cmd *cmd, t_minishell *mini);
int								ft_strlen_y(char **tab);

int								ft_exit(char **argv, t_minishell *mini);
int								ft_unset(t_minishell *mini, char **args);
int								ft_pwd(void);

// cd built-ins
int								cd_error(char *msg);
void							perror_cd(char *path);
int								ft_cd(char **argv, t_minishell *mini);

// fT_malloc
void							*ft_malloc(int size, t_alloc **head);
void							free_alloc(t_alloc *head);

// signals
void							normal_signal(void);
void							in_exec_signal(void);
void							child_signal(void);
void							heredoc_signal(void);
void							handle_normal_sigint(int sig);
void							handle_exec_sigint(int sig);
void							handle_heredoc_sigint(int sig);
void							handle_sigquit(int sig);
void							handle_sigchld(int sig);

// hitman
int								check_for_builtin(char **cmd);
int								is_a_builtin(char **cmd, t_minishell *mini);
char							*find_path(char *cmd, char **envp);
void							error(void);
char							*get_current_path(void);
void							put_err_msg(char *str);

// execution
void							exec_mult(t_cmd *cmd, t_minishell *mini);
int								check_exec(t_cmd *cmd, t_minishell *mini,
									char **path);
void							do_pid_one(t_cmd *cmd, t_minishell *mini,
									char *path);
void							exec_single(t_cmd *cmd, t_minishell *mini);
int								execute(t_cmd *cmd, t_minishell *mini);

// hitman errors
void							error(void);
char							*get_current_path(void);
void							put_err_msg(char *str);

// handling
void							handle_heredoc(t_cmd *current);
void							dup_close(int fd_dup, int std, int fd_close);
void							handle_pid_one_bis(t_cmd *current,
									t_minishell *mini, char *path);
void							handle_pid_one(t_cmd *current, t_exec exec,
									t_minishell *mini, char *path);
void							handle_parent(t_cmd *current, t_exec *exec);

#endif