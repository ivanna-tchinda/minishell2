#include "minishell.h"
void sigint(int signal) 
{
        (void)signal;
			write(STDERR_FILENO, "\n\033[0;36m\033[1m minishell> \033[0m", 29);
}

void allsignals()
{
    
	signal(SIGINT, sigint);
	signal(SIGQUIT, SIG_IGN);
}

void signals_d()
{
    char buffer[1];
    ssize_t bytesRead = read(STDIN_FILENO, buffer, 0);
    if (bytesRead == 0) 
    {
        write(STDERR_FILENO, "exit\n", 5);
        exit(0);
    }
}