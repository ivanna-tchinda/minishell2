#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void	sigint_handler(int signal)
{
	if (signal == SIGINT)
	{
		printf("\nctrl c\n");
		exit(0);
	}
}

void set_signal_action(void)
{
	// Déclaration de la structure sigaction
	struct sigaction	act;

	// Met à 0 tous les bits dans la structure,
	// sinon on aura de mauvaises surprises de valeurs
	// non-initialisées...
	bzero(&act, sizeof(act));
	// On voudrait invoquer la routine sigint_handler
	// quand on reçoit le signal :
	act.sa_handler = &sigint_handler;
	// Applique cette structure avec la fonction à invoquer
	// au signal SIGINT (ctrl-c)
	sigaction(SIGINT, &act, NULL);
}

void	block_signal(int signal)
{
//	L'ensemble des signaux à bloquer
	struct sigaction sigset;//sigset_t	sigset;

//	Initialise l'ensemble à 0
	sigemptyset(&sigset.sa_mask);
//	Ajoute le signal à l'ensemble
	//sigaddset(&sigset, signal);
//	Ajoute les signaux de l'ensemble aux signaux bloqués pour ce processus
	sigaction(signal, &sigset, NULL);//sigprocmask(SIG_BLOCK, &sigset, NULL);
/*if (signal == SIGQUIT)
		printf("\e[36mSIGQUIT (ctrl-\\) blocked.\e[0m\n");*/
}

void	unblock_signal(int signal)
{
//	L'ensemble des signaus à débloquer
	struct sigaction sigset;//sigset_t	sigset;

//	Initialise l'ensemble à 0
	sigemptyset(&sigset.sa_mask);
//	Ajoute le signal à l'ensemble
	//sigaddset(&sigset, signal);
//	Retire les signaux de l'ensemble des
//	signaux bloqués pour ce processus
	sigaction(signal, &sigset, NULL);//sigprocmask(SIG_UNBLOCK, &sigset, NULL);//sigaction(signal, &sigset, NULL);
	/*if (signal == SIGQUIT)
		printf("\e[36mSIGQUIT (ctrl-\\) unblocked.\e[0m\n");*/
}

int	signals_c_backslash()
{
	// Change l'action associée à SIGINT
	set_signal_action();

	block_signal(SIGQUIT);
//	Boucle infinie pour avoir le temps de faire ctrl-\ et
//	ctrl-c autant de fois que ça nous chante.

	while(1)
	{
//		Bloque le signal SIGINT le temps de lire la variable
//		globale.
		block_signal(SIGINT);
		int	g_unblock_sigquit = 0;
//	Si la routine de gestion de SIGINT a indiqué qu'elle a été invoquée dans la variable globale
		if (g_unblock_sigquit == 1)
		{
//			SIGINT (ctrl-c) a été reçu.
			printf("\n\e[36mSIGINT detected. Unblocking SIGQUIT\e[0m\n");
//			Débloque SIGINT et SIGQUIT
			unblock_signal(SIGINT);
			unblock_signal(SIGQUIT);
		}
//		Sinon, on débloque SIGINT et on continue la boucle
		else
			unblock_signal(SIGINT);
	}
	return (0);
}

void	signals_d()
{
	int fd = 0; // Descripteur de fichier correspondant à stdin (entrée standard)
    char buffer[1]; // Tampon pour lire les données
    ssize_t bytesRead; // Nombre d'octets lus
    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) 
    {
        // Continue la boucle tant que des octets sont lus depuis stdin
       
    }
    if (bytesRead == 0) 
    {
        // Si aucun octet n'a été lu, cela signifie que la fin du fichier a été atteinte (EOF)
        write(1, "exit\n", 5); // Écrit "exit" suivi d'un saut de ligne sur stdout (sortie standard)
        printf("\nctrl d\n"); // Affiche un message sur stdout
        exit(0); // Quitte le programme avec un code de retour 0 (succès)
    }
    exit(1); // Quitte le programme avec un code de retour 0 (succès)
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

