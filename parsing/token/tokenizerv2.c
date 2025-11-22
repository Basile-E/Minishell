#include "../includes/minishell.h"

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

static void	crea_tok_pipe(int *i, t_token **tokens)
{
	t_token	*token;

	token = create_token(PIPE, "|");
	*i += 1;
	add_token(tokens, token);
}

static void	crea_tok_app(char *input, int *i, t_token **tokens)
{
	t_token	*token;

	if (input[*i + 1] && input[*i + 1] == '>')
	{
		token = create_token(REDIRECT_APPEND, ">>");
		*i += 2;
	}
	else
	{
		token = create_token(REDIRECT_OUT, ">");
		*i += 1;
	}
	add_token(tokens, token);
}

static void	crea_tok_her(char *input, int *i, t_token **tokens)
{
	t_token	*token;

	if (input[*i + 1] && input[*i + 1] == '<')
	{
		token = create_token(REDIRECT_HEREDOC, "<<");
		*i += 2;
	}
	else
	{
		token = create_token(REDIRECT_IN, "<");
		*i += 1;
	}
	add_token(tokens, token);
}

int	parse_redirect_operator(char *input, int *i, t_token **tokens)
{
	if (input[*i] == '<')
	{
		crea_tok_her(input, i, tokens);
	}
	else if (input[*i] == '>')
	{
		crea_tok_app(input, i, tokens);
	}
	else if (input[*i] == '|')
	{
		crea_tok_pipe(i, tokens);
	}
	else
		return (0);
	return (1);
}

int	parse_word(char *input, int *i, t_token **tokens, t_quote status)
{
	int		start;
	int		len;
	char	*word;
	t_token	*token;

	start = *i;
	while (input[*i] && ((!is_space(input[*i]) && !is_operator(input[*i]))
			|| status != NONE))
		(*i)++;
	len = *i - start;
	word = ft_substr(input, start, len);
	token = create_token(WORD, word);
	free(word);
	add_token(tokens, token);
	return (1);
}

static void	set_var(t_quote *status, t_token **tokens, int *i)
{
	*status = NONE;
	*tokens = NULL;
	*i = 0;
}

t_token	*tokenize(char *input)
{
	t_quote	status;
	t_token	*tokens;
	int		i;

	set_var(&status, &tokens, &i);
	while (input[i])
	{
		set_quote_status(input[i], &status);
		while (input[i] && is_space(input[i]) && status == NONE)
			i++;
		set_quote_status(input[i], &status);
		if (!input[i])
			break ;
		if (is_operator(input[i]) && status == NONE)
		{
			if (!parse_redirect_operator(input, &i, &tokens))
				return (NULL);
		}
		else
		{
			if (!parse_word(input, &i, &tokens, status))
				return (NULL);
		}
	}
	return (tokens);
}

void	free_tokens(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}
