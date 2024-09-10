/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:30:05 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/10 16:41:15 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile sig_atomic_t	g_is_processing_message = false;

int	check_message(siginfo_t *info, __pid_t *current_client_pid)
{
	if (SERVER_DEBUG == YES)
		printf("Checking message\n");
	if (g_is_processing_message == true && *current_client_pid != info->si_pid)
	{
		if (SERVER_DEBUG == YES)
			printf("Server sent busy to %u\n", info->si_pid);
		send_signal(info->si_pid, SIGUSR2, MED_T, SERVER);
		return (-1);
	}
	else if (g_is_processing_message == false)
	{
		*current_client_pid = info->si_pid;
		g_is_processing_message = true;
		if (SERVER_DEBUG == YES)
			printf("SERVER SENT OPEN TO %u (Processing is %d)\n", info->si_pid, g_is_processing_message);
		send_signal(*current_client_pid, SIGUSR1, MED_T, SERVER);
		return (-1);
	}
	return (1);
}

int	process_message(t_message *msg, __pid_t current_client_pid)
{
	if (SERVER_DEBUG == YES)
		printf("Processing message\n");
	append_to_buffer(msg);
	if (msg->current_char == '\0')
	{
		if (write(1, msg->buffer, msg->buffer_index) < 0
			|| write(1, "\n", 1) < 0)
		{
			reset_message(msg);
			error_handler("Printing message failed");
		}
		reset_message(msg);
		send_signal(current_client_pid, SIGUSR1, SHORT_T, SERVER);
		return (true);
	}
	return (false);
}

int	print_message(int signum, t_message *msg, __pid_t current_client_pid)
{
	int	result;

	result = false;
	if (SERVER_DEBUG == YES)
		printf("In print_message\n");
	if (signum == SIGUSR1)
		msg->current_char |= (1 << msg->bit_iter);
	msg->bit_iter--;
	if (msg->bit_iter < 0)
	{
		if (msg->current_char >= 0x80)
		{
			msg->byte_count++;
			if (msg->byte_count)
		}
		msg->bit_iter = 7;
		result = process_message(msg, current_client_pid);
		msg->current_char = 0;
	}
	if (result == false)
		send_signal(current_client_pid, SIGUSR1, SHORT_T, SERVER); // HERE DIFFERENT ERROR MESSAGE
	return (result);
}

void	message_handler(int signum, siginfo_t *info, void *context)
{
	static __pid_t		current_client_pid = 0;
	static t_message	msg = {NULL, 0, 0, 7, 0};

	(void)context;
	if (SERVER_DEBUG == YES)
		printf("Received %d from  %d\n", signum, info->si_pid);
	if (check_message(info, &current_client_pid) == -1)
		return ;
	if (g_is_processing_message == true && current_client_pid == info->si_pid)
	{
		if (print_message(signum, &msg, current_client_pid) == true)
		{
			send_signal(current_client_pid, SIGUSR1, LONG_T, SERVER);
			current_client_pid = 0;
			g_is_processing_message = false;
			if (SERVER_DEBUG == YES)
				printf("SERVER SENT DONE TO %u (Processing is %d)\n", info->si_pid, g_is_processing_message);
		}
	}
}

int	main(void)
{
	__pid_t	pid;
	int		i = 0;

	pid = getpid();
	if (pid < 0)
		error_handler("Failed to get PID");
	minitalk_print_pid(pid);
	write(1, " is server PID\n", 16);
	if (SERVER_DEBUG == YES)
		printf("Setting handler\n");
	signal_config(message_handler);
	while (1)
	{
		if (SERVER_DEBUG == YES)
			printf("Pause nro %u\n", i++);
		pause();
	}
	return (0);
}
