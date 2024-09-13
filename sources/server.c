/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:30:05 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/13 12:35:48 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile sig_atomic_t	g_processing_message = false;

void	append_to_buffer(t_message *msg)
{
	if (msg->buffer_index >= msg->buffer_size)
	{
		if (msg->buffer_size < 2)
			msg->buffer_size = 8;
		msg->buffer_size += msg->buffer_size * 2;
		msg->buffer = minitalk_realloc(msg->buffer, msg->buffer_size);
		if (msg->buffer == NULL)
			minitalk_print("Buffer allocation failed", ERROR, NULL, BOLD);
	}
	msg->buffer[msg->buffer_index++] = msg->current_char;
}

int	process_message(t_message *msg, __pid_t current_client_pid)
{
	append_to_buffer(msg);
	if (msg->current_char == '\0')
	{
		if (write(1, msg->buffer, msg->buffer_index) < 0
			|| write(1, "\n", 1) < 0)
		{
			reset_message(msg);
			minitalk_print("Printing message failed", ERROR, NULL, BOLD);
		}
		reset_message(msg);
		send_signal(current_client_pid, SIGUSR1, SHORT_T, SERVER);
		return (true);
	}
	return (false);
}

int	print_message(int signum, t_message *msg, __pid_t current_client_pid)
{
	int		message_printed;

	message_printed = false;
	if (signum == SIGUSR1)
		msg->current_char |= (1 << msg->bit_iter);
	msg->bit_iter--;
	if (msg->bit_iter < 0)
	{
		msg->bit_iter = 7;
		message_printed = process_message(msg, current_client_pid);
		msg->current_char = 0;
	}
	if (message_printed == false)
		send_signal(current_client_pid, SIGUSR1, SHORT_T, SERVER);
	return (message_printed);
}

void	message_handler(int signum, siginfo_t *info, void *context)
{
	static __pid_t		current_client_pid = 0;
	static t_message	msg = {NULL, 0, 0, 7, 0};

	(void)context;
	if (info->si_pid == getpid())
	{
		reset_message(&msg);
		minitalk_print("Aborting server, confirmation conflict", ERROR, NULL, BOLD);
	}
	if (verify_message(info, &current_client_pid, &g_processing_message) == -1)
		return ;
	if (g_processing_message == true && current_client_pid == info->si_pid)
	{
		if (print_message(signum, &msg, current_client_pid) == true)
		{
			send_signal(current_client_pid, SIGUSR1, LONG_T, SERVER);
			current_client_pid = 0;
			g_processing_message = false;
		}
	}
}

int	main(void)
{
	__pid_t	pid;

	pid = getpid();
	if (pid < 0)
		minitalk_print("Failed to get PID", ERROR, NULL, BOLD);
	minitalk_print_pid(pid);
	write(1, " is server PID\n", 16);
	signal_config(message_handler);
	while (1)
	{
		if (sleep(15) == 0 && g_processing_message == true)
			send_signal(pid, SIGUSR2, LONG_T, SERVER);
	}
	return (0);
}
