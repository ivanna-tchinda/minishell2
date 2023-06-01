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
#include <sys/wait.h>

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
    char *infile;
    char *outfile;
    int nb_tabs;
    int nb_cmd;
    int exitstatus;
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
int tab_of_cmd(s_cmd *prompt, s_token *token);
void add_totab(s_info *cmd, s_token *token, int *i);
void free_pmt(s_cmd *prompt);
int check_pipe(s_info *cmd, int len_cmd);
int check_redif(s_info *cmd, int len_cmd);

//BUILTIN
int is_builtin(s_cmd *prompt);
int built_intab(char *tab);
int check_command(char *tab, char *bltn);
int around_bltn(char *tab, char *bltn, int j);

//EXPAND
void expand_cmd(s_cmd *prompt, char *envp[]);
char  *expand_prompt(char *prompt, char *envp[]);
char *quoted_prompt(char *new_prompt, char *prompt, int *i, int quote_type);
char *before_equal(char *envp);
char *findenv(char *varenv, char **envp);
char *dollar_sign(char *new_prompt, char *prompt, char *envp[], int *i);
char *dquoted_prompt(char *new_prompt, char *prompt, int *i, char *envp[]);
char *dollar_prompt(char *new_prompt, char *prompt, int *i, char *envp[]);

//EXPAND STATUS
void expand_status(s_cmd *prompt);
char *dollar_status(s_cmd *cmd, char *prompt);


//PIPEX
void pipex(s_cmd *cmd, char *env[]);
void process(s_info *cmd, int *fd, char *env[]);
void execute(s_cmd cmd, char *env[]);
int assign_files(s_cmd *cmd);

//UTILS
void free_token(s_token *token);
void free_tab(char **tabl);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char *s1, char *s2);
size_t	ft_strlen(const char *s);
char	**ft_split(char const *s, char c);
size_t	count_words(char const *s, char c);
void	ft_allocate(char **s2, char const *s, char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char *alloc_one(char c);
char *join_one(char *str, char c);
size_t	len_int(int n);


#endif