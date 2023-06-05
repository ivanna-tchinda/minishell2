#include "minishell.h"

void exec_cmd(s_cmd *prompt)
{
    int i;

    i = 0;
    while(i < prompt->nb_tabs)
    {
        //checker s'il y a un bltn
        if(!is_builtin(prompt->cmd[i].tab))
            exec_bltn(prompt->cmd[i].tab);
        //sinon si c'est pas un bltn
        //sinon c'est un pipe
        i++;
    }
}