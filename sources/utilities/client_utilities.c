/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_utilities.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:31:12 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/16 17:27:51 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	args_check(int argc, char **argv)
{
	int	i;

	i = 0;
	if (argc != 3)
		minitalk_print("Invalid number of args, \
Usage: ./client <server_pid> <message>", ERROR, NULL, NULL);
	while (argv[1][i])
	{
		if (argv[1][i] < '0' || argv[1][i] > '9')
			minitalk_print("Invalid PID", ERROR, NULL, NULL);
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
			minitalk_print("Invalid PID (overflow)", ERROR, NULL, NULL);
	}
	return ((int)result);
}

int	retry_loop(volatile sig_atomic_t *server_is_open, int *retries)
{
	int	timeout;

	timeout = TIMEOUT_COUNT;
	while (*server_is_open == false && timeout > 0)
	{
		usleep(RETRY_SLEEP);
		if (*server_is_open == true)
			break ;
		if (--timeout == 0)
		{
			if (--retries == 0)
				minitalk_print("Timeout occured, server has failed",
					ERROR, NULL, BOLD);
			minitalk_print("Recipient busy, retrying sending bit signal",
				MESSAGE, YELLOW, BOLD_ITALIC);
		}
	}
	if (*server_is_open == true)
		return (true);
	return (false);
}

void	send_char(__pid_t server_pid, unsigned char c,
				volatile sig_atomic_t *server_is_open)
{
	int	bits;
	int	retries;

	bits = 8;
	while (bits--)
	{
		retries = MAX_RETRY;
		while (retries > 0)
		{
			if (c & 0b10000000)
			{
				if (kill(server_pid, SIGUSR1) == -1)
					minitalk_print("Signal sending failed, invalid recipient",
						ERROR, NULL, NULL);
			}
			else if (kill(server_pid, SIGUSR2) == -1)
				minitalk_print("Signal sending failed, invalid recipient",
					ERROR, NULL, NULL);
			sleep(1);
			if (retry_loop(server_is_open, &retries) == true)
				break ;
		}
		c <<= 1;
		*server_is_open = false;
	}
}
