#include "minishell.h"

void cd_bltn(char *cmd)
{
    char **cd_cmd;

    cd_cmd = ft_split(cmd, ' ');
    run_cd(cd_cmd, var_envir);
}

static int perform_cd(const char *path, char **envp) 
{
    (void)envp;
	if (chdir(path) == 0) 
	{
		char *cwd = getcwd(NULL, 0);
		setenv("PWD", cwd, 1);
		free(cwd);
		return 0;
	} 		else 
	{
		perror("Erreur lors du changement de répertoire");
		return 1;
	}
}

int run_cd(char **args, char **envp) 
{
	if ((args[1] && args[2]) || !args[1]) 
		return 1;
	
	else 
	{
		// Cas où un chemin est spécifié
		if (args[1][0] == '/') 
		{
			perform_cd(args[1], envp);
			// Chemin absolu
			/*if (perform_cd(args[1], envp) == 0) 
			{
				printf("minishell> PWD = %s\n", getenv("PWD"));
			}*/
		} 
		else 
		{
			// Chemin relatif, ici je constru artificielment un chemin absolu
			char *cwd = getcwd(NULL, 0);
			size_t cwd_len = strlen(cwd);
			size_t arg_len = strlen(args[1]);
			char *new_path = malloc(cwd_len + arg_len + 2); // +2 pour le slash et le caractère nul
			strcpy(new_path, cwd);
			new_path[cwd_len] = '/';
			strcpy(new_path + cwd_len + 1, args[1]);

			perform_cd(new_path, envp);
			/*if (perform_cd(new_path, envp) == 0) 
			{
				printf("minishell> PWD = %s\n", getenv("PWD"));
			}*/

			free(new_path);
			free(cwd);
		}
	}
	return 0;
}