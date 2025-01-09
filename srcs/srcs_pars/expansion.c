/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 03:04:17 by enzo              #+#    #+#             */
/*   Updated: 2025/01/08 18:57:39 by emagnani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (ft_strdup("")); // Return empty string if variable not found
}

static char	*process_expansion(const char *str, size_t *i, char **env)
{
	char	*var_name;
	char	*value;
	size_t	var_len;
	size_t	start;

	(*i)++; // Skip the '$'
	if (!str[*i] || str[*i] == ' ' || str[*i] == '\'' || str[*i] == '"')
		return (ft_strdup("$"));
	
	start = *i;
	// Find the end of the variable name
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


static char	*expand_env_vars(const char *str, char **env)
{
	char			*result;
	char			*expansion;
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
			expansion = process_expansion(str, &i, env);
			if (!expansion)
				return (free(result), NULL);
			result = ft_strjoin(result, expansion);
			free(expansion);
			continue; // Skip the ft_strjoinch for this iteration
		}
		result = ft_strjoinch(result, str[i]);
		i++;
	}
	return (result);
}


t_error start_dollar_expansion(t_ast_node *node, char **env)
{
	char	*expanded;
	int		i;

	printf("Expanding dollar signs reached!\n");
	// Expand command name
	expanded = expand_env_vars(node->data.command.command, env);
	if (!expanded)
		return (ERR_MALLOC);
	free(node->data.command.command);
	node->data.command.command = expanded;

	// Expand arguments
	if (node->data.command.args)
	{
		i = 0;
		while (i < node->data.command.arg_count)
		{
			expanded = expand_env_vars(node->data.command.args[i], env);
			if (!expanded)
				return (ERR_MALLOC);
			free(node->data.command.args[i]);
			node->data.command.args[i] = expanded;
			i++;
		}
	}
	return (SUCCESS);
}