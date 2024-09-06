/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:59:26 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/06 20:11:50 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile size_t	g_server_is_open = false;

int	retry_adjuster(int adjuster, int reset)
{
	static int	retry_adjuster = 5;
	int			abs_adjuster;

	if (reset)
		retry_adjuster = 5;
	else if (adjuster > 0)
		retry_adjuster += adjuster;
	else if (adjuster < 0)
	{
		abs_adjuster = -adjuster;
		retry_adjuster -= abs_adjuster;
	}
	return (retry_adjuster);
}

void	status_handler(int signum, siginfo_t *info, void *ucontext)
{
	int	retry;

	(void)ucontext;
	(void)info;
	if (DEBUG == YES)
		printf("In server_status\n");
	if (signum == SIGUSR2)
	{
		retry = retry_adjuster(1, NO);
		if (retry > 10)
		{
			write(1, "Retry count too high, resetting to 5\n", 38);
			retry_adjuster(0, YES);
		}
		sleep(3);
	}
	else
		g_server_is_open = true;
}

void	knock(__pid_t server_pid)
{
	int	retry;

	while (g_server_is_open == false)
	{
		printf("Waiting for server response...\n");
		send_signal(server_pid, SIGUSR2, 4000, CLIENT);
		if (g_server_is_open == false)
		{
			retry = retry_adjuster(-1, NO);
			printf("Server is busy. %d retries left\n", retry);
			sleep(RETRY_DELAY);
		}
		if (retry == 0)
			error_handler("Client timeout, server too busy");
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
		exit(0);
	}
	if (signum == SIGUSR2)
		error_handler("Server failed");
}

int	main(int argc, char **argv)
{
	__pid_t	server_pid;
	int		i;

	args_check(argc, argv);
	server_pid = minitalk_atoi(argv[1]);
	signal_config(status_handler);
	knock(server_pid);
	if (DEBUG == YES)
		printf("Got answer, sending message\n");
	i = 0;
	while (argv[2][i] != '\0')
		send_char(server_pid, argv[2][i++]);
	send_char(server_pid, argv[2][i]);
	if (DEBUG == YES)
		printf("Message sent\n");
	signal_config(confirmation_handler);
	pause();
	return (0);
}
