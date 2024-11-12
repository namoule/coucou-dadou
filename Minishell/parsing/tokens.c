/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealefev <jealefev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 01:25:41 by jealefev          #+#    #+#             */
/*   Updated: 2024/11/12 12:42:41 by jealefev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*helper_quotes(char *arg, char c, t_state *state)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = state->n[0];
	if (arg[j] != '$')
		j++;
	str = malloc(1024);
	while (arg[j] != c && arg[j])
	{
		str[i] = arg[j];
		i++;
		j++;
	}
	str[i] = '\0';
	if (!str)
		return (NULL);
	if (!ft_strchr(str, '$') || state->sq_open || (ft_strchr(str, '$')
			&& state->dq_open))
		state->n[0] = j;
	return (str);
}

void	copy_quote_strings(t_command *cmd, char *quotes[4096], t_state *state)
{
	int	i;

	i = 0;
	while (quotes[i])
	{
		cmd->token_quotes[i] = malloc(1024);
		if (!cmd->token_quotes[i])
		{
			perror("Error allocating memory for token_quotes");
			exit(1);
		}
		cmd->token_quotes[i] = ft_strdup(quotes[i]);
		i++;
	}
	cmd->token_quotes[state->n[1]] = NULL;
}

char	*copy_quotes_to_arg(t_command *cmd, int *i)
{
	int	total_size;
	int	j;

	j = 0;
	total_size = 0;
	while (cmd->token_quotes[j])
		total_size += strlen(cmd->token_quotes[j++]);
	free(cmd->args[(*i)]);
	cmd->args[(*i)] = malloc(total_size + 1);
	if (!cmd->args[(*i)])
	{
		perror("Error allocating memory for concatenated args");
		exit(1);
	}
	cmd->args[(*i)][0] = '\0';
	j = 0;
	while (cmd->token_quotes[j])
	{
		ft_strlcat(cmd->args[(*i)], cmd->token_quotes[j++], total_size + 1);
	}
	cmd->args[++(*i)] = NULL;
	return (cmd->args[*i]);
}

void	process_quotes(t_command *cmd, t_state *state)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '\'') || ft_strchr(cmd->args[i], '\"')
			|| ft_strchr(cmd->args[i], '$' && (ft_strcmp(cmd->args[0],
						"$?") == -1)))
		{
			if (!check_pair_quotes(cmd))
			{
				perror("Invalid input quotes");
				exit(1);
			}
			cmd->args[i] = process_char(cmd->args[i], cmd->args[i], state, cmd);
			break ;
		}
		i++;
	}
	cmd->args[i] = copy_quotes_to_arg(cmd, &i);
}

void	allocate_args(t_command *cmd)
{
	cmd->args = malloc(sizeof(char *) * 1024);
	if (!cmd->args)
	{
		perror("Error allocating memory for args");
		exit(1);
	}
}
