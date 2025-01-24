/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emagnani <emagnani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 03:04:17 by enzo              #+#    #+#             */
/*   Updated: 2025/01/24 16:44:07 by emagnani         ###   ########.fr       */
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

/**
 * Expand environment variables in a string
 * 
 * @param str the string to expand
 * @param env the environment variables
 * 
 * first create an empty string to store the result
 * then iterate over the string
 * 
 * at each character, update the quote depth
 * to know if we should expand the variable or not
 * 
 * if we find a $ character, process the variable expansion
 * otherwise, just append the character to the result
 * 
 * @return the expanded string
 */
char	*expand_env_vars(const char *str, char **env)
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
			if (!result)
			{
				free(expansion);
				return (NULL);
			}
			free(expansion);
			continue ;
		}
		result = ft_strjoinch(result, str[i]);
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}

/**
 * Start the dollar expansion process for a command node
 * 
 * @param node	the command node to expand
 * @param env	the environment variables
 * 
 * expand the command name and all arguments
 * 
 * @return SUCCESS or ERR_MALLOC
 */
t_error	start_dollar_expansion(t_ast_node *node, char **env)
{
	char	*expanded;
	int		i;

	expanded = expand_env_vars(node->data.command.command, env);
	if (!expanded)
		return (ERR_MALLOC);
	free(node->data.command.command);
	node->data.command.command = expanded;
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
