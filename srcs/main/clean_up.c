/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@42.com>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:18:10 by jbaumfal          #+#    #+#             */
/*   Updated: 2025/02/22 16:11:20 by jbaumfal         ###   ########.fr       */
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
		if (node->u_data.s_command.command)
			free(node->u_data.s_command.command);
		if (node->u_data.s_command.args)
			free_all(node->u_data.s_command.args);
		free(node);
		return ;
	}
}

void	clean_up_child(t_shell *shell)
{
	t_shell	*parrent_shell;

	parrent_shell = shell;
	free_envp(parrent_shell->envp);
	while (parrent_shell->parent_shell)
	{
		parrent_shell = parrent_shell->parent_shell;
		free(parrent_shell->subshell);
	}
	free_ast(parrent_shell->root_node);
}
