/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:30:02 by rtammi            #+#    #+#             */
/*   Updated: 2024/07/08 16:52:19 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minitalk.h"
#include <stdio.h>

void	args_check(int argc, char **argv)
{
	int i;

	i = 0;
	if (argc != 3)
		error_handler("Invalid number of arguments, please only insert PID and message");
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
				//printf("sending SIGUSR1\n");
				kill(server_pid, SIGUSR1);
			}
			else
			{
				//printf("sending SIGUSR2\n");
				kill(server_pid, SIGUSR2);
			}
			usleep(300);
			c <<= 1;
		}
		msg++;
	}
}

void	sigusr_recieved(int signum)
{
	if (signum == SIGUSR1)
		write(1, "Affirmative, character has landed!\n",  36);
}


void	signal_config(void)
{
	struct	sigaction	sa_newsignal;
	
	sa_newsignal.sa_handler = sigusr_recieved; // sa_handler or sa_sigaction?
	sa_newsignal.sa_flags = 0;
	sigemptyset(&sa_newsignal.sa_mask); // is this necessary?
	//printf("Configuring signal handlers...\n");
	if (sigaction(SIGUSR1, &sa_newsignal, NULL) == -1)
		error_handler("SIGSUR1 behavior didn't change.");
	if (sigaction(SIGUSR2, &sa_newsignal, NULL) == -1)
		error_handler("SIGSUR2 behavior didn't change.");
}


int main(int argc, char **argv)
{
	__pid_t	server_pid;
	// __pid_t	client_pid;

	args_check(argc, argv);
	// client_pid = getpid();
	// if  (client_pid < 0)
	// 	error_handler("Failed to get client PID");
	server_pid = minitalk_atoi(argv[1]);
	// minitalk_print_pid(client_pid);
	// write(1, " Is client PID\n", 16);
	signal_config();
	send_message(server_pid, argv[2]);
	while (1)
		pause();
	return (0);	
	}
	