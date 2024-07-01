/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:31:12 by rtammi            #+#    #+#             */
/*   Updated: 2024/07/01 17:57:05 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minitalk.h"

// FIXTHIS: EVERYTHING BELOW THIS LINE
// minitalk_itoa should handle PID_MAX error and negative numbers?
// consider making strtol instead of atoi?

static size_t	minitalk_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void	error_handler(char *error_message)
{
	write(2, "Error: ", 7);
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
			error_handler("Invalid PID, minitalk_atoi overflow");
	}
	return ((int)result);
}



void	minitalk_print_pid(int long nbr)
{
	char	*str;

	str = pid_itoa(nbr);
	if (!str)
	{
		free(str);
		error_handler("PID printing failed");
	}
	if (write(1, str, minitalk_strlen(str)) == -1)
	{
		free(str);
		error_handler("PID printing failed");
	}
	free(str);
}

static int	ft_digit_count(int n)
{
	int	count;
	int	temp;

	count = 1;
	temp = n;
	while (temp / 10 != 0)
	{
		temp /= 10;
		count++;
	}
	return (count);
}


char	*pid_itoa(int n)
{
	int				len;
	unsigned int	nbr;
	char			*str;

	nbr = n;
	len = ft_digit_count(n);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	len--;
	while (len >= 0)
	{
		str[len] = (nbr % 10) + '0';
		nbr /= 10;
		len--;
	}
	return (str);
}
