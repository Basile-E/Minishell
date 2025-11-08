#include "minishell.h"

int is_a_builtin(char **cmd, t_minishell *mini)
{
	

	//if (ft_strncmp("cd", cmd, ft_strlen(cmd) + 1))

	if (ft_strncmp("echo", cmd[0], ft_strlen(cmd) + 1))
	{
		printf("Jason was here\n");
		ft_echo(*mini, cmd);
	}
	//if (ft_strncmp("exit", cmd, ft_strlen(cmd) + 1))

	//if (ft_strncmp("export", cmd, ft_strlen(cmd) + 1))

	//if (ft_strncmp("pwd", cmd, ft_strlen(cmd) + 1))

	//if (ft_strncmp("unset", cmd, ft_strlen(cmd) + 1))

}


int execute(t_cmd *cmd, t_minishell *mini)
{
	/* parcourir la liste des cmd
		check le cmd[0] dans is_a_builtin
		else
			il faut look dans .bin/

	*/	

	t_cmd *current;
	char *cmd_name;
	
	current = cmd;
	while(current)
	{
		if (is_a_builtin(current, mini))
			continue;
		else
		{
			continue;
		}
		current = current->next;
	}
}