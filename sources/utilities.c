/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:31:12 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/03 14:37:49 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minitalk.h"

void	send_nullpointer(__pid_t server_pid)
{
	unsigned char	c;
	int				bits;

	c = '\0';
	bits = 8;
	while (bits--)
	{
		if (c & 0b10000000)
			kill(server_pid, SIGUSR1);
		else
			kill(server_pid, SIGUSR2);
		usleep(2000);
		c <<= 1;
	}
}

size_t	minitalk_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void	error_handler(char *error_message)
{
	write(2, "\nError: ", 8);
	write(2, error_message, minitalk_strlen(error_message));
	write(2, "\n", 1);
	exit(1);
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

void	*minitalk_memset(void *b, int c, size_t n)
{
	size_t			i;
	unsigned char	*u_b;
	unsigned char	u_c;

	i = 0;
	u_b = (unsigned char *)b;
	u_c = (unsigned char)c;
	while (i < n)
	{
		u_b[i] = u_c;
		i++;
	}
	return (b);
}
