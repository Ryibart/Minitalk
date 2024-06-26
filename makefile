# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/26 18:30:16 by rtammi            #+#    #+#              #
#    Updated: 2024/06/26 20:48:24 by rtammi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC					= cc

CFLAGS				+= -Wall -Wextra -Werror

DEPSFLAGS			+= -MMD -MP

RM					= rm -f

SERVER				= server
CLIENT				= client

SERVER_SRCS			= sources/server.c
SERVER_OBJS			= $(SERVER_SRCS:.c=.o)

CLIENT_SRCS			= sources/client.c
CLIENT_OBJS			= $(CLIENT_SRCS:.c=.o)

SERVER_BONUS 		= server_bonus
CLIENT_BONUS 		= client_bonus

SERVER_BONUS_SRCS 	= sources_bonus/server_bonus.c
SERVER_BONUS_OBJS 	= $(SERVER_BONUS_SRCS:.c=.o)

CLIENT_BONUS_SRCS 	= sources_bonus/client_bonus.c
CLIENT_BONUS_OBJS 	= $(CLIENT_BONUS_SRCS:.c=.o)

DEPS				= $(SERVER_SRCS:.c=.d) $(CLIENT_SRCS:.c=.d)

all:
	$(SERVER) $(CLIENT)

$(SERVER): $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(DEPSFLAGS) $(SERVER_OBJS) -o server

$(CLIENT): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) $(DEPSFLAGS) $(CLIENT_OBJS) -o client


$(SERVER_BONUS): $(SERVER_BONUS_OBJS)
	$(CC) $(CFLAGS) $(DEPSFLAGS) $(SERVER_BONUS_OBJS) -o server_bonus

$(CLIENT_BONUS): $(CLIENT_BONUS_OBJS)
	$(CC) $(CFLAGS) $(DEPSFLAGS) $(CLIENT_BONUS_OBJS) -o client_bonus

-include $(DEPS)

clean:
	$(RM) $(SERVER_OBJS) $(CLIENT_OBJS) $(DEPS)
	$(RM) $(SERVER_BONUS_OBJS) $(CLIENT_BONUS_OBJS) $(DEPS)

fclean: clean
	$(RM) $(SERVER) $(CLIENT)
	$(RM) $(SERVER_BONUS) $(CLIENT_BONUS)
	
re: fclean all

redeps:
	touch $(DEPS)
	make

bonus: $(SERVER_BONUS) $(CLIENT_BONUS)

.PHONY: all clean fclean re redeps bonus
