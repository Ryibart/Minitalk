/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk_utilities.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 19:32:20 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/16 13:37:41 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

size_t	minitalk_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

void	minitalk_print(char *message, int flag, char *color, char *format)
{
	if (flag == ERROR)
	{
		if (write(2, RED, minitalk_strlen(RED)) == -1
			|| write(2, BOLD, minitalk_strlen(BOLD)) == -1
			|| write(2, "Error: ", 7) == -1
			|| write(2, message, minitalk_strlen(message)) == -1
			|| write(2, "\n", 1) == -1
			|| write(2, RESET, minitalk_strlen(RESET)) == -1)
			minitalk_print("Write failed in minitalk_print", ERROR, NULL, NULL);
		exit(EXIT_FAILURE);
	}
	else if (flag == MESSAGE)
	{
		if (write(1, color, minitalk_strlen(color)) == -1)
			minitalk_print("Write failed in minitalk_print", ERROR, NULL, NULL);
		if (format)
			if (write(1, format, minitalk_strlen(format)) == -1)
				minitalk_print("Write failed in minitalk_print",
					ERROR, NULL, NULL);
		if (write(1, message, minitalk_strlen(message)) == -1
			|| write(1, "\n", 1) == -1
			|| write(1, RESET, minitalk_strlen(RESET)) == -1)
			minitalk_print("Write failed in minitalk_print", ERROR, NULL, NULL);
	}
}

void	signal_config(void *handler)
{
	struct sigaction	sa_newsignal;

	sa_newsignal.sa_sigaction = handler;
	sa_newsignal.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_newsignal.sa_mask);
	if (sigaction(SIGUSR1, &sa_newsignal, NULL) == -1)
		minitalk_print("SIGUSR1 behavior didn't change.", ERROR, NULL, NULL);
	if (sigaction(SIGUSR2, &sa_newsignal, NULL) == -1)
		minitalk_print("SIGUSR2 behavior didn't change.", ERROR, NULL, NULL);
}
