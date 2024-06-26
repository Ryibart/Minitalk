/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:30:05 by rtammi            #+#    #+#             */
/*   Updated: 2024/06/26 20:51:01 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minitalk.h"

void sigusr_handler(int signum, siginfo_t *info, void *context)
{
	static char	c;
	static int	bit_iter = -1;
	
	(void)context;
	if (bit_iter < 0)
		bit_iter = 7;
	if (signum == SIGUSR1)
		c |= (1 << bit_iter);
	bit_iter--;
	if (bit_iter < 0 && c)
	{
		write(1, &c, 1);
		c = 0;
		if (kill(info->si_pid, SIGUSR1) == -1)
			error_handler("Signal sending failed.");
		return ;
	}
	if (kill(info->si_pid, SIGUSR2) == -1)
		error_handler("Signal sending failed.");
}

void	signal_config(void)
{
	struct	sigaction	sa_newsignal;
	
	sa_newsignal.sa_handler = &sigusr_handler; // sa_handler or sa_sigaction?
	sa_newsignal.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_newsignal.sa_mask); // is this necessary?
	if (sigaction(SIGUSR1, &sa_newsignal, NULL) == -1)
		error_handler("SIGSUR1 behavior didn't change.");
	if (sigaction(SIGUSR2, &sa_newsignal, NULL) == -1)
		error_handler("SIGSUR2 behavior didn't change.");
}

int main(void)
{
	__pid_t	pid;

	pid = getpid();
	write(1, "Server PID: ", 12);
	minitalk_print_pid(pid);
	write(1, "\n", 1);
	while (1)
		signal_config();
	return (0);	
	}