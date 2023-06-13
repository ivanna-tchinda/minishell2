#include "minishell.h"

char	*ft_envir(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (strncmp(env[i], "PATH", 4) == 0)
			break ;
		i++;
	}
	return (env[i]);
}

char	*to_find(char **path, char *new_av)
{
	int		i;
	char	*to_find;

	i = 0;
	while (path[i])
	{
		to_find = ft_strjoin(path[i], new_av);
		free(path[i]);
		if (access(to_find, R_OK) == 0)
		{
			free(new_av);
			while (path[++i] != NULL)
				free(path[i]);
			free(path);
			return (to_find);
		}
		free(to_find);
		i++;
	}
	free(path);
	free(new_av);
	return (NULL);
}

char	*which_envbis(char *av, char **env)
{
	char	*new_av;
	char	**path;
	int		i;
	char	*old;

	i = 0;
	new_av = ft_strjoin("/", av);
	env[i] = ft_envir(env);
	if (!env[i])
		return (NULL);
	old = ft_substr(env[i], 5, ft_strlen(env[i]) - 5);
	path = ft_split(old, ':');
	free(old);
	return (to_find(path, new_av));
}

char	*ft_command(char *av)
{
	int		i;
	int		j;
	char	*command;

	i = 0;
	j = 0;
	while (av[i] == ' ')
		i++;
	j = i;
	while (av[j] != ' ' && av[j])
		j++;
	command = ft_substr(av, i, j);
	return (command);
}

char	*ft_path(char *av, char **envp)
{
	char	*goodenv;
	char	*command;

	command = ft_command(av);
	goodenv = which_envbis(command, envp);
	if (goodenv == NULL)
		return (command);
	free(command);
	return (goodenv);
}