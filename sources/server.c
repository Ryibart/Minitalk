/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:30:05 by rtammi            #+#    #+#             */
/*   Updated: 2024/08/29 17:24:02 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minitalk.h"

static volatile sig_atomic_t	g_is_processing_message = false;

void	check_message(siginfo_t *info, volatile __pid_t *current_client_pid)
{
	if (g_is_processing_message == true && *current_client_pid != info->si_pid)
	{
		if (kill(info->si_pid, SIGUSR2) == -1)
			error_handler("Signal sending failed.");
		error_handler("Simultaneous client conflict, server aborted");
	}
	if (g_is_processing_message == false)
	{
		*current_client_pid = info->si_pid;
		g_is_processing_message = true;
	}
}

void	sigusr_handler(int signum, siginfo_t *info, void *ucontext)
{
	static char						c = 0;
	static int						bit_iter = 7;
	static volatile __pid_t			current_client_pid = 0;

	(void)ucontext;
	check_message(info, &current_client_pid);
	if (signum == SIGUSR1)
		c |= (1 << bit_iter);
	bit_iter--;
	if (bit_iter < 0)
	{
		write(1, &c, 1);
		bit_iter = 7;
		if (kill(info->si_pid, SIGUSR2) == -1)
			error_handler("Signal sending failed.");
		if (c == '\0')
		{
			g_is_processing_message = false;
			current_client_pid = 0;
		}
		c = 0;
	}
	else if (kill(info->si_pid, SIGUSR1) == -1)
		error_handler("Signal sending failed.");
}

void	signal_config(void)
{
	struct sigaction	sa_newsignal;

	sa_newsignal.sa_sigaction = &sigusr_handler;
	sa_newsignal.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_newsignal.sa_mask);
	if (sigaction(SIGUSR1, &sa_newsignal, NULL) == -1)
		error_handler("SIGUSR1 behavior didn't change.");
	if (sigaction(SIGUSR2, &sa_newsignal, NULL) == -1)
		error_handler("SIGUSR2 behavior didn't change.");
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
