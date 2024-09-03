
#include "minishell.h"

//checks if every env_var or PATH have been deleted
void env_checker(char **envp, t_sh_data **sh)
{
    int path_found;

    path_found = 0;
    if ( envp == NULL || *envp == NULL)
    {
        free(*sh);
        perror("No ENV found, don't delete ENVs!");
        exit(EXIT_SUCCESS);
    }
    while (*envp != NULL)
    {
        if (ft_strncmp(*envp, "PATH=", ft_strlen("PATH=")) == 0)
        {
            path_found = 1;
            break;
        }
        envp++;
    }
    if (path_found == 0)
    {
        free(*sh);
        perror("No PATH found, don't delete the PATH env!");
        exit(EXIT_SUCCESS);
    }
}

// exits program if argc is not 1
void	argc_checker(int argc, t_sh_data **sh)
{
    if (argc != 1)
    {
        free(*sh);
        *sh = NULL;
        printf("Invalid argument count. Usage: ./minishell\n");
        exit(EXIT_SUCCESS);
    }

}

int main(int argc, char **argv, char **envp)
{
    t_sh_data *sh;

    (void)argv;
    sh = (t_sh_data *)malloc(sizeof(t_sh_data));
    if (sh == NULL)
    {
        perror("main");
        exit(1);
    }
    argc_checker(argc, &sh);
    env_checker(envp, &sh);
    shell_init(&sh, envp);
    shell_loop(&sh, -1, 0);
    //to be included in a function, probably:
    free_env_list(sh->env_header);
    free_matrix(sh->env);
    //free(sh->last_exit_status); 
    free(sh->prev_line);
    free(sh); //malloced in main
    return 0;
}
