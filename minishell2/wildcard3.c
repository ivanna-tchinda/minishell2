#include "minishell.h"

char **add_toprompt5(int nb_files, s_cmd *cmd, DIR *directory, struct dirent *direc, char *between_wcd, char *after_wcd)
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
                    && between_match(direc->d_name, between_wcd)
                    && end_match(direc->d_name, after_wcd))
                    new_prompt[++i] = direc->d_name;
            }
            closedir(directory);
        } 
    } 
    new_prompt[++i] = NULL;
    return (new_prompt);
}

char **add_toprompt6(int nb_files, s_cmd *cmd, DIR *directory, struct dirent *direc, char *between_wcd, char *before_wcd)
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
                    && between_match(direc->d_name, between_wcd)
                    && !strncmp(direc->d_name, before_wcd, ft_strlen(before_wcd)))
                    new_prompt[++i] = direc->d_name;
            }
            closedir(directory);
        } 
    } 
    new_prompt[++i] = NULL;
    return (new_prompt);
}

char *aftertwo_wcd(char *prompt, s_cmd *cmd) // *o*.c
{
    (void)cmd;
    int len_between;
    int len_after;
    DIR *directory;
    int nb_files;
    struct dirent *direc;
    char **new_prompt;
    char *between_wcd;
    char *after_wcd;

    nb_files = 0;
    len_between = 0;
    while(prompt[++len_between] && prompt[len_between] != 42);
    len_after = len_between;
    while(prompt[++len_after]);
    len_after -= len_between + 1;
    len_between--;
    between_wcd = ft_substr(prompt, 1, len_between);
    after_wcd = ft_substr(prompt, len_between + 2, len_after);
    if (getcwd(cmd->tab, sizeof(cmd->tab)) != NULL)
    {
        directory = opendir(cmd->tab);
        if (directory) 
        {
            while ((direc = readdir(directory)) != NULL) 
            {
                if (strcmp(direc->d_name, ".") != 0 
                    && strcmp(direc->d_name, "..") != 0
                    && between_match(direc->d_name, between_wcd)
                    && end_match(direc->d_name, after_wcd))
                    nb_files++;
            }
            closedir(directory);
        } 
    }
    if(nb_files)
    {
        new_prompt = add_toprompt5(nb_files, cmd, directory, direc, between_wcd, after_wcd);
        prompt = tabtochar(new_prompt);
    }
    return(prompt);
}

char *beforetwo_wcd(char *prompt, s_cmd *cmd) // a*o*
{
    int len_between;
    int len_before;
    DIR *directory;
    int nb_files;
    struct dirent *direc;
    char **new_prompt;
    char *between_wcd;
    char *before_wcd;

    nb_files = 0;
    len_before = -1;
    while(prompt[++len_before] && prompt[len_before] != 42);
    len_between = len_before;
    while(prompt[++len_between] != 42);
    len_between -= len_before + 1;
    between_wcd = ft_substr(prompt, len_before + 1, len_between);
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
                    && between_match(direc->d_name, between_wcd)
                    && !strncmp(direc->d_name, before_wcd, ft_strlen(before_wcd)))
                    nb_files++;
            }
            closedir(directory);
        } 
    }
    if(nb_files)
    {
        new_prompt = add_toprompt6(nb_files, cmd, directory, direc, between_wcd, before_wcd);
        prompt = tabtochar(new_prompt);
    }
    return(prompt);
}

char *alltwo_wcd(char *prompt, s_cmd *cmd) // a*o*.c
{
    (void)cmd;
    int len_between;
    int len_before;
    int len_after;
    DIR *directory;
    int nb_files;
    struct dirent *direc;
    char **new_prompt;
    char *between_wcd;
    char *before_wcd;
    char *after_wcd;

    len_before = -1;
    while(prompt[++len_before] && prompt[len_before] != 42);
    len_between = len_before;
    while(prompt[++len_between] != 42);
    len_between -= len_before + 1;
    len_after = ft_strlen(prompt);
    while(prompt[--len_after] && prompt[len_after] != 42);
    len_after = ft_strlen(prompt) - len_after - 1;
    nb_files = 0;
    before_wcd = ft_substr(prompt, 0, len_before);
    between_wcd = ft_substr(prompt, len_before + 1, len_between);
    after_wcd = ft_substr(prompt, ft_strlen(prompt) - len_after, len_after);
    if (getcwd(cmd->tab, sizeof(cmd->tab)) != NULL)
    {
        directory = opendir(cmd->tab);
        if (directory) 
        {
            while ((direc = readdir(directory)) != NULL) 
            {
                if (strcmp(direc->d_name, ".") != 0 
                    && strcmp(direc->d_name, "..") != 0
                    && between_match(direc->d_name, between_wcd)
                    && !strncmp(direc->d_name, before_wcd, ft_strlen(before_wcd))
                    && end_match(direc->d_name, after_wcd))
                    nb_files++;
            }
            closedir(directory);
        } 
    }
    if(nb_files)
    {
        new_prompt = add_toprompt6(nb_files, cmd, directory, direc, between_wcd, before_wcd);
        prompt = tabtochar(new_prompt);
    }
    return(prompt);
}