#include "minishell.h"

int between_match(char *name, char *ext)
{
    int i;
    int len;

    i = -1;
    len = 0;
    if(!ext)
        return(1);
    while(name[++i])
    {
        len = 0;
        while(name[i] == ext[len] && name[i] && ext[len])
        {
            i++;
            len++;
        }
        if(len == (int)ft_strlen(ext))
            return (1);
    }
    return(0);
}

char **add_toprompt4(int nb_files, s_cmd *cmd, DIR *directory, struct dirent *direc, char *between_wcd)
{
    char **new_prompt;
    int i = -1;

    new_prompt = (char **)malloc(sizeof(char *) * nb_files + 1);
    if (getcwd(cmd->tab, sizeof(cmd->tab)) != NULL) // Obtenir le répertoire de travail actuel
    {
        directory = opendir(cmd->tab);
        if (directory) 
        {
            while ((direc = readdir(directory)) != NULL) 
            {
                if (strcmp(direc->d_name, ".") != 0 
                    && strcmp(direc->d_name, "..") != 0 
                    && between_match(direc->d_name, between_wcd))
                    new_prompt[++i] = direc->d_name;
            }
            closedir(directory);
        } 
    } 
    new_prompt[++i] = NULL;
    return (new_prompt);
}

char *between_wcd(char *prompt, s_cmd *cmd)
{
    int len_between;
    DIR *directory;
    int nb_files;
    struct dirent *direc;
    char **new_prompt;
    char *between_wcd;

    nb_files = 0;
    len_between = ft_strlen(prompt) - 2;
    between_wcd = ft_substr(prompt, 1, len_between);
    if (getcwd(cmd->tab, sizeof(cmd->tab)) != NULL)
    {
        directory = opendir(cmd->tab);
        if (directory) 
        {
            while ((direc = readdir(directory)) != NULL) 
            {
                if (strcmp(direc->d_name, ".") != 0 
                    && strcmp(direc->d_name, "..") != 0
                    && between_match(direc->d_name, between_wcd))
                    nb_files++;
            }
            closedir(directory);
        } 
    }
    if(nb_files)
    {
        new_prompt = add_toprompt4(nb_files, cmd, directory, direc, between_wcd);
        prompt = tabtochar(new_prompt);
    }
    return(prompt);
}