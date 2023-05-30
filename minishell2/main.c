#include "minishell.h"

// void set_envir(char *envp[])

void free_pmt(s_cmd *prompt)
{
    int i;

    i = 0;
    while(i < prompt->nb_tabs)
    {
        prompt->cmd[i].tab = NULL;
        prompt->cmd[i].type = NULL;
        i++;
    }
}

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
            free_token(token);
            free(line);
            // free_pmt(&prompt);
            continue;
        }
        add_history(line);
        if(is_builtin(&prompt))
        {
            printf("we have to expand\n");
            expand_cmd(&prompt);
        }
        // pipex(&prompt, envp);
        // free_pmt(&prompt);
        free_token(token);
        free(line);
    }
    return(1);
}