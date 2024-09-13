/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk_utilities.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 19:32:20 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/13 10:51:56 by rtammi           ###   ########.fr       */
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
	if (write(1, MOVE_CURSOR_UP, 4) == -1
		|| write(1, CLEAR_LINE, 3) == -1)
		minitalk_print("Write failed in retry_message", ERROR, NULL, NULL);
	minitalk_print(message, MESSAGE, YELLOW, BLINK);
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

void	send_signal(__pid_t pid, int signal, int sleep_time, int sender)
{
	if (sender == SERVER && sleep_time > 0)
		usleep(sleep_time);
	if (kill(pid, signal) == -1)
		minitalk_print("Signal sending failed (invalid recipient)",
			ERROR, NULL, NULL);
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
		minitalk_print("SIGUSR1 behavior didn't change.", ERROR, NULL, NULL);
	if (sigaction(SIGUSR2, &sa_newsignal, NULL) == -1)
		minitalk_print("SIGUSR2 behavior didn't change.", ERROR, NULL, NULL);
}
