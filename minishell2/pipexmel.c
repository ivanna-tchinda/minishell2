#include "minishell.h"

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

char* get_command_path(const char* command) 
{
    char* path = NULL;
    char command_line[256];
    FILE* pipe;

    // Construction de la commande "which <command>"
    snprintf(command_line, sizeof(command_line), "which %s", command);

    // Exécution de la commande "which" dans un pipe
    pipe = popen(command_line, "r");
    if (pipe != NULL) 
    {
        // Lecture de la sortie du pipe
        char output[256];
        if (fgets(output, sizeof(output), pipe) != NULL) 
        {
            // Suppression du saut de ligne à la fin
            char* newline = strchr(output, '\n');
            if (newline != NULL) 
            {
                *newline = '\0';
            }
            // Allocation et copie du chemin complet
            path = strdup(output);
        }
        pclose(pipe);
    }
    return path;
}


int main(int argc, char* argv[], char *envp[]) {
    if (argc >= 3) {
        int num_commands = argc - 2; // Nombre de commandes (à partir de av[2])
        char **commands = argv + 2; // Pointeur vers le tableau des commandes

        int prev_pipe[2]; // Descripteurs de fichier pour le tuyau précédent
        int next_pipe[2]; // Descripteurs de fichier pour le tuyau suivant

        // Initialisation du tuyau précédent
        pipe(prev_pipe);

        for (int i = 0; i < num_commands; i++) {
            char* command = commands[i];
            char* path = get_command_path(command);
            char **args = ft_split(command, ' ');

            // Vérification des valeurs de path et args
            if (path != NULL && args != NULL) {
                // Si ce n'est pas la dernière commande, créez un nouveau tuyau
                if (i < num_commands - 1) {
                    pipe(next_pipe);
                }

                pid_t child_pid = fork();
                if (child_pid == 0) {
                    // Code du processus enfant

                    // Redirection des descripteurs de fichier pour l'entrée et la sortie
                    if (i > 0) {
                        dup2(prev_pipe[0], STDIN_FILENO);
                        close(prev_pipe[1]);
                        close(prev_pipe[0]);
                    }

                    if (i < num_commands - 1) {
                        dup2(next_pipe[1], STDOUT_FILENO);
                        close(next_pipe[0]);
                        close(next_pipe[1]);
                    }

                    execve(path, args, envp);
                    exit(EXIT_FAILURE); // Sortie du processus enfant en cas d'échec de l'exécution de la commande
                } else if (child_pid > 0) {
                    // Code du processus parent

                    // Fermeture des descripteurs de fichier utilisés par le processus parent
                    if (i > 0) {
                        close(prev_pipe[0]);
                        close(prev_pipe[1]);
                    }

                    if (i < num_commands - 1) {
                        prev_pipe[0] = next_pipe[0];
                        prev_pipe[1] = next_pipe[1];
                    }
                } else {
                    fprintf(stderr, "Erreur lors de la création du processus enfant.\n");
                    exit(EXIT_FAILURE);
                }

                free(path);
                free(args);
            } else {
                fprintf(stderr, "Erreur lors de l'extraction du chemin ou des arguments de la commande '%s'.\n", command);
            }
        }

        // Fermeture des descripteurs de fichier restants dans le processus parent
        close(prev_pipe[0]);
        close(prev_pipe[1]);

        // Attente de la terminaison de tous les processus enfants
        for (int i = 0; i < num_commands; i++) {
            wait(NULL);
        }
    } else {
        printf("Veuillez spécifier au moins une commande en tant qu'argument.\n");
    }

    return 0;
}