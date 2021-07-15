#include "minishell.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

/*
    EXEMPLES        FONCTIONS UTILISATIONS                  FONCTIONS PROTOTYPES
    echo    ==>     echo("Bonjour", fd, TRUE, shell);       echo(const char *str, t_fd output, t_bool newline, t_shell  *shell)
    cd      ==>     cd("DOSSIER", shell, fd);               cd(char *path, t_shell  *shell, t_fd fd)
    pwd     ==>     pwd(fd, shell);                         pwd(t_fd    output, t_shell *shell)
    export  ==>     export("TOTO", "bmoulin", shell, fd);   export(const char *name, const char *arg, t_shell *shell, t_fd fd)
    unset   ==>     unset("TOTO", shell);                   unset(const char *name, t_shell *shell)
    env     ==>     env(fd, shell);                         env(t_fd    output, t_shell *shell)
    exit    ==>     exit_cmd();                             exit_cmd(void)

    A SAVOIR
    cd      ==> ne prend en compte que le premier argument (cd minishell toto blabla == cd minishell)
    pwd     ==> ne prend pas en compte les arguments (pwd tata erfer rfe e== pwd) pwd n'as pas d'erreur

    TODO                EXPLICATION                                                 EXEMPLE
    ctrl-c      ==>     bug quand on a une grande commande.                         ggugjgjsygjyfegwyfguwgfugewufuwgeufgwuegfuwgefuw ^C
    Makefile    ==>     rl_replace_line ne marche pas
*/

// void    ctrl_c(int sig)
// {
//     char    *cmd;
//     int     i;

//     cmd = ft_strdup(rl_line_buffer);
//     i = str_len(cmd);
//     if (sig == SIGINT) // ctrl-C
//     {
//         rl_replace_line("", 0);
//         printf("minishell %s", cmd);
//         printf("  \n");
//         rl_redisplay();
//         printf("minishett ");
//     }
//     free(cmd);
// }

// int     main(int argc, char **argv, char **envp)
// {
//     t_shell     *shell;
//     char        *cmd;
//     char        t[1];
//     int         i=0;

//     signal(SIGINT,  ctrl_c);
//     cmd = NULL;
//     shell = malloc(sizeof(t_shell));
//     init_env(envp, shell);
//     shell->history[0] = NULL;
// }
