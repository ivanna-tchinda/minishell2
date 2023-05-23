#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include <termios.h>
#include <termcap.h>
#include <curses.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>

typedef struct t_token{
    char token;
    char *type;
}               s_token;

typedef struct t_info{
    char *tab;
    char *type;
}               s_info;

typedef struct t_cmd{
    s_info *cmd;
}               s_cmd;


void add_cmd(s_cmd *prompt, s_token *token);
int error_characters(char *lineav);
int ft_parsing(s_cmd *prompt, s_token *token, char *line);
int ft_nbtokens(s_token *token);
char *substr_cmd(s_token *token, int start, int end);
int tab_of_cmd(s_cmd *prompt, s_token *token);
void attribute_types(s_token *token, char *line);
void free_prompt(s_cmd *prompt);
int parse_command(s_cmd *prompt, int len_cmd);


//UTILS
void free_token(s_token *token);
void free_tab(char **tabl);

#endif