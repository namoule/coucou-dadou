/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealefev <jealefev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 14:03:20 by jealefev          #+#    #+#             */
/*   Updated: 2024/11/12 12:37:36 by jealefev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

void	launch_exec(t_command *cmd, char **envp)
{
	execute_cmd(cmd, envp);
	if (cmd->pprev != -2)
		close(cmd->pprev);
	if (cmd->next == NULL && cmd->p[READ_END] >= 0)
		close(cmd->p[READ_END]);
	if (cmd->p[WRITE_END] >= 0)
		close(cmd->p[WRITE_END]);
}

int	flunch(t_command *cmd, char **envp)
{
	int			val;
	int			return_value;
	t_command	*quentin;

	quentin = cmd;
	while (cmd && cmd->args[0])
	{
		val = is_builtins(cmd);
		if (val == -1 && check_cmd(cmd, envp) == 0)
			launch_exec(cmd, envp);
		else
		{
			cmd->table->pids[cmd->table->ipids] = val;
			cmd->table->ipids++;
		}
		cmd = cmd->next;
	}
	return_value = wait_pids(quentin);
	return (return_value);
}

char	**exec_command(char *line, char **envp, int *return_value)
{
	char		**env;
	t_command	*cmd;

	if (check_line(line) == -1)
		return (envp);
	add_line_to_history(line);
	cmd = fill_t_command(line, envp, *return_value);
	*return_value = flunch(cmd, envp);
	env = cmd->table->envp;
	if (cmd)
	{
		if (cmd->table)
			free(cmd->table->pids);
		free_cmd(cmd);
	}
	return (env);
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	int		return_value;
	char	**env;

	return_value = 0;
	env = get_env(envp);
	(void)argc;
	(void)argv;
	while (1)
	{
		line = get_line(env);
		if (!line)
			break ;
		if (line[0] != '\0' && line[0] != '\n')
			env = exec_command(line, env, &return_value);
		free(line);
	}
	free(line);
	return (0);
}
