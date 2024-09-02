/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:59:26 by rtammi            #+#    #+#             */
/*   Updated: 2024/08/30 15:08:25 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minitalk.h"

static volatile sig_atomic_t	g_message_status = false;

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
	if (*argv[2] == 0)
		error_handler("Invalid message (empty)");
}

void	send_length(__pid_t server_pid, size_t length)
{
	unsigned char	byte;
	int				bits;

	while (length > 0)
	{
		byte = (unsigned char)length;
		bits = 8;
		while (bits--)
		{
			if (byte & 0b10000000)
			{
				if (kill(server_pid, SIGUSR1) == -1)
					error_handler("Signal sending failed (invalid server)");
			}
			else if (kill(server_pid, SIGUSR2) == -1)
				error_handler("Signal sending failed (invalid server)");
			usleep(100);
			byte <<= 1;
		}
		length >>= 8;
	}
}

void	send_message(__pid_t server_pid, char *msg)
{
	unsigned char	c;
	int				bits;

	while (*msg)
	{
		c = *msg;
		bits = 8;
		while (bits--)
		{
			if (c & 0b10000000)
			{
				if (kill(server_pid, SIGUSR1) == -1)
					error_handler("Signal sending failed (invalid server)");
			}
			else if (kill(server_pid, SIGUSR2) == -1)
				error_handler("Signal sending failed (invalid server)");
			usleep(100);
			c <<= 1;
		}
		msg++;
	}
	send_nullpointer(server_pid);
}

void	sigusr_recieved(int signum, siginfo_t *info, void *ucontext)
{
	(void)ucontext;
	(void)info;
	if (signum == SIGUSR1 && !g_message_status)
	{
		write(1, "Message received by server.\n", 29);
		g_message_status = true;
	}
}

void	signal_config(void)
{
	struct sigaction	sa_newsignal;

	sa_newsignal.sa_sigaction = &sigusr_recieved;
	sa_newsignal.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_newsignal.sa_mask);
	if (sigaction(SIGUSR1, &sa_newsignal, NULL) == -1)
		error_handler("SIGUSR1 behavior didn't change.");
	if (sigaction(SIGUSR2, &sa_newsignal, NULL) == -1)
		error_handler("SIGUSR2 behavior didn't change.");
}

int	main(int argc, char **argv)
{
	__pid_t	server_pid;
	size_t	message_len;

	args_check(argc, argv);
	server_pid = minitalk_atoi(argv[1]);
	message_len = minitalk_strlen(argv[2]);
	signal_config();
	send_length(server_pid, message_len);
	send_message(server_pid, argv[2]);
	while (g_message_status == false)
		pause();
	return (0);
}
