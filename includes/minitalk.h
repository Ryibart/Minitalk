/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 18:28:59 by rtammi            #+#    #+#             */
/*   Updated: 2024/07/18 13:17:49 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# define MESSAGE_RECEIVED_TRUE 1
# define MESSAGE_RECEIVED_FALSE 0
# define MESSAGE_FAILED_TRUE 1 
# define MESSAGE_FAILED_FALSE 0

# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>

size_t	minitalk_strlen(const char *s);
int		minitalk_atoi(const char *str);
void	minitalk_print_pid(int long nbr);
void	error_handler(char *error_message);
void	*minitalk_realloc(void *ptr, size_t size);
void	minitalk_free(void *ptr);
void	*minitalk_malloc(size_t size);

#endif