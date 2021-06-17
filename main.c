#include "minishell.h"
#include <signal.h>

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

void ctrl_c(int sig)
{
    if (sig == SIGINT) // ctrl-C
    {
        write(0, "ctrl-c", str_len("ctrl-c"));
        // write(0, "\n", 1);
        // write(0, "B", 1);
    }
}

int     main(int argc, char **argv, char **envp)
{
    t_shell     *shell;
    char        *cmd;
    char t[1]; //
    int i=0; //

    signal(SIGINT,  ctrl_c);
    cmd = NULL;
    shell = malloc(sizeof(t_shell));
    init_env(envp, shell);
    while (1)
    {
        write(1, "minishell# ", str_len("minishell# "));
        if (get_next_line(&cmd) == 0) // Ctrl-D ==> exit the shell
        {
            write(1, "\n", 1);
            exit(0);
        }
        if (cmd[0] == 'o' && cmd[1] == 'k')
            export("TOTO", "", shell, 1);
        if (cmd[0] == 'o' && cmd[1] == 'l')
            export("USER", "", shell, 1);
        if (cmd[0] == 'o' && cmd[1] == 'm')
            export("TOTO", NULL, shell, 1);
        if (cmd[0] == 'e' && cmd[1] == 'n' && cmd[2] == 'v')
            env(1, shell);
            // do_execve(shell, "./minishell", ft_split("./minishell ", ' ')); // execute minishell in minishell
    }
}
