/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk_utilities.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 19:32:20 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/11 20:22:59 by rtammi           ###   ########.fr       */
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

void	error_handler(char *error_message)
{
	write(2, "Error: ", 8);
	write(2, error_message, minitalk_strlen(error_message));
	write(2, "\n", 1);
	exit(EXIT_FAILURE);
}

void	send_signal(__pid_t pid, int signal, int sleep_time, int sender)
{
	if (DEBUG == YES)
		printf("Sent %u to %u\n", signal, pid);
	if (sender == SERVER && sleep_time > 0)
		usleep(sleep_time);
	if (kill(pid, signal) == -1)
		error_handler("Signal sending failed (invalid recipient)");
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
		error_handler("SIGUSR1 behavior didn't change.");
	if (sigaction(SIGUSR2, &sa_newsignal, NULL) == -1)
		error_handler("SIGUSR2 behavior didn't change.");
	if (DEBUG == YES)
		printf("Signal_config done\n");
}

int	char_length(unsigned char c)
{
	if ((c & 0x80) == 0)
		return 1; // 1-byte ASCII
	if ((c & 0xE0) == 0xC0)
		return 2; // 2-byte UTF-8
	if ((c & 0xF0) == 0xE0)
		return 3; // 3-byte UTF-8
	if ((c & 0xF8) == 0xF0)
		return 4; // 4-byte UTF-8
	return (-1); // Invalid UTF-8 sequence
}
