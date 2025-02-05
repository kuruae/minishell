/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:18:10 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/05 15:44:37 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_envp(char ***envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	if (!*envp)
		return ;
	while (envp[0][i])
		free(envp[0][i++]);
	free(*envp);
	free(envp);
}

void	clean_up_parser(t_parser *parser)
{
	if (!parser)
		return ;
	if (parser->tokens)
		free_tokens(parser->tokens);
	free(parser);
}

void	clean_up_end(t_shell *shell)
{
	free_envp(shell->envp);
	rl_clear_history();
}

void	clean_up_node(t_ast_node *node)
{
	if (node->type == NODE_COMMAND)
	{
		if (node->data.command.command)
			free(node->data.command.command);
		if (node->data.command.args)
			free_all(node->data.command.args);
		free(node);
		return ;
	}
}
