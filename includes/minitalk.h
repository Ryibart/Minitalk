/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:28:59 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/16 13:38:38 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

/* Identifier macros */
# define CLIENT 1
# define SERVER 2

/* Macros for retry mechanism */
# define MAX_RETRY     5	  // Times retry performs
# define RETRY_SLEEP   500000	 // Microseconds usleep is active
# define TIMEOUT_COUNT 3	// Times will RETRY_SLEEP perform before next retry

/* Macros for send_signal usleep times */
# define SHORT_T	1
# define LONG_T		300

/* Flags for minitalk_print */
# define ERROR  1
# define MESSAGE 2

/* ANSI codes */
# define RED    "\033[31m"
# define GREEN  "\033[32m"
# define PURPLE "\033[35m"
# define YELLOW "\033[33m"

# define MOVE_CURSOR_UP "\033[A"
# define CLEAR_LINE 	"\033[K"
# define BOLD        	"\033[1m"
// # define ITALIC			"\033[3m"
// # define UNDERLINE		"\033[4m"
# define BLINK			"\033[5m"
# define BOLD_ITALIC	"\033[1;3m"

# define RESET	"\033[0m"

/* Headers */
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>

# include <stdio.h> // printf debug

/* Struct for server to handle message */
typedef struct s_message
{
	char		*buffer;
	size_t		buffer_size;
	size_t		buffer_index;
	int			bit_iter;
	char		current_char;
}				t_message;

/* minitalk_utilities.c */
size_t	minitalk_strlen(const char *s);
void	retry_message(char *message);
void	minitalk_print(char *message, int flag, char *color, char *format);
void	signal_config(void *handler);

/* minitalk_realloc.c */
void	*minitalk_memset(void *b, int c, size_t n);
void	*minitalk_realloc(void *ptr, size_t size);
void	minitalk_meta_free(void *ptr);

/* server_utilities.c */
void	minitalk_print_pid(int pid);
void	reset_message(t_message *msg);
int		verify_message(siginfo_t *info, __pid_t *current_client_pid,
			volatile sig_atomic_t *processing_message);

/* client_utilities.c */
void	args_check(int argc, char **argv);
int		minitalk_atoi(const char *str);
void	send_char(__pid_t server_pid, unsigned char c,
			volatile sig_atomic_t *server_is_open);
#endif