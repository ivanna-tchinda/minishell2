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

extern char **envir;

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
    int index;
}               s_cmd;

//PARSING
int error_characters(char *lineav);
int ft_parsing(s_cmd *prompt, s_token *token, char *line);
int ft_nbtokens(s_token *token);
void free_prompt(s_cmd *prompt);
int parse_command(s_cmd *prompt, int len_cmd);
int check_pipe(s_info *cmd, int len_cmd);
int check_redif(s_info *cmd, int len_cmd);
int parentheses(char *cmd);

//TAB_OF_CMD
void add_cmd(s_cmd *prompt, s_token *token);
char *substr_cmd(s_token *token, int start, int end);
int tab_of_cmd(s_cmd *prompt, s_token *token);
void attribute_types(s_token *token, char *line);
int tab_of_cmd(s_cmd *prompt, s_token *token);
void add_totab(s_info *cmd, s_token *token, int *i);
int only_cmd(s_cmd *prompt);
void free_pmt(s_cmd *prompt);

//BUILTIN
int is_builtin(char *cmd);
int built_intab(char *tab);
int check_command(char *tab, char *bltn);
int around_bltn(char *tab, char *bltn, int j);
void exec_bltn(char *cmd);

//ECHO
void echo_bltn(char *line);
int is_optn(char *line);
int after_echo(char *line);
int after_optn(char *line);
void env_bltn(void);

//PWD
int	pwd_bltn();
char *recup_env(const char *env_param);
ssize_t find_good_env(const char *env_param) ;

//ENV
void set_envir(char *envp[]);

//EXPORT
void export_bltn(char *new_var);
char *get_var(char *new_var);
void set_envir2(char *envp[]);

//EXPAND
void expand_cmd(s_cmd *prompt);
char  *expand_prompt(char *prompt);
char *quoted_prompt(char *new_prompt, char *prompt, int *i, int quote_type);
char *before_equal(char *envp);
char *findenv(char *varenv);
char *dollar_sign(char *new_prompt, char *prompt, int *i);
char *dquoted_prompt(char *new_prompt, char *prompt, int *i);
char *dollar_prompt(char *new_prompt, char *prompt, int *i);

//EXPAND STATUS
void expand_status(s_cmd *prompt);
char *dollar_status(s_cmd *cmd, char *prompt);


//PIPEX
int ft_pipex(s_cmd *prompt);
void allsignals();
void sigquit(int signal);
void sigint(int signal);
int	ft_execve(char *path, char **args);
void	close_and_wait(int *prev_pipe, int num_commands);
void	process_father(int i, int *prev_pipe, int *next_pipe, int num_commands);
void	process_child(int i, int *prev_pipe, int *next_pipe, int num_commands);
void not_last_command(int i, int num_commands, int *next_pipe);
char	*ft_command(char *av);
char	*ft_recup_path(char *command);
char	**ft_recup_envp(void);

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
int	ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strnstr(const char *big, const char *little, size_t len);
char	*ft_strdup(char *src);

#endif