# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rtammi <rtammi@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/26 18:30:16 by rtammi            #+#    #+#              #
#    Updated: 2024/09/12 17:56:53 by rtammi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Paths

HEADER			:= ./includes/
SOURCES			:= ./sources/
UTILITIES		:= ./sources/utilities/

# Compiler and Flags
CC              := cc
CFLAGS          := 
DEPSFLAGS       := -MMD -MP
ECHOFLAG		:= 
HEADERFLAG		:= -I ./includes/

# Clean Command
RM              := rm -f

# Target Executables
SERVER          := server
CLIENT          := client
# Source Files
SERVER_SRCS     := $(SOURCES)server.c $(UTILITIES)minitalk_utilities.c $(UTILITIES)server_utilities.c $(UTILITIES)minitalk_realloc.c
CLIENT_SRCS     := $(SOURCES)client.c $(UTILITIES)minitalk_utilities.c $(UTILITIES)client_utilities.c

# Object Files
SERVER_OBJS     = $(SERVER_SRCS:.c=.o)
CLIENT_OBJS     = $(CLIENT_SRCS:.c=.o)

# Dependency Files
DEPS            = $(SERVER_OBJS:.o=.d) $(CLIENT_OBJS:.o=.d)

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
	@$(CC) $(CFLAGS) $(SERVER_OBJS) $(HEADERFLAG) -o $@ \
		&& echo $(ECHOFLAG) "🥳 $(BOLD)$(GREEN)$(SERVER) built successfully!$(NC)" \
		|| { echo $(ECHOFLAG) "$(BOLD)$(RED)Error: Failed to build $(SERVER)!🤣$(NC)"; exit 1; }

$(CLIENT): $(CLIENT_OBJS)
	@echo $(ECHOFLAG) "$(YELLOW)🔗 Linking $(CLIENT)...$(NC)"
	@$(CC) $(CFLAGS) $(CLIENT_OBJS) $(HEADERFLAG) -o $@ \
		&& echo $(ECHOFLAG) "🥳 $(BOLD)$(GREEN)$(CLIENT) built successfully!$(NC)" \
		|| { echo $(ECHOFLAG) "$(BOLD)$(RED)Error: Failed to build $(CLIENT)!🤣$(NC)"; exit 1; }

%.o: %.c
	@echo $(ECHOFLAG) "$(BLUE)🤔 Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(DEPSFLAGS) $(HEADERFLAG) -c $< -o $@ \
		&& echo $(ECHOFLAG) "🥳 $(BOLD)$(GREEN)Compiled $< successfully!$(NC)" \
		|| { echo $(ECHOFLAG) "$(BOLD)$(RED)Error: Failed to compile $<!🤣$(NC)"; exit 1; }

-include $(DEPS)

clean:
	@echo $(ECHOFLAG) "$(YELLOW)🚮 Cleaning up...$(NC)"
	@$(RM) $(SERVER_OBJS) $(CLIENT_OBJS) $(DEPS) \
		&& echo $(ECHOFLAG) "🧹 $(BOLD)$(GREEN)Cleaned up object and dependency files!$(NC)" \
		|| { echo $(ECHOFLAG) "$(BOLD)$(RED)Error: Failed to clean up!🤣$(NC)"; exit 1; }

fclean: clean
	@echo $(ECHOFLAG) "$(YELLOW)😱 Removing executables...$(NC)"
	@$(RM) $(SERVER) $(CLIENT) \
		&& echo $(ECHOFLAG) "🫡 $(BOLD)$(GREEN)Executables removed!$(NC)" \
		|| { echo $(ECHOFLAG) "$(BOLD)$(RED)Error: Failed to remove executables!🤣$(NC)"; exit 1; }

re: fclean all

redeps:
	@echo $(ECHOFLAG) "$(YELLOW)💽 Updating dependencies...$(NC)"
	@touch $(DEPS) \
		&& $(MAKE) --no-print-directory \
		&& echo $(ECHOFLAG) "🥳 $(BOLD)$(GREEN)Dependencies updated succesfully!$(NC)" \
		|| { echo $(ECHOFLAG) "$(BOLD)$(RED)Error: Failed to update dependencies!🤣$(NC)"; exit 1; }

.PHONY: all clean fclean re redeps