/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:31:12 by rtammi            #+#    #+#             */
/*   Updated: 2024/06/26 20:51:01 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minitalk.h"

// FIXTHIS: EVERYTHING BELOW THIS LINE
// minitalk_itoa should handle PID_MAX error and negative numbers?
// consider making strtol instead of atoi?

void	error_handler(char *error_message)
{
	write(2, "Error: ", 7);
	write(2, error_message, ft_strlen(error_message));
	write(2, "\n", 1);
	exit(1);
}

int	minitalk_atoi(const char *str)
{
	long	result;
	int		sign;
	long	temp;

	result = 0;
	sign = 1;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '+' || *str == '-')
		if (*str++ == '-')
			sign = -1;
	while (*str >= '0' && *str <= '9')
	{
		temp = result;
		result = (result * 10) + (*str++ - '0');
		if (temp > result && sign == 1)
			return (-1);
		else if (temp > result && sign == -1)
			return (0);
	}
	return ((int)result * sign);
}



int	minitalk_print_pid(int long nbr)
{
	char	*str;
	int		len;

	if (nbr > INT_MAX || nbr < INT_MIN)
		return (-1);
	str = minitalk_itoa(nbr);
	if (!str)
	{
		free(str);
		return (-1);
	}
	len = 0;
	while (str[len])
	{
		if (write(1, &str[len], 1) == -1)
		{
			free(str);
			return (-1);
		}
		len++;
	}
	free(str);
	return (0);
}

static int	ft_absolute_value(int n)
{
	if (n == -2147483648)
		return (-(n + 1) + 1);
	if (n < 0)
		return (-n);
	return (n);
}

static int	ft_digit_count(int n)
{
	int	count;
	int	temp;

	count = 1;
	temp = n;
	if (n < 0)
		count = 2;
	while (temp / 10 != 0)
	{
		temp /= 10;
		count++;
	}
	return (count);
}


char	*minitalk_itoa(int n)
{
	int				len;
	unsigned int	abs_n;
	char			*str;

	len = ft_digit_count(n);
	abs_n = ft_absolute_value(n);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (0);
	str[len] = '\0';
	len--;
	while (len >= 0)
	{
		str[len] = (abs_n % 10) + '0';
		abs_n /= 10;
		len--;
	}
	if (n < 0)
		str[0] = '-';
	return (str);
}

