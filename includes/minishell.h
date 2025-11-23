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
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h> // may be illegal, sert a strerror
# include <sys/stat.h>
# include <sys/wait.h> // same shit, sert aux pipes
# include <unistd.h>

///////////////////
/*  Define		*/ //<- un peut cheum avec du recul
/*  Define		*/ //<- un peut cheum avec du recul
/////////////////

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
/////////////////

// simulation struct
typedef struct s_alloc
{
	void			**adr;
	struct s_alloc	*next;
}					t_alloc;

typedef struct s_minishell
{
	char			*input;
	int				status;
	char			**env;
	t_alloc			*alloc;
}					t_minishell;

// la linked list des token (args -> token)
typedef struct s_args
{
	char			*arg;
	int				clean;
	int				type;
	struct s_args	*next;
}					t_args;

typedef enum e_quote
{
	NONE,
	SINGLE,
	DOUBLE
}					t_quote;

typedef enum e_token_type
{
	WORD,
	PIPE,             // |
	REDIRECT_IN,      // <
	REDIRECT_OUT,     // >
	REDIRECT_APPEND,  // >>
	REDIRECT_HEREDOC, // <<
	EOF_TOKEN
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef struct s_cmd
{
	char **args;     // ["echo", "hello", NULL]
	int fd_in;       // "input.txt" ou NULL
	int fd_out;      // "output.txt" ou NULL
	int append_mode; // 1 si >>, 0 si >
	char			*heredoc;
	int				in_child;
	struct s_cmd *next; // Commande après |
}					t_cmd;

typedef struct s_lexer
{
	char			**cmd_tab;
	char			*heredoc;
	int				tab_idx;
	int				fd_in;
	int				fd_out;
	int				app_mode;
	int				words;
	int				ret_val;
}					t_lexer;

typedef struct s_heredoc
{
	char			*line;
	char			*ret_line;
	char			*line_nl;
	int				here_ret;
}					t_heredoc;

///////////////////
/*  Fonctions   */
/////////////////

// Expand
char				*do_expand(t_minishell *minishell, char *str);
int					check_for_expand(char *str);
char				*find_path(char *cmd, char **envp);
char				*do_expandV2(t_minishell *minishell, char *str);

// Parsing
int					parsinette(t_minishell *minishell);

// Token
int					is_space(char c);
int					is_sep(char c);
int					is_operator(char c);
t_token				*create_and_add_token(t_token **head, t_token **current,
						char *content);
void				clear_buff(char *buffer);
int					tokenizer(t_minishell *minishell, t_args **args);

// Utils
int					tab_len(char **tab);
int					print_token(t_token *head);
void				print_env(char **env);
int					set_struct_minishell(t_minishell *minishell, char **env);
int					is_char(char suspect);
int					is_number(char mightbeanint);
int					is_uppercase(char suspect);
int					is_lowercase(char suspect);

// Prototypes
t_token				*tokenize(char *input);
void				free_tokens(t_token *tokens);

// New Expand
char				*do_expand_simple(t_minishell *minishell, char *str);
char				*get_env_value(t_minishell *minishell, char *var_name);
char				*remove_quotes(char *str);
void				set_quote_status(char c, t_quote *status);
int					check_syntax_errors(t_token *head);
int					check_parentheses_syntax(char *input);
int					check_unclosed_quotes(char *str);
char				**split_field(char const *s, char c);
t_cmd				*lexer(t_token *token, t_cmd *cmd);

// syntaxe
int					check_syntax_errors(t_token *head);
void				print_lexer(t_cmd *cmd);

// exec built-ins
void				ft_creat_env(t_minishell *mini, char **envp);
void				ft_add_env(t_minishell *mini, char *new);
void				ft_update_env(t_minishell *mini, char *new);
int					ft_env(t_minishell *mini);
void				ft_error(const char *str, const char *error);
int					ft_strcmp_equal(char *s1, char *s2);
int					does_it_exist(t_minishell *mini, char *src);
int					ft_echo(t_minishell mini, char **argv);
char				**ft_create_export(t_minishell *mini);
void				ft_sort_export(char **exp);
void				ft_export(t_minishell *mini, char **cmd);
int					execute(t_cmd *cmd, t_minishell *mini);
int					ft_strlen_y(char **tab);
int					ft_exit(char **argv, t_minishell *mini);
int					ft_unset(char **argv, t_minishell *mini);
int					ft_pwd(void);

// cd built-ins
int					cd_error(char *msg);
void				perror_cd(char *path);
int					ft_cd(char **argv, t_minishell *mini);

void				*ft_malloc(int size, t_alloc **head);
void				free_alloc(t_alloc *head);
char				*extract_var_name(char *str, int start, int *end,
						t_minishell **mini);

// Lexer
char				*ft_strdup_gc(char *src, t_minishell **mini);
char				*ft_strjoin_gc(const char *s1, const char *s2,
						t_minishell *mini);
void				*ft_calloc_gc(size_t nbr_elements, size_t element_size,
						t_minishell *mini);

void				set_lexer(t_lexer **lexer);
int					do_lexing(t_lexer *lexer, t_cmd **new_node, t_cmd **cmd);
int					lexinette(t_lexer *lexer, t_token **current,
						t_cmd **new_node, t_cmd **cmd);
void				print_lexer(t_cmd *cmd);

int					be_a_heredoc(char **line, char **ret_line, char **limiter,
						char **line_nl);
int					do_heredoc(char *limiter, char **out);

void				cmd_set(t_cmd *new, int fd_in, int fd_out, int app_mode);
t_cmd				*cmd_create(char **cmds, int fd_in, int fd_out,
						int app_mode);
void				cmd_append(t_cmd **cmd, t_cmd *new_node);
int					count_words_until_pipe(t_token *start);

int					do_type_redirin(t_lexer *lexer, t_token **current);
int					do_type_redirher(t_lexer *lexer, t_token **current);
int					do_type_redirapp(t_lexer *lexer, t_token **current);
int					do_type_pipe(t_token **current, t_cmd **new_node,
						t_lexer *lexer, t_cmd **cmd);
int					do_type_word(t_lexer *lexer, t_token **current);
int 				do_type_redirout(t_lexer *lexer, t_token **current);


#endif