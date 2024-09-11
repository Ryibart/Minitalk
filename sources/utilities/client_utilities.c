/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_utilities.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:31:12 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/11 20:16:55 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	args_check(int argc, char **argv)
{
	int	i;

	i = 0;
	if (argc != 3)
		error_handler("Invalid number of args, \
Usage: ./client <server_pid> <message>");
	while (argv[1][i])
	{
		if (argv[1][i] < '0' || argv[1][i] > '9')
			error_handler("Invalid PID");
		i++;
	}
}

int	minitalk_atoi(const char *str)
{
	long	result;
	long	temp;

	result = 0;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	while (*str >= '0' && *str <= '9')
	{
		temp = result;
		result = (result * 10) + (*str++ - '0');
		if (temp > result)
			error_handler("Invalid PID (overflow)");
	}
	return ((int)result);
}

void	send_char(__pid_t server_pid, unsigned char c, volatile sig_atomic_t *server_is_open)
{
	int	bits;
	int	retries;
	int	timeout;

	bits = 8;
	if (DEBUG == YES)
		printf("Sending '%c'\n", c);
	while (bits--)
	{
		retries = MAX_RETRY;
		while (retries > 0)
		{
			if (c & 0b10000000)
				send_signal(server_pid, SIGUSR1, 1, CLIENT);
			else
				send_signal(server_pid, SIGUSR2, 1, CLIENT);
			timeout = TIMEOUT_COUNT;
			while (*server_is_open == false && timeout > 0)
			{
				usleep(RETRY_SLEEP);
				timeout--;
				if (*server_is_open == true)
					break ;
				if (timeout == 0)
				{
					if (--retries == 0)
						error_handler("Timeout occured, server has failed");
					if (DEBUG == YES)
						printf("Retrying bit '%d' for character '%c'\n", bits, c);
				}
			}
			if (*server_is_open == true)
				break ;
		}
		c <<= 1;
		*server_is_open = false;
	}
}
