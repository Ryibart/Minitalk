/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:28:59 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/11 20:30:18 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# define CLIENT 1
# define SERVER 2

# define YES 1
# define NO 0
# define DEBUG YES
# define SERVER_DEBUG YES

# define MAX_RETRY 5
# define TIMEOUT_COUNT 1000
# define RETRY_SLEEP 5000

# define SHORT_T 0
# define MED_T 500
# define LONG_T 1000

# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <stdio.h> //TAKE THIS OUT (for a walk)

typedef struct s_message
{
	char		*buffer;
	size_t		buffer_size;
	size_t		buffer_index;
	int			bit_iter;
	char		current_char;
}				t_message;

size_t	minitalk_strlen(const char *s);
int		minitalk_atoi(const char *str);
void	minitalk_print_pid(int pid);
void	error_handler(char *error_message);
void	*minitalk_memset(void *b, int c, size_t n);
void	args_check(int argc, char **argv);
void	*minitalk_realloc(void *ptr, size_t size);
void	minitalk_meta_free(void *ptr);
void	signal_config(void *sigaction_function);
void	send_signal(__pid_t pid, int signal, int sleep_time, int sender);
void	reset_message(t_message *msg);
void	append_to_buffer(t_message *msg);
void	send_char(__pid_t server_pid, unsigned char c,
			volatile sig_atomic_t *server_is_open);
int		char_length(unsigned char c);

#endif