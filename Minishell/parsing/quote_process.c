/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealefev <jealefev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 01:25:41 by jealefev          #+#    #+#             */
/*   Updated: 2024/11/08 17:51:58 by jealefev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_state_quotes(t_state *state)
{
	state->i = 0;
	state->sq_open = 0;
	state->dq_open = 0;
}

void	update_quotes(char ch, t_state *state, char *result, size_t result_size)
{
	if (ch == '\'' && !state->dq_open)
	{
		state->sq_open = !state->sq_open;
		ft_strlcat(result, "\'", result_size);
	}
	else if (ch == '\"' && !state->sq_open)
	{
		state->dq_open = !state->dq_open;
		ft_strlcat(result, "\"", result_size);
	}
}

char	*process_char_helper(const char *input, char *result, t_state *state)
{
	size_t	result_size;
	char	temp[2];

	result_size = 4096;
	result = initialize_result(result, result_size);
	while (input[state->i])
	{
		temp[0] = input[state->i];
		temp[1] = '\0';
		update_quotes(input[state->i], state, result, result_size);
		if (input[state->i] == '$' && !state->sq_open)
			handle_env_var(input, result, state, result_size);
		else if (!state->dq_open)
			state->n[0] = state->i + 1;
		else
			ft_strlcat(result, temp, result_size);
		state->i++;
	}
	handle_state_quotes(state);
	return (result);
}

char	*process_char(char *input, char *result, t_state *state, t_command *cmd)
{
	allocate_resources(&result, &state);
	state->cmd = cmd;
	handle_quotes_and_expand(input, result, cmd, state);
	return (result);
}

void	handle_quotes_and_expand(char *arg, char *result, t_command *cmd,
		t_state *state)
{
	char	*quotes[4096];

	ft_bzero(quotes, sizeof(char *) * 4096);
	while (arg[state->n[0]])
	{
		if (arg[state->n[0]] == '\'')
			handle_single_quotes(arg, quotes, state);
		else if (arg[state->n[0]] == '\"')
			handle_double_quotes(arg, result, quotes, state);
		else if (arg[state->n[0]] == '$' && !state->sq_open)
			handle_dollar_sign(arg, result, quotes, state);
		else if (!ft_strchr(arg, '$'))
			handle_regular_char(arg, quotes, state);
		if (state->n[1] >= 4096)
		{
			perror("Too many quoted strings");
			exit(1);
		}
		state->n[0]++;
	}
	allocate_token_quotes(cmd, quotes, state);
}
