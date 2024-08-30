#include "./include/rachid.h"




void	handle_sigint(int sig)
{
	(void)sig;
	rl_replace_line("", 0);
	write(STDOUT_FILENO, "\nMiniShell$ ", 13);
}

void	child_sigint(int pid)
{
	(void)pid;
	// write(1, "7baaaaaaaaaas \n", 15);
	g_stop_heredoc = 1;
}

// void	child_sigquit(int pid)
// {
// 	(void)pid;
// 	write(1, "Quit (core dumped)\n", 19);
// }
