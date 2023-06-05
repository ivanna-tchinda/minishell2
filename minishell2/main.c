#include "minishell.h"

char **envir = NULL;

void set_envir(char *envp[])
{
    int len_env;
    int i;

    len_env = -1;
    i = -1;
    while(envp[++len_env]);
    envir = (char **)malloc(sizeof(char *) * len_env);
    while(++i < len_env)
        envir[i] = envp[i];
    envir[i] = NULL;
}

int main(int ac, char **av, char *envp[])
{
    (void)ac;
    (void)av;
    s_token token[1000];
    s_cmd prompt;
    char *line;
    set_envir(envp);
    while(1)
    {
        line = readline("\033[0;36m\033[1m minishell> \033[0m");
        if(line && ft_parsing(&prompt, token, line))
        {
            prompt.exitstatus = 1;
            free_token(token);
            add_history(line);
            free(line);
            continue;
        }
        add_history(line);
        expand_cmd(&prompt, envp);
        expand_status(&prompt);
        ft_pipex(&prompt, envp);
        free_token(token);
        free(line);
    }
    return(1);
}