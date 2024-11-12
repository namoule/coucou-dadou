/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_and_ressources.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealefev <jealefev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 01:25:41 by jealefev          #+#    #+#             */
/*   Updated: 2024/11/08 17:50:55 by jealefev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	allocate_resources(char **result, t_state **state)
{
	*result = malloc(sizeof(char) * 4096);
	if (!(*result))
	{
		perror("Error allocating memory for result");
		exit(1);
	}
	(*result)[0] = '\0';
	*state = malloc(sizeof(t_state));
	if (!(*state))
	{
		perror("Error allocating memory for state");
		free(*result);
		exit(1);
	}
	ft_bzero((*state)->n, sizeof(int) * 2);
	(*state)->i = 0;
	(*state)->sq_open = 0;
	(*state)->dq_open = 0;
}

char	*initialize_result(char *result, size_t result_size)
{
	if (!result)
	{
		result = malloc(result_size);
		if (!result)
		{
			perror("Memory allocation failed for result");
			exit(1);
		}
		result[0] = '\0';
	}
	return (result);
}

void	allocate_token_quotes(t_command *cmd, char *quotes[], t_state *state)
{
	cmd->token_quotes = malloc(sizeof(char *) * (state->n[1] + 1));
	if (!cmd->token_quotes)
	{
		perror("Error allocating memory for token_quotes");
		exit(1);
	}
	copy_quote_strings(cmd, quotes, state);
}

char	*get_env_var(const char *name, int len, t_state *state)
{
	char	*env;

	env = getenv(name);
	if (env)
	{
		state->n[0] += len;
		state->i += len;
	}
	return (env);
}

char	*expand_env_var(const char *input, t_state *state)
{
	if (!ft_strncmp(&input[state->i + 1], "PATH", 4))
		return (get_env_var("PATH", 4, state));
	else if (!ft_strncmp(&input[state->i + 1], "HOME", 4))
		return (get_env_var("HOME", 4, state));
	else if (!ft_strncmp(&input[state->i + 1], "PWD", 3))
		return (get_env_var("PWD", 3, state));
	else if (!ft_strncmp(&input[state->i + 1], "OLDPWD", 6))
		return (get_env_var("OLDPWD", 6, state));
	else if (!ft_strncmp(&input[state->i + 1], "SHLVL", 5))
		return (get_env_var("SHLVL", 5, state));
	else if (!ft_strncmp(&input[state->i + 1], "_", 1))
		return (get_env_var("_", 1, state));
	else if (!ft_strncmp(&input[state->i + 1], "USER", 4))
		return (get_env_var("USER", 4, state));
	else if (!ft_strncmp(&input[state->i + 1], "TERM", 4))
		return (get_env_var("TERM", 4, state));
	else if (!ft_strncmp(&input[state->i + 1], "?", 1))
		return (get_exit_code(1, state));
	return (NULL);
}
