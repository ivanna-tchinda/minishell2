#include "minishell.h"

void exit_bltn(s_cmd *prompt)
{
    printf("exit\n");
    prompt->exitstatus = 0;
    exit(0);
}