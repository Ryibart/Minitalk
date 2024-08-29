/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:59:26 by rtammi            #+#    #+#             */
/*   Updated: 2024/08/29 17:22:33 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minitalk.h"

static volatile sig_atomic_t	g_message_status = false;

void	args_check(int argc, char **argv)
{
	int	i;

	i = 0;
	if (argc != 3)
		error_handler("Invalid number of args, please only insert PID and msg");
	while (argv[1][i])
	{
		if (argv[1][i] < '0' || argv[1][i] > '9')
			error_handler("Invalid PID");
		i++;
	}
	if (*argv[2] == 0)
		error_handler("Invalid message (empty)");
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
			usleep(2000);
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
	if (signum == SIGUSR1 && g_message_status == MESSAGE_RECEIVED_FALSE)
	{
		write(1, "Affirmative, message has been delivered!\n", 41);
		g_message_status = MESSAGE_RECEIVED_TRUE;
	}
	if (signum == SIGUSR2 && g_message_status == MESSAGE_FAILED_FALSE)
	{
		error_handler("Failed to send message (server not receiving)");
		g_message_status = MESSAGE_FAILED_TRUE;
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

	args_check(argc, argv);
	server_pid = minitalk_atoi(argv[1]);
	signal_config();
	send_message(server_pid, argv[2]);
	while ((g_message_status == MESSAGE_RECEIVED_FALSE)
		|| (g_message_status == MESSAGE_FAILED_FALSE))
		pause();
	return (0);
}
