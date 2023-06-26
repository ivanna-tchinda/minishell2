#include "minishell.h"

int exec_par(char *cmd, s_cmd *prompt, int prevpipe, int *i)
{
	(void)i;
	(void)prompt;
	char *path;
	char **args;
	int exitStatus = 0;
	pid_t pid;


	path = ft_path(cmd, var_envir);
	args = ft_split(cmd, ' ');
	// printf("path: %s args[0]: %s\n", path, args[0]);
	pid = fork();
	if(pid == 0)
	{
		dup2(prevpipe, STDIN_FILENO);
		close(prevpipe);
		// if(is_builtin(cmd))
		// 	exec_bltn(cmd, prompt);
		if(execve(path, args, var_envir) == -1)
		{
			prompt->exitstatus = 127;
			write(2, "minishell: cmd not found\n", 25);
			exit(1);
			// return(1);
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
        // printf("Processus parent : le processus fils s'est terminé avec le code de sortie %d %s\n", exitStatus, cmd);
	}
	return(exitStatus);
}