# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/28 10:47:26 by fvon-nag          #+#    #+#              #
#    Updated: 2023/04/20 13:05:00 by fvon-nag         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #





SOURCES = main.c \
			prompt.c \
			signals.c \
			syscommands.c \
			syscommands_utils.c \
			export.c \
			buildins.c \









# OBJECTS = $(SOURCES:.c=.o)
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I $(HOME)/goinfre/.brew/opt/readline/include/

$(NAME): libft
	$(CC) $(CFLAGS) -o $@ $(SOURCES) -LLibft -lft -L $(HOME)/goinfre/.brew/opt/readline/lib/ -lreadline

all: $(NAME)

# %.o: %.c
	# $(CC) -c $(CFLAGS) $< -o $@

libft:
	+make -C Libft

clean:
	rm -f $(OBJECTS) $(BOBJECTS) $(EXECUTABLES).o
	+make -C Libft clean

fclean: clean
	rm -f $(NAME) Libft/libft.a $(EXECUTABLES)

%: %.c
		$(CC) $(CFLAGS) -o $@ $<

re: fclean all

.PHONY: all bonus libft clean fclean re

