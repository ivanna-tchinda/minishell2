#include "minishell.h"

// void execute(s_cmd cmd, char *env[])
// {
//     if(strcmp(cmd, "char") == 0)
//         execve(path, cmd, env)
// }

void process(s_info *cmd, int *fd, char *env[])
{
    (void)cmd;
    printf("ok\n");
    int pid;
    const char *path = {"/usr/bin/ls"};
    char *args[] = {"/usr/bin/ls", NULL};

    pid = fork();
    if(pipe(fd) == -1)
        return;
    if(pid == -1)
        return;
    (void)path;
    (void)args;
    (void)env;
    if(pid == 0)
    {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        execve(path, args, env);
    }
    else
    {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        waitpid(pid, NULL, 0);
    }
    exit(0);
}

int assign_files(s_cmd *cmd)
{
    int i;

    i = -1;
    while(++i < cmd->nb_tabs)
    {
        if(strcmp(cmd->cmd[i].type, "redirin") == 0)
        {
            cmd->infile = cmd->cmd[i - 1].tab;
            while(strcmp(cmd->cmd[++i].type, "redirout") != 0 && cmd->cmd[i].tab);
            if(i  + 1 <= cmd->nb_tabs)
                return(cmd->outfile = cmd->cmd[i + 1].tab, 0);
            else
                return(cmd->outfile = "/dev/stdout", 0);
        }
    }
    if(!cmd->infile)
        cmd->infile = "/dev/stdin";
    if(!cmd->outfile)
        cmd->outfile = "/dev/stdout";
    return(0);
}

void pipex(s_cmd *cmd, char *env[])
{
    int fd[2];
    (void)env;
    int i;

    i = 0;
    assign_files(cmd);
    printf("infile: %s outfile: %s\n", cmd->infile, cmd->outfile);
    fd[0] = open(cmd->infile, O_RDONLY, 0777);
    if(fd[0] < 0)
    {
        write(2, "infile: no such file or directory\n", 34);
        return;
    }
    fd[1] = open(cmd->outfile, O_CREAT | O_RDWR, 0777);
    while(i + 1 < cmd->nb_cmd)
    {
        process(&cmd->cmd[i], fd, env);
        i++;
    }
    process(&cmd->cmd[i], fd, env);
}