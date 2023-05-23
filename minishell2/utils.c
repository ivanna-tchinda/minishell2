#include "minishell.h"

void free_prompt(s_cmd *prompt)
{
    int i;

    i = 0;
    while(prompt[i].cmd)
    {
        free(prompt[i].cmd);
        i++;
    }

}

void free_token(s_token *token)
{
    int i;

    i = 0;
    while(token[i].token)
    {
        token[i].token = 0;
        token[i].type = 0;
        i++;
    }
}

void free_tab(char **tabl)
{
    int i;

    i = 0;
    while(tabl[i])
    {
        free(tabl[i]);
        i++;
    }
    free(tabl);
}