/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:59:26 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/12 18:40:00 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile sig_atomic_t	g_server_is_open = false;

void	status_handler(int signum, siginfo_t *info, void *ucontext)
{
	int				retries;
	int				timeout;
	static __pid_t	server_pid = 0;

	(void)ucontext;
	(void)info;
	if (server_pid == 0)
		server_pid = info->si_pid;
	else if (server_pid != info->si_pid)
		return ;
	if (CLIENT_DEBUG == ON)
		printf("In server_status\n");
	if (signum == SIGUSR1)
	{
		if (CLIENT_DEBUG == ON)
			printf("Server is open\n");
		g_server_is_open = true;
	}
	else if (signum == SIGUSR2)
	{
		if (CLIENT_DEBUG == ON)
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
						minitalk_print("Timeout occurred, server too busy", ERROR, NULL, BOLD);
					retry_message("Recipient busy, retrying sending verify signal");
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
	if (CLIENT_DEBUG == ON)
		printf("In confirmation_sig\n");
	if (signum == SIGUSR1)
	{
		if (write(1, "Message printed by server\n", 27) == -1)
			minitalk_print("Write failed in confirmation_sig", ERROR, NULL, BOLD);
		exit(EXIT_SUCCESS);
	}
	if (signum == SIGUSR2)
		minitalk_print("Server failed", ERROR, NULL, BOLD);
}

int	main(int argc, char **argv)
{
	__pid_t	server_pid;
	__pid_t	client_pid;
	int		i;

	if (CLIENT_DEBUG == ON)
		client_pid = getpid();
	args_check(argc, argv);
	server_pid = minitalk_atoi(argv[1]);
	signal_config(status_handler);
	if (CLIENT_DEBUG == ON)
		printf("Client %u is knocking\n", client_pid);
	send_signal(server_pid, SIGUSR2, SHORT_T, CLIENT);
	if (CLIENT_DEBUG == ON)
		printf("Got answer, sending message\n");
	i = 0;
	usleep(SHORT_T);
	while (argv[2][i] != '\0')
		send_char(server_pid, argv[2][i++], &g_server_is_open);
	send_char(server_pid, argv[2][i], &g_server_is_open);
	if (CLIENT_DEBUG == ON)
		printf("Message sent\n");
	signal_config(confirmation_handler);
	sleep(10);
	minitalk_print("Timeout, confirmation not received", ERROR, NULL, BOLD);
}
