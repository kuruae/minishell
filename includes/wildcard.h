/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: enzo <enzo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 10:16:31 by enzo              #+#    #+#             */
/*   Updated: 2024/12/31 11:02:18 by enzo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WILDCARD_H
# define WILDCARD_H

# include "minishell.h"
# include <dirent.h>


t_error start_wildcard_expansion(t_ast_node *node, char **env);

#endif