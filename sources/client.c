/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:59:26 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/16 19:02:52 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile sig_atomic_t	g_server_is_open = false;

static void	status_signal(__pid_t server_pid, int *retries)
{
	int	timeout;

	while (*retries > 0)
	{
		timeout = TIMEOUT_COUNT;
		while (g_server_is_open == false && timeout > 0)
		{
			usleep(RETRY_SLEEP);
			if (g_server_is_open == true)
				break ;
			if (--timeout == 0)
			{
				if (--retries == 0)
					minitalk_print("Timeout occurred, server too busy",
						ERROR, NULL, NULL);
				minitalk_print("Recipient busy, retrying sending verify signal",
					MESSAGE, YELLOW, BOLD_ITALIC);
			}
		}
		if (kill(server_pid, SIGUSR2) == -1)
			minitalk_print("Signal sending failed, invalid recipient",
				ERROR, NULL, NULL);
		if (g_server_is_open == true)
			break ;
	}
}

void	status_handler(int signum, siginfo_t *info, void *ucontext)
{
	static __pid_t	server_pid = 0;
	int				retries;

	(void)ucontext;
	(void)info;
	if (server_pid == 0)
		server_pid = info->si_pid;
	else if (server_pid != info->si_pid)
		return ;
	if (signum == SIGUSR1)
		g_server_is_open = true;
	retries = MAX_RETRY;
	if (signum == SIGUSR2)
		status_signal(server_pid, &retries);
}

void	confirmation_handler(int signum, siginfo_t *info, void *ucontext)
{
	(void)ucontext;
	(void)info;
	if (signum == SIGUSR1)
	{
		minitalk_print(""MOVE_CURSOR_UP""CLEAR_LINE"💘\
	 ⋞ Love letter delivered ⋟	 💘", MESSAGE, GREEN, BOLD);
		exit(EXIT_SUCCESS);
	}
	if (signum == SIGUSR2)
		minitalk_print("Server failed", ERROR, NULL, NULL);
}

int	main(int argc, char **argv)
{
	__pid_t	server_pid;
	int		i;

	args_check(argc, argv);
	minitalk_print("❇️  Welcome to Teletammi™ dating services ❇️\n\
"BLINK"💌   Currently sending your loveletter   💌",
		MESSAGE, PURPLE, BOLD);
	server_pid = minitalk_atoi(argv[1]);
	signal_config(status_handler);
	kill(server_pid, SIGUSR2);
	sleep(1);
	i = 0;
	while (argv[2][i] != '\0')
		send_char(server_pid, argv[2][i++], &g_server_is_open);
	send_char(server_pid, argv[2][i], &g_server_is_open);
	signal_config(confirmation_handler);
	sleep(15);
	minitalk_print("Timeout, confirmation not received", ERROR, NULL, BOLD);
}
