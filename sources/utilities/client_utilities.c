/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_utilities.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:31:12 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/06 13:27:39 by rtammi           ###   ########.fr       */
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
