#include "minishell.h"

int main(int ac, char **av, char *envp[])
{
    (void)ac;
    (void)av;
    s_token token[1000];
    s_cmd prompt;
    char *line;
	// allsignals();
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