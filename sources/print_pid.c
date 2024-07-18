/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_pid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 13:19:15 by rtammi            #+#    #+#             */
/*   Updated: 2024/07/11 15:27:53 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minitalk.h"

static int	minitalk_digit_count(int n)
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
	len = minitalk_digit_count(n);
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
