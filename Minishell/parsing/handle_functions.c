/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealefev <jealefev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 01:25:41 by jealefev          #+#    #+#             */
/*   Updated: 2024/11/08 17:51:05 by jealefev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_env_var(const char *input, char *result, t_state *state,
		size_t result_size)
{
	char	*env;

	env = expand_env_var(input, state);
	if (env)
	{
		ft_strlcat(result, env, result_size);
		return (1);
	}
	return (0);
}

void	handle_single_quotes(char *arg, char *quotes[], t_state *state)
{
	char	*str;

	state->sq_open = !state->sq_open;
	if (state->sq_open)
	{
		str = helper_quotes(arg, '\'', state);
		if (str == NULL)
			return ;
		quotes[state->n[1]] = ft_strdup(str);
		if (str[0] != '\0' && str)
			state->n[1]++;
		state->sq_open = 0;
		free(str);
	}
}

void	handle_double_quotes(char *arg, char *result, char *quotes[],
		t_state *state)
{
	char	*str;

	state->dq_open = !state->dq_open;
	if (state->dq_open)
	{
		str = helper_quotes(arg, '\"', state);
		if (str == NULL)
			return ;
		result = process_char_helper(str, result, state);
		if (result && result[0] != '\0')
		{
			quotes[state->n[1]] = ft_strdup(result);
			state->n[1]++;
			state->dq_open = 0;
			result = NULL;
		}
		free(str);
	}
}

void	handle_regular_char(char *arg, char *quotes[], t_state *state)
{
	quotes[state->n[1]] = malloc(2);
	if (!quotes[state->n[1]])
	{
		perror("Memory allocation failed");
		exit(1);
	}
	quotes[state->n[1]][0] = arg[state->n[0]];
	quotes[state->n[1]][1] = '\0';
	state->n[1]++;
}

void	handle_dollar_sign(char *arg, char *result, char *quotes[],
		t_state *state)
{
	char	*str;

	str = helper_quotes(arg, '\"', state);
	if (str == NULL)
		return ;
	result = process_char_helper(str, result, state);
	if (result)
	{
		quotes[state->n[1]] = ft_strdup(result);
		state->n[1]++;
		result = NULL;
	}
	free(str);
}
