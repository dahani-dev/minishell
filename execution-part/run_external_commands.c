/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_external_commands.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-all <mait-all@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 09:14:22 by mait-all          #+#    #+#             */
/*   Updated: 2025/05/20 20:40:30 by mait-all         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*get_env_path(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
	{
		if (ft_strncmp("PATH=", env[i], 5) == 0)
			return (env[i]);
		i++;
	}
	return (NULL);
}

static char	*get_path_name(char *cmd, char **env)
{
	char	*env_path;
	char	*holder;
	char	*binary_path;
	char	**exec_dirs;
	int		i;

	env_path = get_env_path(env);
	exec_dirs = ft_split(env_path, ':');
	i = 0;
	while (exec_dirs && exec_dirs[i])
	{
		holder = ft_strjoin(exec_dirs[i], "/");
		binary_path = ft_strjoin(holder, cmd);
		if (access(binary_path, F_OK) == 0 && access(binary_path, X_OK) == 0)
			return (binary_path);
		i++;
	}
	return (NULL);
}

static int	is_directory(char *path)
{
	struct stat	sb;

	if (stat(path, &sb) == -1)
		return (0);
	return (S_ISDIR(sb.st_mode));
}

static char	*get_exec_path(char **env, char *cmd)
{
	if (!cmd || !cmd[0])
		return (NULL);
	if (cmd && (cmd[0] == '.' || cmd[0] == '/'))
	{
		if (is_directory(cmd))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(cmd, STDERR_FILENO);
			ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
			ft_exit(126);
		}
		if (access(cmd, F_OK) == 0)
		{
			if (access(cmd, X_OK) == 0)
				return (cmd);
			else
				return ("no permission");
		}
		return ("no file");
	}
	return (get_path_name(cmd, env));
}

void	execute_command(char **args, char **env)
{
	char	*path;

	if (!args)
		ft_exit(0);
	path = get_exec_path(env, args[0]);
	manage_shell_errors(args[0], path);
	if (!path || ft_strncmp(path, "no permission", ft_strlen(path)) == 0)
		custom_error(ERR_PERMISSION, args[0], 126, false);
	execve(path, args, env);
	perror("execve: ");
	ft_exit(EXIT_FAILURE);
}
