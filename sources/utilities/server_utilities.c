/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utilities.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 13:19:15 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/15 13:58:45 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	minitalk_print_pid(int pid)
{
	char	buffer[12];
	int		i;

	i = 11;
	buffer[i--] = '\0';
	if (pid == 0)
		buffer[i--] = '0';
	while (pid > 0)
	{
		buffer[i--] = (pid % 10) + '0';
		pid /= 10;
	}
	if (write(1, &buffer[i + 1], 11 - i) == -1)
		minitalk_print("PID printing failed", ERROR, NULL, BOLD);
}

void	reset_message(t_message *msg)
{
	minitalk_meta_free(msg->buffer);
	msg->buffer = NULL;
	msg->buffer_size = 0;
	msg->buffer_index = 0;
	msg->bit_iter = 7;
	msg->current_char = 0;
}

int	verify_message(siginfo_t *info, __pid_t *current_client_pid,
					volatile sig_atomic_t *processing_message)
{
	if (*processing_message == true && *current_client_pid != info->si_pid)
	{
		kill(info->si_pid, SIGUSR2);
		return (-1);
	}
	else if (*processing_message == false)
	{
		*current_client_pid = info->si_pid;
		*processing_message = true;
		usleep(1000);
		kill(*current_client_pid, SIGUSR1);
		return (-1);
	}
	return (1);
}
