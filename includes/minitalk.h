/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:28:59 by rtammi            #+#    #+#             */
/*   Updated: 2024/09/03 14:38:01 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>

size_t	minitalk_strlen(const char *s);
int		minitalk_atoi(const char *str);
void	minitalk_print_pid(int long nbr);
void	error_handler(char *error_message);
void	send_nullpointer(__pid_t server_pid);
void	*minitalk_memset(void *b, int c, size_t n);

#endif