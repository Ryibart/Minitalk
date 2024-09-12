/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk_utilities.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 19:32:20 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/12 18:44:19 by rtammi           ###   ########.fr       */
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

void	retry_message(char *message)
{
	write(1, MOVE_CURSOR_UP, 4);
	write(1, CLEAR_LINE, 3);
	minitalk_print(message, MESSAGE, YELLOW, BLINK);
}

void	minitalk_print(char *message, int flag, char *color, char *format)
{
	if (flag == ERROR)
	{
		write(2, RED, minitalk_strlen(RED));
		if (format)
			write(2, format, minitalk_strlen(format));
		write(2, "Error: ", 7);
		write(2, message, minitalk_strlen(message));
		write(2, "\n", 1);
		write(2, RESET, minitalk_strlen(RESET));
		exit(EXIT_FAILURE);
	}
	else if (flag == MESSAGE)
	{
		write(1, color, minitalk_strlen(color));
		if (format)
			write(1, format, minitalk_strlen(format));
		write(1, message, minitalk_strlen(message));
		write(1, "\n", 1);
		write(1, RESET, minitalk_strlen(RESET));
	}
}

void	send_signal(__pid_t pid, int signal, int sleep_time, int sender)
{
	if (CLIENT_DEBUG == ON || SERVER_DEBUG == ON)
		printf("Sent %u to %u\n", signal, pid);
	if (sender == SERVER && sleep_time > 0)
		usleep(sleep_time);
	if (kill(pid, signal) == -1)
		minitalk_print("Signal sending failed (invalid recipient)", ERROR, NULL, BOLD);
	if (sender == CLIENT && sleep_time > 0)
		usleep(sleep_time);
}

void	signal_config(void *handler)
{
	struct sigaction	sa_newsignal;

	sa_newsignal.sa_sigaction = handler;
	sa_newsignal.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_newsignal.sa_mask);
	if (sigaction(SIGUSR1, &sa_newsignal, NULL) == -1)
		minitalk_print("SIGUSR1 behavior didn't change.", ERROR, NULL, BOLD);
	if (sigaction(SIGUSR2, &sa_newsignal, NULL) == -1)
		minitalk_print("SIGUSR2 behavior didn't change.", ERROR, NULL, BOLD);
	if (CLIENT_DEBUG == ON || SERVER_DEBUG == ON)
		printf("Signal_config done\n");
}
