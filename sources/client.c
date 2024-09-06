/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:59:26 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/06 13:27:35 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile sig_atomic_t	g_server_is_open = false;

void knock (__pid_t server_pid)
{
	int	retries = 10;
	int	timeout = 3;

	while (g_server_is_open == false && retries > 0)
	{
		if (DEBUG == ON)
			printf("Waiting for server response...\n");
		send_signal(server_pid, SIGUSR2, 0, CLIENT);
		sleep(timeout);
		// timeout += timeout * 2;
		if (g_server_is_open == false)
		{
			retries--;
			if (DEBUG == ON)
				printf("No response. Retrying after %d seconds (%d retries left)\n", timeout, retries);
		}
	}
	if (g_server_is_open == false)
		error_handler("Server not responding. Exiting...");
}

void	send_char(__pid_t server_pid, unsigned char c)
{
	int				bits;

	bits = 7;
	if (DEBUG == ON)
		printf("Sending %c, bits left %u\n", c, bits);
	while (bits >= 0)
	{
		if (c & (1 << bits))
			send_signal(server_pid, SIGUSR1, 0, CLIENT);
		else
			send_signal(server_pid, SIGUSR2, 0, CLIENT);
		bits--;
		pause();
		usleep(100);
	}
}

void	confirmation_sig(int signum, siginfo_t *info, void *ucontext)
{
	(void)ucontext;
	(void)info;
	if (DEBUG == ON)
		printf("In confirmation_sig\n");
	if (signum == SIGUSR1)
	{
		if (write(1, "Message printed by server\n", 27) == -1)
			error_handler("Write failed in confirmation_sig");
		exit(0);
	}
	if (signum == SIGUSR2)
		error_handler("Server failed");
}

void	server_status(int signum, siginfo_t *info, void *ucontext)
{
	(void)ucontext;
	(void)info;
	if (DEBUG == ON)
		printf("In server_status\n");
	if (signum == SIGUSR2)
	{
		write(1, "Server is busy, please wait⌛\n", 32);
		sleep(3);
	}
	else
		g_server_is_open = true;
}

int	main(int argc, char **argv)
{
	__pid_t	server_pid;
	int		i;

	args_check(argc, argv);
	server_pid = minitalk_atoi(argv[1]);
	signal_config(server_status);
	knock (server_pid);
	if (DEBUG == ON)
		printf("Got answer, sending message\n");
	i = 0;
	while (argv[2][i] != '\0')
		send_char(server_pid, argv[2][i++]);
	send_char(server_pid, argv[2][i]);
	if (DEBUG == ON)
		printf("Message sent\n");
	signal_config(confirmation_sig);
	pause();
	return (0);
}
