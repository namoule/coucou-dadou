/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealefev <jealefev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 01:25:41 by jealefev          #+#    #+#             */
/*   Updated: 2024/11/08 17:50:21 by jealefev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	process_token_arguments(char **tokens, t_command *cmd, int *j)
{
	int	i;

	i = 0;
	while (tokens[*j] && ft_strcmp(tokens[*j], "|"))
	{
		cmd->args[i] = ft_strdup(tokens[*j]);
		if (!cmd->args[i])
		{
			perror("Error duplicating token");
			exit(1);
		}
		i++;
		(*j)++;
	}
	cmd->args[i] = NULL;
}

void	expand_tokens(char **tokens, t_command *cmd, t_state *state)
{
	int		i;
	char	*result;

	i = 0;
	result = NULL;
	while (tokens[i])
	{
		if (ft_strchr(tokens[i], '$') && (ft_strcmp(tokens[i], "$?") != 0))
		{
			if (ft_strchr(tokens[i], '\"') || ft_strchr(tokens[i], '\''))
				process_quotes(cmd, state);
			else
			{
				free(cmd->args[i]);
				cmd->args[i] = process_char_helper(tokens[i], result, state);
			}
		}
		i++;
	}
}

void	process_tokens(char **tokens, t_command *cmd, int *j)
{
	t_state	*state;

	state = initialize_t_state();
	allocate_args(cmd);
	process_token_arguments(tokens, cmd, j);
	expand_tokens(tokens, cmd, state);
	find_fd_in(cmd);
	find_fd_out(cmd);
}

void	split_and_process_tokens(t_command *cmd, char **tokens, t_table *table)
{
	int	j;

	j = 0;
	while (tokens[j])
	{
		process_tokens(tokens, cmd, &j);
		if (tokens[j] && ft_strcmp(tokens[j], "|") == 0)
		{
			new_t_command(cmd, table);
			cmd = cmd->next;
			j++;
		}
	}
}

t_command	*tokenize_input(char *input, char **envp, int return_value)
{
	char		**tokens;
	t_table		*table;
	t_command	*cmd;
	t_command	*head;

	if (!input)
		return (NULL);
	table = initialize_t_main(input, envp, &return_value);
	cmd = initialize_t_command(table);
	if (!cmd)
	{
		perror("Error allocating memory");
		return (NULL);
	}
	head = cmd;
	tokens = ft_split(input, ' ');
	split_and_process_tokens(cmd, tokens, table);
	freetab(tokens);
	return (head);
}
