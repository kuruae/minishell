/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jbaumfal <jbaumfal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:06:49 by jbaumfal          #+#    #+#             */
/*   Updated: 2024/12/03 15:50:42 by jbaumfal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// void	child_1(char **argv, char **env, int *p)
// {
// 	int	file_1;

// 	close(p[0]);
// 	file_1 = open(argv[1], O_RDONLY);
// 	if (file_1 == -1)
// 	{
// 		close(p[1]);
// 		close(file_1);
// 		exit_error("can't open first file");
// 	}
// 	if (dup2(file_1, STDIN_FILENO) == -1)
// 	{
// 		close(p[1]);
// 		close(file_1);
// 		exit_error("dup 2");
// 	}
// 	if (dup2(p[1], STDOUT_FILENO) == -1)
// 	{
// 		close(p[1]);
// 		close(file_1);
// 		exit_error("dup 2 is failing");
// 	}
// 	close(file_1);
// 	close(p[1]);
// 	execute(argv[2], env);
// }

// void	child_2(char **argv, char **env, int *p)
// {
// 	int	file_2;

// 	close(p[1]);
// 	file_2 = open(argv[4], O_CREAT | O_RDWR | O_TRUNC, 0644);
// 	if (file_2 == -1)
// 	{
// 		close(p[0]);
// 		close(file_2);
// 		exit_error("can't open first file");
// 	}
// 	if (dup2(p[0], STDIN_FILENO) == -1)
// 	{
// 		close(p[0]);
// 		close(file_2);
// 		exit_error("dup 2");
// 	}
// 	if (dup2(file_2, STDOUT_FILENO) == -1)
// 	{
// 		close(p[0]);
// 		close(file_2);
// 		exit_error("dup 2 is failing");
// 	}
// 	close(file_2);
// 	close(p[0]);
// 	execute(argv[3], env);
// }

// void	exit_error(char *message)
// {
// 	perror(message);
// 	exit(EXIT_FAILURE);
// }

// int	check_arg(char **argv)
// {
// 	int	i;

// 	i = 1;
// 	while (argv[i])
// 	{
// 		if (ft_strlen(argv[i]) == 0)
// 			return (EXIT_FAILURE);
// 		if (ft_is_space(argv[i]) == 1)
// 			return (EXIT_FAILURE);
// 		i++;
// 	}
// 	return (EXIT_SUCCESS);
// }

// void	start_exec()
// {

// }

// int main(int argc, char **argv, char **env)
// {

// }
/*
int	main(int argc, char **argv, char **env)
{
	int			p[2];
	pid_t		pid[2];

	if (argc != 5)
		return (ft_putstr_fd("Wrong number of arguments\n", 2), 1);
	if (check_arg(argv) == EXIT_FAILURE)
		return (ft_putstr_fd("Invalid Argument\n", 2), 1);
	if (pipe(p) == -1)
		return (ft_putstr_fd("Pipe process failed\n", 2), 1);
	pid[0] = fork();
	if (pid[0] == -1)
		return (ft_putstr_fd("Fork process failed\n", 2), 1);
	if (pid[0] == 0)
		child_1(argv, env, p);
	pid[1] = fork();
	if (pid[1] == -1)
		return (ft_putstr_fd("Fork process failed\n", 2), 1);
	if (pid[1] == 0)
		child_2(argv, env, p);
	close(p[0]);
	close(p[1]);
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], NULL, 0);
	return (0);
}
*/
