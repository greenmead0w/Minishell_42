#include "../minishell.h"

/*
** using these functions to update the terminal inmediately and 
** for not waiting until then next iteration of the loop
** "If handle_sigint is called during get_input, it interrupts readline, 
**  and the loop resumes, causing get_input to be called again"
*/
void handle_sigint(int sig) 
{
    //(void)sig;
	write(1, "\n", 1);
	//printf("sig is: %d\n", sig);
	if (sig != -7)
	{
		rl_on_new_line(); //prepares terminal for new input [UNPROTECTED]
		rl_replace_line("", 0); //replaces current line with an empty string
		rl_redisplay(); //refreshes the terminal display, showing the empty string{{{
	}
}

void	def_signals(void)
{
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	//printf("in def_signals\n");
}

void	sig_blocking_handler(int sig)
{
	if (sig == SIGINT)
		handle_sigint(-7);
	else if (sig == SIGQUIT)
		write(2, "Quit (core dumped)\n", 19);
}

/*
**	blocking command: halts execution of commands until condition is met.
**	examples:
**		1. cat (without arguments): waits for user input
**		2.	sleep: pauses execution for a specified amount of time
*/
void	blocking_cmd_sig(void)
{
	signal(SIGINT, sig_blocking_handler);
	signal(SIGQUIT, sig_blocking_handler);
}

void heredoc_sigint(int sig)
{
    (void)sig;
    signal_received = 1;
    write(1, "\n", 1);
}

void e_pipe_sig_handler(int sig)
{
	write(1, "\n", 1);
	write(1, "$ ", 2);
	(void)sig;
	signal_received = 3;
}
void ending_pipe_sig(void)
{
	signal(SIGINT, e_pipe_sig_handler);
}