static char	*ft_malloc(char const *s, unsigned int start, size_t len)
{
	char	*s2;

	if (ft_strlen(s) == 1 && start > ft_strlen(s))
		s2 = (char *)malloc(sizeof(char));
	else if (len > ft_strlen(s))
		s2 = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	else if (start > ft_strlen(s))
		s2 = (char *)malloc(sizeof(char));
	else if (len == ft_strlen(s) && start < len && len != 0)
		s2 = (char *)malloc(sizeof(char) * (len - start) + 1);
	else
		s2 = (char *)malloc(sizeof(char) * (len + 1));
	return (s2);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*s2;

	i = 0;
	if (!s)
		return (NULL);
	s2 = ft_malloc(s, start, len);
	if (!s2)
		return (NULL);
	while (i < len && s[i] && start < ft_strlen(s))
	{
		s2[i] = s[start];
		i++;
		start++;
	}
	s2[i] = '\0';
	return (s2);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*join;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	join = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!join)
		return (NULL);
	while (s1[i])
	{
		join[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		join[i + j] = s2[j];
		j++;
	}
	join[i + j] = '\0';
	return (join);
}

static int	count_words(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i] && s)
	{
		if (s[i] != c)
		{
			count++;
			while (s[i] != c && s[i])
				i++;
		}
		else
			i++;
	}
	return (count);
}

void	ft_allocate(char **s2, char const *s, char c)
{
	char	**tab_s2;
	char	*tmp;

	tmp = (char *)s;
	tab_s2 = s2;
	while (*tmp)
	{
		while (*s == c)
			++s;
		tmp = (char *)s;
		while (*tmp && *tmp != c)
			++tmp;
		if (*tmp == c || tmp > s)
		{
			*tab_s2 = ft_substr(s, 0, tmp - s);
			s = tmp;
			++tab_s2;
		}
	}
	*tab_s2 = NULL;
}

char	**ft_split(char const *s, char c)
{
	char	**s2;
	int		size;

	if (!s)
		return (NULL);
	size = count_words(s, c);
	s2 = (char **) malloc (sizeof(char *) * (size + 1));
	if (!s2)
		return (NULL);
	ft_allocate(s2, s, c);
	return (s2);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
	{
		return (0);
	}
	while ((unsigned char)*s1 == (unsigned char)*s2 && *s1 != '\0' && n - 1 > 0)
	{
		s1++;
		s2++;
		n--;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

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

void	free_tab(char **tab)
{
	int	i;

	i = 0;
	if (tab == NULL)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
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

int	ft_execve(char *path, char **args, char *envp[], char **av)
{
	//access a modifier apres selon le parsing et ou peut/se situera l'infile
	/*if (access(av[1], R_OK) == -1)
	  {
	  write(2, "zsh: permission denied\n", 23);
	  exit(EXIT_FAILURE);
	  }*/
	  (void)av;
	  					
	if (execve(path, args, envp) == -1)
	{
		free(path);
		free_tab(args);
		write(2, "zsh: command not found\n", 23);
		exit(EXIT_FAILURE);
	}
	return (0);
}

void handle_sigquit(int sig) {
    if (sig == SIGQUIT) {
        printf("Ctrl+D (SIGQUIT) détecté. Fermeture du minishell...\n");
        // Autres actions à effectuer avant la fermeture du minishell, le cas échéant
        exit(0); // Quitter le minishell
    }
}


int main(int argc, char* argv[], char* envp[]) 
{

//signal(SIGQUIT, (void*)handle_sigquit);

	if (argc >= 2) 
	{
		int num_commands = argc - 1;
		int prev_pipe[2]; // Descripteurs de fichier pour le tuyau précédent
		int next_pipe[2]; // Descripteurs de fichier pour le tuyau suivant

		// Initialisation du tuyau précédent
		pipe(prev_pipe);
		int i = 0;
		while(i < num_commands) 
		{
			char *command = ft_command(argv[i + 1]);
			char *path = ft_recup_path(command, envp);
			char **args = ft_split(argv[i + 1], ' ');
			if (path != NULL) 
			{
				not_last_command(i, num_commands, next_pipe);
				pid_t child_pid = fork();
				if (child_pid == 0) 
				{
					process_child(i, prev_pipe, next_pipe, num_commands);
					ft_execve(path, args, envp, argv);
					exit(EXIT_FAILURE);
				} 
				else if (child_pid > 0) 
				{
						process_father(i, prev_pipe, next_pipe, num_commands);
				}
				else 
				{
					printf("Erreur lors de la création du processus enfant.\n");
					exit(EXIT_FAILURE);
				}
			}
			i++;
		}
		close_and_wait(prev_pipe, num_commands);
		//signals_c_backslash();
	}

	return 0;
}