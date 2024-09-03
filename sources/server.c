/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:30:05 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/03 18:20:15 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minitalk.h"
#include <stdio.h>

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
	static int						remaining_bits = sizeof(size_t) * 8;

	(void)ucontext;
	check_message(info, &current_client_pid);
	if (remaining_bits > 0)
	{
		message_length |= (signum == SIGUSR1) << (remaining_bits -1);
		printf("Server: Receiving bit: %d, message_length: %zu, bit_iter: %d, remaining_bits: %d\n", (signum == SIGUSR1), message_length, bit_iter, remaining_bits);
		remaining_bits--;
		if (remaining_bits == 0)
		{
			buffer = (char *)malloc(message_length + 1);
			if (!buffer)
				error_handler("Buffer allocation failed");
			minitalk_memset(buffer, 0, message_length + 1);
			buffer_index = 0;
			printf("Server: Allocated buffer of size: %zu\n", message_length);
		}
	}
	else
	{
		buffer[buffer_index] |= (signum == SIGUSR1) << bit_iter;
		if (bit_iter == 0)
		{
			buffer_index++;
			bit_iter = 7;
		}
		else
			bit_iter--;

		if (buffer_index == message_length)
		{
			buffer[buffer_index] = '\0';
			write(1, buffer, message_length);
			write(1, "\n", 1);
			free(buffer);
			buffer = NULL;
			message_length = 0;
			remaining_bits = sizeof(size_t) * 8;
			if (kill(current_client_pid, SIGUSR1) == -1)
				error_handler("Signal sending failed.");
			usleep(200);
			g_is_processing_message = false;
			current_client_pid = 0;
		}
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
