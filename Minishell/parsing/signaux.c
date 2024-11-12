/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealefev <jealefev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 01:25:41 by jealefev          #+#    #+#             */
/*   Updated: 2024/11/08 17:50:44 by jealefev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		exit_sig = 0;

// void	gestion_signaux;

char	*get_exit_code(int len, t_state *state)
{
	char	*env;

	env = ft_itoa(*(state->cmd->table->exit_code));
	if (env)
	{
		state->n[0] += len;
		state->i += len;
	}
	return (env);
}

int	check_pair_quotes(t_command *cmd)
{
	int	i;
	int	j;
	int	count_sq;
	int	count_dq;

	i = 0;
	j = 0;
	count_dq = 0;
	count_sq = 0;
	while (cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '\'') || ft_strchr(cmd->args[i], '\"'))
		{
			while (cmd->args[i][j])
			{
				if (cmd->args[i][j] == '\'')
					count_sq++;
				if (cmd->args[i][j] == '\"')
					count_dq++;
				j++;
			}
		}
		i++;
	}
	if ((count_sq % 2) || (count_dq % 2))
		return (0);
	return (1);
}
