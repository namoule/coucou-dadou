/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealefev <jealefev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 16:27:01 by jealefev          #+#    #+#             */
/*   Updated: 2024/11/08 17:49:34 by jealefev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	freetab(char **tab)
{
	int	i;

	if (tab)
	{
		i = 0;
		while (tab[i])
		{
			free(tab[i]);
			i++;
		}
		free(tab);
	}
}

void	free_cmd(t_command *cmd)
{
	int	i;

	if (cmd->fd_in >= 0)
		close(cmd->fd_in);
	if (cmd->fd_out >= 0 && cmd->next == NULL)
		close(cmd->fd_out);
	if (cmd->pprev >= 0)
		close(cmd->pprev);
	i = 0;
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	free(cmd);
}
