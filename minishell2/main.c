#include "minishell.h"



int main(int ac, char **av, char *envp[])
{
    (void)ac;
    (void)av;
    (void)envp;
    s_token token[1000];
    s_cmd prompt;
    char *line;
    while(1)
    {
        line = readline("\033[0;36m\033[1m minishell> \033[0m");
        ft_parsing(&prompt, token, line);
        add_history(line);
        free_token(token);
        free(line);
    }
    return(0);
}