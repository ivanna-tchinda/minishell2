#include "minishell.h"

char *tabtochar(char **tab)
{
    int i;
    int j;
    int i_tab;
    int len_tab;
    char *tab2;

    i = -1;
    j = 0;
    i_tab = -1;
    len_tab = 0;
    while(tab[++i])
        len_tab += ft_strlen(tab[i]) + 1;
    i = -1;
    tab2 = malloc(sizeof(char) * len_tab + 2);
    while(++i_tab < len_tab && tab[++i])
    {
        j = -1;
        while(tab[i] && ++j < (int)ft_strlen(tab[i]) && tab[i][j])
        {
            tab2[i_tab] = tab[i][j];
            i_tab++;
        }
        tab2[i_tab] = 32;
    }
    return(tab2[i_tab] = '\0', tab2);
}

char **add_toprompt(int nb_files, s_cmd *cmd, DIR *directory, struct dirent *direc)
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
                if (strcmp(direc->d_name, ".") != 0 && strcmp(direc->d_name, "..") != 0)
                    new_prompt[++i] = direc->d_name;
            }
            closedir(directory);
        } 
    } 
    new_prompt[i] = NULL;
    return (new_prompt);
}


char *allfiles_wcd(s_cmd *cmd)
{
    DIR *directory = NULL;
    int nb_files;
    struct dirent *direc = NULL;
    char **new_prompt;
    char *prompt;

    nb_files = 0;
    if (getcwd(cmd->tab, sizeof(cmd->tab)) != NULL) // Obtenir le répertoire de travail actuel
    {
        directory = opendir(cmd->tab);
        if (directory) 
        {
            while ((direc = readdir(directory)) != NULL) 
            {
                if (strcmp(direc->d_name, ".") != 0 && strcmp(direc->d_name, "..") != 0 )
                    nb_files++;
            }
            closedir(directory);
        } 
    } 
    new_prompt = add_toprompt(nb_files, cmd, directory, direc);
    prompt = tabtochar(new_prompt);
    return(prompt);
}



char **add_toprompt2(int nb_files, s_cmd *cmd, DIR *directory, struct dirent *direc, char *before_wcd)
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
                if (strcmp(direc->d_name, ".") != 0 && strcmp(direc->d_name, "..") != 0 && !strncmp(direc->d_name, before_wcd, ft_strlen(before_wcd)))
                    new_prompt[++i] = direc->d_name;
            }
            closedir(directory);
        } 
    } 
    new_prompt[++i] = NULL;
    return (new_prompt);
}

char **add_toprompt3(int nb_files, s_cmd *cmd, DIR *directory, struct dirent *direc, char *after_wcd)
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
                    && end_match(direc->d_name, after_wcd))
                    new_prompt[++i] = direc->d_name;
            }
            closedir(directory);
        } 
    } 
    new_prompt[++i] = NULL;
    return (new_prompt);
}

char *before_wcd(char *prompt, s_cmd *cmd)
{
    int len_before;
    DIR *directory;
    int nb_files;
    struct dirent *direc;
    char **new_prompt;
    char *before_wcd;

    nb_files = 0;
    len_before = -1;
    while(prompt[++len_before] && prompt[++len_before] != 42);
    before_wcd = ft_substr(prompt, 0, len_before);
    if (getcwd(cmd->tab, sizeof(cmd->tab)) != NULL)
    {
        directory = opendir(cmd->tab);
        if (directory) 
        {
            while ((direc = readdir(directory)) != NULL) 
            {
                if (strcmp(direc->d_name, ".") != 0 
                    && strcmp(direc->d_name, "..") != 0
                    && !strncmp(direc->d_name, before_wcd, ft_strlen(before_wcd)))
                    nb_files++;
            }
            closedir(directory);
        } 
    }
    if(nb_files)
    {
        new_prompt = add_toprompt2(nb_files, cmd, directory, direc, before_wcd);
        prompt = tabtochar(new_prompt);
    }
    return(prompt);
}

int end_match(char *name, char *ext)
{
    int i;
    int len;

    i = ft_strlen(name) - 1;
    len = ft_strlen(ext) - 1;
    while(len >= 0)
    {
        if(ext[len] != name[i])
            return 0;
        i--;
        len--;
    }
    return(1);
}

char *after_wcd(char *prompt, s_cmd *cmd)
{
    int len_after;
    DIR *directory;
    int nb_files;
    struct dirent *direc;
    char **new_prompt;
    char *after_wcd;

    nb_files = 0;
    len_after = ft_strlen(prompt) - 1;
    after_wcd = ft_substr(prompt, 1, len_after);
    if (getcwd(cmd->tab, sizeof(cmd->tab)) != NULL)
    {
        directory = opendir(cmd->tab);
        if (directory) 
        {
            while ((direc = readdir(directory)) != NULL) 
            {
                if (strcmp(direc->d_name, ".") != 0 
                    && strcmp(direc->d_name, "..") != 0
                    && end_match(direc->d_name, after_wcd))
                    nb_files++;
            }
            closedir(directory);
        } 
    }
    if(nb_files)
    {
        new_prompt = add_toprompt3(nb_files, cmd, directory, direc, after_wcd);
        prompt = tabtochar(new_prompt);
    }
    return(prompt);
}