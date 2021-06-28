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
*/

void    ctrl_c(int sig)
{
    char    *cmd;

    cmd = ft_substr(rl_line_buffer, 0, str_len(rl_line_buffer) - 2);
    if (sig == SIGINT) // ctrl-C
    {
        // write(1, "\nminishell ", str_len("minishell "));
        // printf("\n");
        // printf("---------------------------------------minishett |%s|", cmd);
        printf("minishett ");
        rl_replace_line("", 0);
        rl_redisplay();
        // write(1, "\n", 1);
        // write(0, "B", 1);
        // rl_free_line_state ();
        // rl_cleanup_after_signal ();
    }
    // free(rl_line_buffer);
}

int     main(int argc, char **argv, char **envp)
{
    t_shell     *shell;
    char        *cmd;
    char        t[1];
    int         i=0;

    signal(SIGINT,  ctrl_c);
    cmd = NULL;
    shell = malloc(sizeof(t_shell));
    init_env(envp, shell);
    shell->history[0] = NULL;
    // do_execve(shell, "cat", ft_split("cat", ' '));
    while (1)
    {
        // write(1, "minishell# ", str_len("minishell# "));
        cmd = readline("minishell ");
        if (cmd == 0) // Ctrl-D ==> exit the shell
        {
            // write(1, "\n", 1);
            exit(0);
        }
        // printf("cmd %s\n", cmd);
        shell->history[tab_size(shell->history) + 1] = NULL;
        shell->history[tab_size(shell->history)] = ft_strdup(cmd);
        if (cmd[0] == 'h')
            print_history(shell);
        if (cmd[0] == 'o' && cmd[1] == 'k')
            export("TOTO", "", shell, 1);
        if (cmd[0] == 'o' && cmd[1] == 'l')
            export("USER", "", shell, 1);
        if (cmd[0] == 'o' && cmd[1] == 'm')
            export("TOTO", NULL, shell, 1);
        if (cmd[0] == 'e' && cmd[1] == 'n' && cmd[2] == 'v')
            env(1, shell);
        if (cmd[0] == 'u' && cmd[1] == 'n' && cmd[2] == 's' && cmd[3] == 'e' && cmd[4] == 't')
            unset(ft_substr(cmd, 6, str_len(cmd) - 6), shell);
        if (cmd[0] == 'p')
        {
            // int fd = open("sans_erreur", O_CREAT | O_TRUNC | O_WRONLY, 0600);
            do_execve(shell, "echo", ft_split("echo -n ttt", ' '), 1); // execute minishell in minishell
        }
        if (cmd[0] == 'i')
        {
            // int fd = open("avec_erreur", O_CREAT | O_TRUNC | O_WRONLY, 0600);
            do_execve(shell, "dfghdfghdfh", ft_split("dfghdfghdfh", ' '), 1); // execute minishell in minishell
        }
        if (cmd[0] == 'k')
        {
            do_execve(shell, "ls", ft_split("ls -lo3", ' '), 1);
        }
    }
    // rl_replace_line();
}
