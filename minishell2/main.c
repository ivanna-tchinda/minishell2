#include "minishell.h"

// void set_envir(char *envp[])

int main(int ac, char **av, char *envp[])
{
    (void)ac;
    (void)av;
    (void)envp;
    s_token token[1000];
    s_cmd prompt;
    char *line;
    // set_envir(envp);
    while(1)
    {
        line = readline("\033[0;36m\033[1m minishell> \033[0m");
        if(line && ft_parsing(&prompt, token, line))
        {
            prompt.exitstatus = 1;
            printf("err\n");
            continue;
        }
        add_history(line);
        pipex(&prompt, envp);
        free_token(token);
        free(line);
    }
    return(0);
}