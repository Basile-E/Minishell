#include "../includes/minishell.h"

int is_operator(char c)
{
    if (c == '|' || c == '>'  || c == '<')
        return (1); 
    return (0);
}

int is_sep(char c)
{
    if (c == ' ' || c == '\n' || c == '\t' || c == '|'  || c == '>'  || c == '<' || c == '\0') //pas sur que \0 soit un sep
        return (1);
    return (0);
}

int is_space(char c)
{
    if (c == ' ' || (c >= '\t' && c <= '\v'))
        return(1);
    return(0);
}