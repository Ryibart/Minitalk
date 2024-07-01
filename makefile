# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/26 18:30:16 by rtammi            #+#    #+#              #
#    Updated: 2024/07/01 15:47:44 by rtammi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC					= cc

CFLAGS				+= -Wall -Wextra -Werror

DEPSFLAGS			+= -MMD -MP

RM					= rm -f

SERVER				= server
CLIENT				= client

SERVER_SRCS			= 	sources/server.c \
						sources/utilities.c

SERVER_OBJS			= $(SERVER_SRCS:.c=.o)

CLIENT_SRCS			= 	sources/client.c	\
						sources/utilities.c
CLIENT_OBJS			= $(CLIENT_SRCS:.c=.o)

SERVER_BONUS 		= server_bonus
CLIENT_BONUS 		= client_bonus

SERVER_BONUS_SRCS 	= sources_bonus/server_bonus.c
SERVER_BONUS_OBJS 	= $(SERVER_BONUS_SRCS:.c=.o)

CLIENT_BONUS_SRCS 	= sources_bonus/client_bonus.c
CLIENT_BONUS_OBJS 	= $(CLIENT_BONUS_SRCS:.c=.o)

DEPS				= $(SERVER_OBJS:.o=.d) $(CLIENT_OBJS:.o=.d)  $(SERVER_BONUS_OBJS:.o=.d) $(CLIENT_BONUS_OBJS:.o=.d)

all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(SERVER_OBJS) -o $@

$(CLIENT): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) $(CLIENT_OBJS) -o $@


$(SERVER_BONUS): $(SERVER_BONUS_OBJS)
	$(CC) $(CFLAGS) $(SERVER_BONUS_OBJS) -o $@

$(CLIENT_BONUS): $(CLIENT_BONUS_OBJS)
	$(CC) $(CFLAGS) $(CLIENT_BONUS_OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(DEPSFLAGS) -c $< -o $@

-include $(DEPS)

clean:
	$(RM) $(SERVER_OBJS) $(CLIENT_OBJS) $(SERVER_BONUS_OBJS) $(CLIENT_BONUS_OBJS) $(DEPS)

fclean: clean
	$(RM) $(SERVER) $(CLIENT) $(SERVER_BONUS) $(CLIENT_BONUS)
	
re: fclean all

redeps:
	touch $(DEPS)
	$(MAKE)

bonus: $(SERVER_BONUS) $(CLIENT_BONUS)

.PHONY: all clean fclean re redeps bonus
