/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk_bonus.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/26 20:50:50 by rtammi            #+#    #+#             */
/*   Updated: 2024/07/11 15:09:55 by rtammi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_BONUS_H
# define MINITALK_BONUS_H

# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>

size_t	minitalk_strlen(const char *s);
int		minitalk_atoi(const char *str);
void	minitalk_print_pid(int long nbr);
void	error_handler(char *error_message);

#endif