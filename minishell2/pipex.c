/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melissaadjogoua <marvin@42.fr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 11:08:21 by melissaadjogo     #+#    #+#             */
/*   Updated: 2023/06/05 16:01:08 by melissaadjogo    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

sig_atomic_t g_sigsigint = 0;
sig_atomic_t g_sigexit_status = 0;
sig_atomic_t g_sigpid = 0;
sig_atomic_t g_sigsigquit = 0;


char	**ft_recup_envp(char **envp)
{
	char	*env;
	char	**new_env;
	int		i;

	env = NULL;
	new_env = NULL;
	i = 0;
	if (envp)
	{
		while (envp[i] != NULL)
		{
			if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			{
				env = envp[i] + 5; 
				break ;
			}
			i++;
		}
	}
	if (!env)
		return (new_env);
	else
		new_env = ft_split(env, ':');
	return (new_env);
}


char	*ft_recup_path(char *command, char **envp)
{
	char	**allpath;
	char	*allpathnew;
	char	*cmdpath;
	int		j;

	allpath = ft_recup_envp(envp);
	j = 0;
	if (!allpath)
		return (command);
	while (allpath[++j])
	{
		allpathnew = ft_strjoin(allpath[j], "/");
		cmdpath = ft_strjoin(allpathnew, command); 
		if (access(cmdpath, R_OK) != -1)
		{
			free_tab(allpath);
			free(allpathnew);
			//vars->flag = 1;
			return (cmdpath); 
		}	  
		free(allpathnew);
		free(cmdpath); 
	} 
	free_tab(allpath);
	return (command);
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

// Si ce n'est pas la dernière commande, créez un nouveau tuyau
void not_last_command(int i, int num_commands, int *next_pipe)
{
	if (i < num_commands - 1) 
	{
		pipe(next_pipe);
	}
}

void	process_child(int i, int *prev_pipe, int *next_pipe, int num_commands)
{
	// Code du processus enfant
	// Redirection des descripteurs de fichier pour l'entrée et la sortie
	if (i > 0) 
	{
		dup2(prev_pipe[0], STDIN_FILENO);
		close(prev_pipe[1]);
		close(prev_pipe[0]);
	}

	if (i < num_commands - 1) 
	{
		dup2(next_pipe[1], STDOUT_FILENO);
		close(next_pipe[0]);
		close(next_pipe[1]);
	}
}

void	process_father(int i, int *prev_pipe, int *next_pipe, int num_commands)
{
	if (i > 0) 
	{
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}

	if (i < num_commands - 1) 
	{
		prev_pipe[0] = next_pipe[0];
		prev_pipe[1] = next_pipe[1];
	}
}

// Fermeture des descripteurs de fichier restants dans le processus parent
// et attente de la terminaison de tous les processus enfants
void	close_and_wait(int *prev_pipe, int num_commands)
{
	close(prev_pipe[0]);
	close(prev_pipe[1]);

	int i = 0;
	while (i++ < num_commands) 
	{
		wait(NULL);
	}
}

int	ft_execve(char *path, char **args, char *envp[])
{
	if (execve(path, args, envp) == -1)
	{
		free(path);
		free_tab(args);
		write(2, "zsh: command not found\n", 23);
		exit(EXIT_FAILURE);
	}
	return (0);
}

void sigint(int signal) 
{
	(void)signal;
		if (g_sigpid == 0) 
		{
			write(STDERR_FILENO, "\n\033[0;36m\033[1m👍 minishell> \033[0m", 29);
			g_sigexit_status = 1;
		} 
		else 
		{
			printf("\n");
			g_sigexit_status = 130;
		}
		g_sigsigint = 1;
}

void sigquit(int signal) 
{
	(void)signal;
		if (g_sigsigint) 
			g_sigsigint = 1;
		else 
			write(STDERR_FILENO, "\b\b  \b\b", 6);	
}


void allsignals()
{
	signal(SIGINT, sigint);
	signal(SIGQUIT, sigquit);
}

int ft_pipex(s_cmd *prompt, char* envp[]) 
{
	int prev_pipe[2];
	int next_pipe[2]; 
	
	pipe(prev_pipe);
	int i = 0;
	while(i < prompt->nb_tabs) 
	{
		if(is_builtin(prompt->cmd[i].tab))
			exec_bltn(prompt->cmd[i].tab);
		else if(strcmp(prompt->cmd[i].type, "char") == 0)
		{
			char *command = ft_command(prompt->cmd[i].tab);
			char *path = ft_recup_path(command, envp);
			char **args = ft_split(prompt->cmd[i].tab, ' ');
			if (path != NULL) 
			{
				not_last_command(i, prompt->nb_tabs, next_pipe);
				pid_t child_pid = fork();
				if (child_pid == 0) 
				{
					process_child(i, prev_pipe, next_pipe, prompt->nb_tabs);
					ft_execve(path, args, envp);
					exit(EXIT_FAILURE);
				} 
				else if (child_pid > 0) 
					process_father(i, prev_pipe, next_pipe, prompt->nb_tabs);
				else 
				{
					printf("Erreur lors de la création du processus enfant.\n");
					exit(EXIT_FAILURE);
				}
			}
		}
		i++;
	}
	close_and_wait(prev_pipe, prompt->nb_tabs);
	return 0;
}
