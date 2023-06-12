#include <stdio.h>
#include <unistd.h>
#include <string.h>

void execute_cmd(char *cmd)
{
    (void)cmd;
    char *args[] = {"/usr/bin/ls", NULL};
    char *envp[] = {NULL};
    printf("%d\n", execve(cmd, args, envp));
}

void ft_pipex(char **tab)
{
    int fd[2];
    int infile1 = open()
    int i = 0;
    while(tab[i])
    {
        if(!strcmp(tab[i], "/usr/bin/ls"))
            execute_cmd(tab[i]);
        printf("tab: %s\n", tab[i]);
        i++;
    }
}

int main(void)
{
    char *tab[] = {"/usr/bin/ls", "|", "pwd", NULL};
    ft_pipex(tab);
    return (0);
}