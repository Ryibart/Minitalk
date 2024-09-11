/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:59:26 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/11 20:26:45 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile sig_atomic_t	g_server_is_open = false;

void	status_handler(int signum, siginfo_t *info, void *ucontext)
{
	static int	retries = 5;
	int			timeout;

	(void)ucontext;
	(void)info;
	if (DEBUG == YES)
		printf("In server_status\n");
	if (signum == SIGUSR1)
	{
		if (DEBUG == YES)
			printf("Server is open\n");
		g_server_is_open = true;
	}
	else
	{
		if (DEBUG == YES)
			printf("Server is busy\n");
		retries = MAX_RETRY;
		while (retries > 0)
		{
			timeout = TIMEOUT_COUNT;
			while (g_server_is_open == false && timeout > 0)
			{
				usleep(RETRY_SLEEP);
				timeout--;
				if (g_server_is_open == true)
					break ;
				if (timeout == 0)
				{
					retries--;
					if (retries == 0)
					{
						error_handler("Timeout occurred, server too busy");
					}
					if (DEBUG == YES)
						printf("Retrying knock signal, %d retries left\n", retries);
				}
			}
			send_signal(info->si_pid, SIGUSR2, LONG_T, CLIENT);
			if (g_server_is_open == true)
				break ;
		}
	}
}

void	confirmation_handler(int signum, siginfo_t *info, void *ucontext)
{
	(void)ucontext;
	(void)info;
	if (DEBUG == YES)
		printf("In confirmation_sig\n");
	if (signum == SIGUSR1)
	{
		if (write(1, "Message printed by server\n", 27) == -1)
			error_handler("Write failed in confirmation_sig");
		exit(EXIT_SUCCESS);
	}
	if (signum == SIGUSR2)
		error_handler("Server failed");
}

int	main(int argc, char **argv)
{
	__pid_t	server_pid;
	__pid_t	client_pid;
	int		i;

	if (DEBUG == YES)
		client_pid = getpid();
	args_check(argc, argv);
	server_pid = minitalk_atoi(argv[1]);
	signal_config(status_handler);
	if (DEBUG == YES)
		printf("Client %u is knocking\n", client_pid);
	send_signal(server_pid, SIGUSR2, LONG_T, CLIENT);
	if (DEBUG == YES)
		printf("Got answer, sending message\n");
	i = 0;
	usleep(SHORT_T);
	while (argv[2][i] != '\0')
		send_char(server_pid, argv[2][i++], &g_server_is_open);
	send_char(server_pid, argv[2][i], &g_server_is_open);
	if (DEBUG == YES)
		printf("Message sent\n");
	signal_config(confirmation_handler);
	sleep(5);
	error_handler("Timeout, server failed");
}
