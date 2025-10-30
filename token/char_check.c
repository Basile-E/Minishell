#include "../includes/minishell.h"

int is_operator(char c)
{
    if (c == '|' || c == '>'  || c == '<')
        return (1); 
    return (0);
}

int is_sep(char c)
{
    if (c == ' ' || c == '\n' || c == '\t' ||
		c == '|' || c == '>'  || c == '<' || c == '\0') //pas sur que \0 soit un sep
        return (1);
    return (0);
}

int is_space(char c)
{
    if (c == ' ' || (c >= '\t' && c <= '\v'))
        return(1);
    return(0);
}

int is_lowercase(char suspect)
{
	if (suspect >= 'a' && suspect <= 'z')
		return(1);
	return(0);
}

int is_uppercase(char suspect)
{
	if (suspect >= 'A' && suspect <= 'Z')
		return(1);
	return(0);
}

int is_number(char mightbeanint)
{
	if (mightbeanint >= '0' && mightbeanint <= '9')
		return(1);
	return(0);
}
// j'avais pas de bon nom, mais elle check si c'est dif de abc... ou 123... 
int	is_char(char suspect)
{
	if (!is_lowercase(suspect) || !is_uppercase(suspect) ||
		!is_number(suspect))
		return(1);
	return(0);
}