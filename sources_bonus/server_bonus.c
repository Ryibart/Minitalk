/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:59:41 by rtammi            #+#    #+#             */
/*   Updated: 2024/07/11 15:12:39 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/minitalk_bonus.h"

void	sigusr_handler(int signum, siginfo_t *info, void *ucontext)
{
	static char	c = 0;
	static int	bit_iter = 7;

	(void)ucontext;
	if (signum == SIGUSR1)
		c |= (1 << bit_iter);
	bit_iter--;
	if (bit_iter < 0)
	{
		write(1, &c, 1);
		bit_iter = 7;
		c = 0;
		if (kill(info->si_pid, SIGUSR1) == -1)
			error_handler("Signal sending failed.");
	}
	else if (kill(info->si_pid, SIGUSR2) == -1)
		error_handler("Signal sending failed.");
}

void	signal_config(void)
{
	struct sigaction	sa_newsignal;

	sa_newsignal.sa_sigaction = &sigusr_handler;
	sa_newsignal.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_newsignal.sa_mask);
	if (sigaction(SIGUSR1, &sa_newsignal, NULL) == -1)
		error_handler("SIGSUR1 behavior didn't change.");
	if (sigaction(SIGUSR2, &sa_newsignal, NULL) == -1)
		error_handler("SIGSUR2 behavior didn't change.");
}

int	main(void)
{
	__pid_t	pid;

	pid = getpid();
	if (pid < 0)
		error_handler("Failed to get PID");
	minitalk_print_pid(pid);
	write(1, " is server PID\n", 16);
	signal_config();
	while (1)
		pause();
	return (0);
}
