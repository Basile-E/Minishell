#include <stdio.h>

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

typedef struct s_cmd
{
	char **cmd;//  cmd[0] = echo ; cmd[1] = "bonjour" 
	struct s_cmd *next;
}				t_cmd;


typedef enum e_quote
{
	NONE,
	SINGLE,
	DOUBLE
}			t_quote;

void set_int(int *a, int *b, int *c, int *d)
{
	if (a != NULL)
		*a = 0;
	if (b != NULL)
		*b = 0;
	if (c != NULL)
		*c = 0;
	if (d != NULL)
		*d = 0;
}



int check_unclosed(char *str)
{
	int i;
	t_quote	status;

	i = 0;
	status = NONE;
	while(str[i])
	{
		if (status == NONE && str[i] == '\'')
			status = SINGLE;
		else if (status == NONE && str[i] == '"')
			status = DOUBLE;
		else if (status == SINGLE && str[i] == '\''
			|| status == DOUBLE && str[i] == '"')
			status = NONE;
		i++;
	}
	return((int)status);
}




int find_quote(char *str)
{
	int i;
	int g;
	set_int(&i, &g, NULL, NULL);
	printf("%i\n", i);
	printf("%i\n", g);
	return (i);
}


{
	int i = 0;
	

}

int main()
{
	char *str = "bonjour les loulou";
	if (!check_unclosed(str))
		return(printf("Quote closed\n"), 0);
	else
		return(printf("Quote unclosed\n"), 0);
	find_quote(str);
}