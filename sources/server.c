/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:30:05 by rtammi            #+#    #+#             */
/*   Updated: 2024/08/30 15:06:30 by rtammi           ###   ########.fr       */
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
	static char						*buffer = NULL;
	static size_t					buffer_index = 0;
	static size_t					message_length = 0;
	static int						bit_iter = 7;
	static volatile __pid_t			current_client_pid = 0;
	static int						receiving_length = 1;

	(void)ucontext;
	check_message(info, &current_client_pid);
	if (receiving_length)
	{
		message_length |= (signum == SIGUSR1) << bit_iter;
		if (bit_iter == 0)
		{
			receiving_length = 0;
			bit_iter = 7;
			buffer = (char *)malloc(message_length + 1);
			if (!buffer)
				error_handler("Buffer allocation failed");
			buffer_index = 0;
		}
		else
			bit_iter--;
	}
	else
	{
		buffer[buffer_index] |= (signum == SIGUSR1) << bit_iter;
		if (bit_iter == 0)
		{
			if (buffer[buffer_index] == '\0')
			{
				write(1, buffer, message_length);
				write(1, "\n", 1);
				free(buffer);
				buffer = NULL;
				message_length = 0;
				receiving_length = 1;
				if (kill(current_client_pid, SIGUSR1) == -1)
					error_handler("Signal sending failed.");
				usleep(100);
				g_is_processing_message = false;
				current_client_pid = 0;
			}
			else
				buffer_index++;
			bit_iter = 7;
		}
		else
			bit_iter--;
	}
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
