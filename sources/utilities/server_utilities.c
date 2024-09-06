/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utilities.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 13:19:15 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/06 20:05:23 by rtammi           ###   ########.fr       */
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
		error_handler("PID printing failed");
}

void	reset_message(t_message *msg)
{
	if (DEBUG == YES)
		printf("Reseting message\n");
	minitalk_meta_free(msg->buffer);
	msg->buffer = NULL;
	msg->buffer_size = 0;
	msg->buffer_index = 0;
	msg->bit_iter = 7;
	msg->current_char = 0;
}

void	append_to_buffer(t_message *msg)
{
	if (DEBUG == YES)
	{
		printf("Appending to buffer\n");
		printf("Before appending:\n");
		printf("buffer_index: %zu\n", msg->buffer_index);
		printf("buffer_size: %zu\n", msg->buffer_size);
		printf("current_char: '%c' (ASCII: %d)\n", msg->current_char, msg->current_char);
	}
	if (msg->buffer_index >= msg->buffer_size)
	{
		if (DEBUG == YES)
			printf("Resizing buffer...\n");
		if (msg->buffer_size < 2)
			msg->buffer_size = 8;
		msg->buffer_size += msg->buffer_size * 2;
		if (DEBUG == YES)
			printf("New buffer_size: %zu\n", msg->buffer_size);
		msg->buffer = minitalk_realloc(msg->buffer, msg->buffer_size);
		if (msg->buffer == NULL)
			error_handler("Buffer allocation failed");
	}

	msg->buffer[msg->buffer_index++] = msg->current_char;
	if (DEBUG == YES)
	{
		printf("After appending:\n");
		printf("buffer_index: %zu\n", msg->buffer_index);
		printf("Buffer now contains: \"%s\"\n", msg->buffer);
	}
}
