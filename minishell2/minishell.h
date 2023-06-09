#ifndef MINISHELL_H
#define MINISHELL_H

#define _GNU_SOURCE
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
#include <dirent.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 25
#endif

extern char **var_envir;

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
    int ret;
    char tab[1024];
}               s_cmd;

int minishell(char *envp[]);

//PARSING
int check_and(s_cmd *prompt, int len_cmd);
int error_characters(char *lineav);
int ft_parsing(s_cmd *prompt, s_token *token, char *line);
int ft_nbtokens(s_token *token);
void free_prompt(s_cmd *prompt);
int parse_command(s_cmd *prompt, int len_cmd, s_token *token);
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
void exec_bltn(char *cmd, s_cmd *prompt);

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
void set_envir(char **envp);

//EXPORT
void export_bltn(char *new_var);
char *get_var(char *new_var);

//UNSET
void unset_bltn(char *cmd);

//CD
void cd_bltn(char *cmd);
int run_cd(char **args, char **envp);

//EXIT
void exit_bltn(char *prompt);
int execute_exit(char **args);
int check_exit_signs(char **args);
int check_correct_exit(char **args);

//EXPAND
void expand_cmd(s_cmd *prompt);
char  *expand_prompt(char *prompt);
char *quoted_prompt(char *new_prompt, char *prompt, int *i, int quote_type);
char *before_equal(char *envp);
char *findenv(char *varenv);
char *dollar_sign(char *new_prompt, char *prompt, int *i);
char *dquoted_prompt(char *new_prompt, char *prompt, int *i);
char *dollar_prompt(char *new_prompt, char *prompt, int *i);

// WILDCARD EXPAND
char *tabtochar(char **tab);
char *wildcard_expand(char *prompt, s_cmd *cmd);
char *wildcard_prompt(char *prompt, s_cmd *cmd);
char *modify_wildcard(char *prompt, s_cmd *cmd);
char *one_asterisk(char *prompt, s_cmd *cmd);
char *allfiles_wcd(s_cmd *cmd);
char *before_wcd(char *prompt, s_cmd *cmd);
char *after_wcd(char *prompt, s_cmd *cmd);
int end_match(char *name, char *ext);
char *two_asterisk(char *prompt, s_cmd *cmd);
char *between_wcd(char *prompt, s_cmd *cmd);
char *aftertwo_wcd(char *prompt, s_cmd *cmd);
int between_match(char *name, char *ext);
char **add_toprompt5(int nb_files, s_cmd *cmd, DIR *directory, struct dirent *direc, char *between_wcd, char *after_wcd);
char **add_toprompt6(int nb_files, s_cmd *cmd, DIR *directory, struct dirent *direc, char *between_wcd, char *before_wcd);
char *beforetwo_wcd(char *prompt, s_cmd *cmd);
char *alltwo_wcd(char *prompt, s_cmd *cmd);

//EXPAND STATUS
void expand_status(s_cmd *prompt);
char *dollar_status(s_cmd *cmd, char *prompt);

//PARENTHESES
void ft_parentheses(s_cmd *prompt, int *i, s_token *token, s_cmd *par_prompt);
int exec_par(char *cmd, s_cmd *prompt, int prevpipe, int *i);
int ft_execpar(s_cmd *prompt, int *i);
void ft_andpar(s_cmd *prompt, int *i, int ret);

//EXEC
int ft_exec(s_cmd *prompt, s_token *token);
void ft_firstcmd(int ret_val, s_cmd *prompt, int *i, int infile);
void pipex_cmd(s_cmd *prompt, int *i, int *prevpipe);
int exec_lastcmd(s_cmd *prompt, int *i, int prevpipe, char *outfile);
void ft_firstredirin(s_cmd *prompt, int *i);
char *after_infile(char *prompt);
void ft_and(s_cmd *prompt, int *i, int ret_value);
char *without_spaces(char *cmd);
void ft_redirout(s_cmd *prompt, int *i);
void outfile_cmd(char *cmd, s_cmd *prompt, int *i, int infile);
void ft_pipe(s_cmd *prompt, int *i);
void ft_redirdoc(char *cmd, s_cmd *prompt, int *i, int infile);
void or_cmd(int ret_val, s_cmd *prompt, int *i);
void ft_heredoc(s_cmd *prompt, int *i);
void ft_heredocpipex(s_cmd *prompt, int *i);
int exec_lastcmddoc(char *cmd, s_cmd *prompt, int prevpipe, char *outfile, int *i);
void ft_heredocpipex2(s_cmd *prompt, int *i, int ret_val);
int exec_lastcmddoc2(char *cmd, s_cmd *prompt, int prevpipe, char *outfile, int *i);
void ft_heredocpipex3(s_cmd *prompt, int *i, int ret_val);
char *without_spaces(char *cmd);
void ft_execve(char *cmd, int prevpipe, s_cmd *prompt, int *i, int outfile);
int exec_cmd(s_cmd *prompt, int *i, int prevpipe, char *outfile);
int exec_builtin(s_cmd *prompt, int *i, int prevpipe, char *outfile);
void set_envir2(char **envp);


//UTILS PIPEX
char	*ft_envir(char **env);
char	*to_find(char **path, char *new_av);
char	*which_envbis(char *av, char **env);
char	*ft_command(char *av);
char	*ft_path(char *av, char **envp);

//SIGNALS
void sigint(int signal);
void allsignals();
void signals_d();

//GNL
char	*get_next_line(int fd);
char	*ft_read(int fd, char *varstat);
char	*fill_afterend(char *buffer);
char	*fill_beforeend(char *buffer);
int	is_endline(char *varstat);
int	ft_lenbuff(char *buffer);

//UTILS
char	*ft_strtrim(char const *s1, char const *set);
int	start_trim(char const *s1, char const *set);
int	end_trim(char const *s1, char const *set, size_t i);
int	is_in_set(char const s1, char const *set);
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
char	*ft_strchr(const char *s, int c);
char	*ft_itoa(int n);

#endif