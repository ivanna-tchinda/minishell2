#include "minishell.h"


void ft_execve(char *cmd, int prevpipe, s_cmd *prompt, int *i, int outfile)
{
	char *path;
	char **args;
	pid_t pid;
	(void)i;

	cmd = without_spaces(cmd);
	path = ft_path(cmd, var_envir);
	args = ft_split(cmd, ' ');
	pid = fork();
	if(pid == 0)
	{
		
		dup2(prevpipe, STDIN_FILENO);
		dup2(outfile, STDOUT_FILENO);
		close(prevpipe);
		if(is_builtin(cmd))
			exec_bltn(cmd, prompt);
		else if(execve(path, args, var_envir) == -1)
		{
			free(path);
			prompt->exitstatus = 127;
			write(2, "minishell: cmd not found\n", 25);
			exit(0);
		}
	}
	else
	{
		close(prevpipe);
		close(outfile);
		wait(NULL);
	}
	return;
}


int exec_lastcmddoc2(char *cmd, s_cmd *prompt, int prevpipe, char *outfile, int *i)
{
	(void)i;
	(void)prompt;
	char *path;
	char **args;
	pid_t pid;


	path = ft_path(prompt->cmd[*i - 2].tab, var_envir);
	args = ft_split(prompt->cmd[*i - 2].tab, ' ');
	pid = fork();
	if(pid == 0)
	{
		if(outfile)
		{
			int out;
			out = open(outfile, O_TRUNC | O_CREAT | O_WRONLY, 0644);
			dup2(out, STDOUT_FILENO);
			close(out);
		}
		dup2(prevpipe, STDIN_FILENO);
		close(prevpipe);
		if(is_builtin(prompt->cmd[*i - 2].tab))
			exec_bltn(prompt->cmd[*i - 2].tab, prompt);
		if(execve(path, args, var_envir) == -1)
		{
			prompt->exitstatus = 127;
			write(2, "minishell: cmd not found\n", 25);
			exit(1);
		}
	}
	else
	{
		int childStatus;
        pid_t terminatedChildPid = wait(&childStatus);
		if (terminatedChildPid == -1) {
            perror("Erreur lors de l'appel à wait");
            exit(1);
        }
		int exitStatus = WEXITSTATUS(childStatus);
		if(exitStatus)
			prompt->exitstatus = 127;
        // printf("Processus parent : le processus fils s'est terminé avec le code de sortie %d\n", exitStatus);
		if(!exitStatus)
		{
			path = ft_path(cmd, var_envir);
			args = ft_split(cmd, ' ');
			execve(path, args, var_envir);
		}
	}
	return(0);
}

int exec_lastcmddoc(char *cmd, s_cmd *prompt, int prevpipe, char *outfile, int *i)
{
	(void)i;
	(void)cmd;
	(void)prompt;
	char *path;
	char **args;
	int exitStatus = 0;
	pid_t pid;


	path = ft_path(prompt->cmd[*i - 2].tab, var_envir);
	args = ft_split(prompt->cmd[*i - 2].tab, ' ');
	pid = fork();
	if(pid == 0)
	{
		if(outfile)
		{
			int out;
			out = open(outfile, O_TRUNC | O_CREAT | O_WRONLY, 0644);
			// printf("out: %d\n", out);
			dup2(out, STDOUT_FILENO);
			close(out);
		}
		dup2(prevpipe, STDIN_FILENO);
		close(prevpipe);
		if(is_builtin(prompt->cmd[*i - 2].tab))
			exec_bltn(prompt->cmd[*i - 2].tab, prompt);
		if(execve(path, args, var_envir) == -1)
		{
			prompt->exitstatus = 127;
			write(2, "minishell: cmd not found\n", 25);
			exit(1);
		}
	}
	else
	{
		int childStatus;
        pid_t terminatedChildPid = wait(&childStatus);
		if (terminatedChildPid == -1) {
            perror("Erreur lors de l'appel à wait");
            exit(1);
        }
		exitStatus = WEXITSTATUS(childStatus);
		if(exitStatus)
			prompt->exitstatus = 127;
		return(exitStatus);
	}
	return(exitStatus);
}

int is_directory(s_cmd *prompt, int *i)
{
	char **direc;

	direc = ft_split(prompt->cmd[*i].tab, ' ');
	if(strchr(direc[0], '/'))
		return(write(2, "minishell: Is a directory\n", 26));
	return(0);
}

int exec_lastcmd(s_cmd *prompt, int *i, int prevpipe, char *outfile)
{
	if(is_directory(prompt, i))
		return(prompt->exitstatus=126, 1);
	else if(is_builtin(prompt->cmd[(*i)].tab))
		return(exec_builtin(prompt, i, prevpipe, outfile));
	else
		return(exec_cmd(prompt, i, prevpipe, outfile));
}

int exec_cmd(s_cmd *prompt, int *i, int prevpipe, char *outfile)
{
	char *path;
	char **args;
	pid_t pid;
	int ret = 0;

	path = ft_path(prompt->cmd[(*i)].tab, var_envir);
	args = ft_split(prompt->cmd[(*i)].tab, ' ');
	pid = fork();
	if(outfile)
	{
		int out;
		out = open(outfile, O_TRUNC | O_CREAT | O_WRONLY, 0644);
		dup2(out, STDOUT_FILENO);
		close(out);
	}
	if(pid == 0)
	{
		
		dup2(prevpipe, STDIN_FILENO);
		close(prevpipe);
		if(is_builtin(prompt->cmd[(*i)].tab))
		{
			// printf("bltn %s\n", prompt->cmd[(*i)].tab);
			exec_bltn(prompt->cmd[(*i)].tab, prompt);
		}
		else if(execve(path, args, var_envir) == -1)
		{
			write(2, "minishell: cmd not found\n", 25);
			exit(1);
		}
	}
	else
	{
		close(prevpipe);
		int childStatus;
        pid_t terminatedChildPid = wait(&childStatus);
		if (terminatedChildPid == -1) {
            perror("Erreur lors de l'appel à wait");
            exit(1);
        }
		ret = WEXITSTATUS(childStatus);
		if(ret)
			prompt->exitstatus = 127;
		if(prompt->cmd[(*i)].tab && is_builtin(prompt->cmd[(*i)].tab))
			ret = 0;
        // printf("Processus parent : le processus fils s'est terminé avec le code de sortie %d\n", ret);
		
	}
	return(ret);
}

int exec_builtin(s_cmd *prompt, int *i, int prevpipe, char *outfile)
{
	pid_t pid;
	int ret = 0;

	pid = fork();
	if(outfile)
	{
		int out;
		out = open(outfile, O_TRUNC | O_CREAT | O_WRONLY, 0644);
		dup2(out, STDOUT_FILENO);
		close(out);
	}
	if(pid == 0)
		exec_bltn(prompt->cmd[(*i)].tab, prompt);
	else
	{
		close(prevpipe);
		int childStatus;
        pid_t terminatedChildPid = wait(&childStatus);
		if (terminatedChildPid == -1) {
            perror("Erreur lors de l'appel à wait");
            exit(1);
        }
		ret = WEXITSTATUS(childStatus);
		if(prompt->cmd[(*i)].tab && is_builtin(prompt->cmd[(*i)].tab))
			ret = 0;
        // printf("Processus parent : le processus fils s'est terminé avec le code de sortie %d\n", ret);
	}
	return(ret);
}