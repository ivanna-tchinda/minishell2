#include "minishell.h"

char **var_envir = NULL;

void set_envir(char *envp[])
{
    int len_env;
    int i;
    (void)i;

    len_env = -1;
    i = -1;
    while(envp[++len_env]);
    var_envir = (char **)malloc(sizeof(char *) * len_env + 1);
    while(++i < len_env)
    {
        var_envir[i] = ft_strdup(envp[i]);
    }
    var_envir[i] = NULL;
}

int minishell(char *envp[])
{
    s_cmd prompt;
    char *line;
    set_envir(envp);
    while(1)
    {
        s_token token[1000];
        line = readline("\033[0;36m\033[1m minishell> \033[0m");
        if(!strcmp(line, ":") || !line[0])
        {
            if(!strcmp(line, ":"))
                prompt.exitstatus = 0;
            // free_token(token);
            add_history(line);
            continue;
        }
        else if((line && ft_parsing(&prompt, token, line)))
        {
            // free_token(token);
            add_history(line);
            continue;
        }
        add_history(line);
        expand_cmd(&prompt);
        expand_status(&prompt);
        // signals_d();
        ft_exec(&prompt, token);
        free_token(token);//
    }
    return(1);
}

int main(int ac, char **av, char **envp)
{
    (void)ac;
    (void)av;
    // allsignals();
    minishell(envp);
    return 0;
}