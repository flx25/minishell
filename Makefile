# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/28 10:47:26 by fvon-nag          #+#    #+#              #
#    Updated: 2023/04/17 15:08:49 by fvon-nag         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #





SOURCES = main.c \
			prompt.c \
			signals.c \
			expand_env_isinquotes.c \
			expand_env.c \
			ft_strncpy.c \







OBJECTS = $(SOURCES:.c=.o)
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I $(HOME)/goinfre/.brew/opt/readline/include/

$(NAME): $(OBJECTS) libft
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) -LLibft -lft -L $(HOME)/goinfre/.brew/opt/readline/lib/ -lreadline

all: $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

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

