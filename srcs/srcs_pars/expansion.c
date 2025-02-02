/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 03:04:17 by enzo              #+#    #+#             */
/*   Updated: 2025/01/24 17:57:26 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/**
 * gets the value of an environment variable
 * 
 * @param var_name	the name of the variable
 * @param env		the environment variables
 * 
 * iterate over the env table
 * the var_name acts as a key to find the value
 * 
 * @return the value of the variable or an empty string
 */
static char	*get_env_value(const char *var_name, char **env)
{
	size_t	var_len;
	size_t	i;

	if (!var_name || !env)
		return (NULL);
	var_len = ft_strlen(var_name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var_name, var_len) == 0
			&& env[i][var_len] == '=')
			return (ft_strdup(env[i] + var_len + 1));
		i++;
	}
	return (ft_strdup(""));
}

/**
 * process a variable expansion in the form of $VAR
 * for the current string
 * 
 * @param str	the string to process
 * @param i		the index of the current character
 * @param env	the environment variables
 * 
 * skip the $ character and find the end of the variable name
 * then get the value of the variable from the environment
 * $? returns the exit status of the last command
 * 
 * @return the expanded variable value
*/
static char	*process_expansion(const char *str, size_t *i, char **env)
{
	char	*var_name;
	char	*value;
	size_t	var_len;
	size_t	start;

	(*i)++;
	if (!str[*i] || str[*i] == ' ' || str[*i] == '\'' || str[*i] == '"')
		return (ft_strdup("$"));
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(g_sig_offset));
	}
	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var_len = *i - start;
	var_name = ft_substr(str, start, var_len);
	if (!var_name)
		return (NULL);
	value = get_env_value(var_name, env);
	free(var_name);
	return (value);
}

static char	*process_dollar_var(const char *str, size_t *i,
			char **env, char *result)
{
	char	*expansion;
	char	*new_result;

	expansion = process_expansion(str, i, env);
	if (!expansion)
		return (free(result), NULL);
	new_result = ft_strjoin(result, expansion);
	free(expansion);
	return (new_result);
}

static char	*build_expanded_str(const char *str, size_t *i, char *result)
{
	char	*new_result;

	new_result = ft_strjoinch(result, str[*i]);
	(*i)++;
	return (new_result);
}

char	*expand_env_vars(const char *str, char **env)
{
	char			*result;
	t_quote_depth	depth;
	size_t			i;

	result = ft_strdup("");
	if (!result)
		return (NULL);
	depth = (t_quote_depth){0, 0};
	i = 0;
	while (str[i])
	{
		update_quote_depth(&depth, str[i]);
		if (str[i] == '$' && should_expand(depth))
		{
			result = process_dollar_var(str, &i, env, result);
			if (!result)
				return (NULL);
			continue ;
		}
		result = build_expanded_str(str, &i, result);
		if (!result)
			return (NULL);
	}
	return (result);
}
