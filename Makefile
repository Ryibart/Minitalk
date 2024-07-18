# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/26 18:30:16 by rtammi            #+#    #+#              #
#    Updated: 2024/07/18 12:22:40 by rtammi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and Flags
CC              = cc
CFLAGS          += -Wall -Wextra -Werror
DEPSFLAGS       += -MMD -MP
ECHOFLAG		+= -e

# Clean Command
RM              = rm -f

# Target Executables
SERVER          = server
CLIENT          = client
SERVER_BONUS    = server_bonus
CLIENT_BONUS    = client_bonus

# Source Files
SERVER_SRCS     = sources/server.c sources/utilities.c sources/print_pid.c sources/utilities/minitalk_realloc.c
CLIENT_SRCS     = sources/client.c sources/utilities.c sources/print_pid.c
SERVER_BONUS_SRCS = sources_bonus/server_bonus.c
CLIENT_BONUS_SRCS = sources_bonus/client_bonus.c

# Object Files
SERVER_OBJS     = $(SERVER_SRCS:.c=.o)
CLIENT_OBJS     = $(CLIENT_SRCS:.c=.o)
SERVER_BONUS_OBJS = $(SERVER_BONUS_SRCS:.c=.o)
CLIENT_BONUS_OBJS = $(CLIENT_BONUS_SRCS:.c=.o)

# Dependency Files
DEPS            = $(SERVER_OBJS:.o=.d) $(CLIENT_OBJS:.o=.d) $(SERVER_BONUS_OBJS:.o=.d) $(CLIENT_BONUS_OBJS:.o=.d)

# Color and Style Codes
YELLOW          = \033[33m
GREEN           = \033[32m
BLUE            = \033[34m
RED             = \033[31m
BOLD            = \033[1m
NC              = \033[0m

# Targets
all: $(SERVER) $(CLIENT)

$(SERVER): $(SERVER_OBJS)
	@echo $(ECHOFLAG) "$(YELLOW)🔗 Linking $(SERVER)...$(NC)"
	@$(CC) $(CFLAGS) $(SERVER_OBJS) -o $@ \
		&& echo $(ECHOFLAG) "🥳 $(BOLD)$(GREEN)$(SERVER) built successfully!$(NC)" \
		|| { echo $(ECHOFLAG) "$(BOLD)$(RED)Error: Failed to build $(SERVER)!🤣$(NC)"; exit 1; }

$(CLIENT): $(CLIENT_OBJS)
	@echo $(ECHOFLAG) "$(YELLOW)🔗 Linking $(CLIENT)...$(NC)"
	@$(CC) $(CFLAGS) $(CLIENT_OBJS) -o $@ \
		&& echo $(ECHOFLAG) "🥳 $(BOLD)$(GREEN)$(CLIENT) built successfully!$(NC)" \
		|| { echo $(ECHOFLAG) "$(BOLD)$(RED)Error: Failed to build $(CLIENT)!🤣$(NC)"; exit 1; }

$(SERVER_BONUS): $(SERVER_BONUS_OBJS)
	@echo $(ECHOFLAG) "$(YELLOW)🔗 Linking $(SERVER_BONUS)...$(NC)"
	@$(CC) $(CFLAGS) $(SERVER_BONUS_OBJS) -o $@ \
		&& echo $(ECHOFLAG) "$🥳 (BOLD)$(GREEN)$(SERVER_BONUS) built successfully!$(NC)" \
		|| { echo $(ECHOFLAG) "$(BOLD)$(RED)Error: Failed to build $(SERVER_BONUS)!🤣$(NC)"; exit 1; }

$(CLIENT_BONUS): $(CLIENT_BONUS_OBJS)
	@echo $(ECHOFLAG) "$(YELLOW)🔗 Linking $(CLIENT_BONUS)...$(NC)"
	@$(CC) $(CFLAGS) $(CLIENT_BONUS_OBJS) -o $@ \
		&& echo $(ECHOFLAG) "🥳 $(BOLD)$(GREEN)$(CLIENT_BONUS) built successfully!$(NC)" \
		|| { echo $(ECHOFLAG) "$(BOLD)$(RED)Error: Failed to build $(CLIENT_BONUS)!🤣$(NC)"; exit 1; }

%.o: %.c
	@echo $(ECHOFLAG) "$(BLUE)🤔 Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(DEPSFLAGS) -c $< -o $@ \
		&& echo $(ECHOFLAG) "🥳 $(BOLD)$(GREEN)Compiled $< successfully!$(NC)" \
		|| { echo $(ECHOFLAG) "$(BOLD)$(RED)Error: Failed to compile $<!🤣$(NC)"; exit 1; }

-include $(DEPS)

clean:
	@echo $(ECHOFLAG) "$(YELLOW)🚮 Cleaning up...$(NC)"
	@$(RM) $(SERVER_OBJS) $(CLIENT_OBJS) $(SERVER_BONUS_OBJS) $(CLIENT_BONUS_OBJS) $(DEPS) \
		&& echo $(ECHOFLAG) "🧹 $(BOLD)$(GREEN)Cleaned up object and dependency files!$(NC)" \
		|| { echo $(ECHOFLAG) "$(BOLD)$(RED)Error: Failed to clean up!🤣$(NC)"; exit 1; }

fclean: clean
	@echo $(ECHOFLAG) "$(YELLOW)😱 Removing executables...$(NC)"
	@$(RM) $(SERVER) $(CLIENT) $(SERVER_BONUS) $(CLIENT_BONUS) \
		&& echo $(ECHOFLAG) "🫡 $(BOLD)$(GREEN)Executables removed!$(NC)" \
		|| { echo $(ECHOFLAG) "$(BOLD)$(RED)Error: Failed to remove executables!🤣$(NC)"; exit 1; }

re: fclean all

redeps:
	@echo $(ECHOFLAG) "$(YELLOW)💽 Updating dependencies...$(NC)"
	@touch $(DEPS) \
		&& $(MAKE) --no-print-directory \
		&& echo $(ECHOFLAG) "🥳 $(BOLD)$(GREEN)Dependencies updated succesfully!$(NC)" \
		|| { echo $(ECHOFLAG) "$(BOLD)$(RED)Error: Failed to update dependencies!🤣$(NC)"; exit 1; }

bonus: $(SERVER_BONUS) $(CLIENT_BONUS)
	@echo $(ECHOFLAG) "$(YELLOW)💯 Building bonus targets...$(NC)"
	@echo $(ECHOFLAG) "🥳 $(BOLD)$(GREEN)Bonus targets built successfully!$(NC)"

.PHONY: all clean fclean re redeps bonus