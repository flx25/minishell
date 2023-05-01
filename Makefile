# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/28 10:47:26 by fvon-nag          #+#    #+#              #
#    Updated: 2023/05/01 16:26:53 by melkholy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #





SOURCES = main.c \
			parsing_utils.c \
			parsing_redirection.c \
			lexer.c \
			parser.c \
			input_analysis.c \
			signals.c \
			syscommands.c \
			syscommands_utils.c \
			export.c \
			export_no_args.c \
			export_no_args_2.c \
			export_w_args.c \
			buildins.c \
			buildins_2.c \
			execution.c \


NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I $(HOME)/goinfre/.brew/opt/readline/include/

$(NAME): libft
	$(CC) $(CFLAGS) -o $@ $(SOURCES) -LLibft -lft -L $(HOME)/goinfre/.brew/opt/readline/lib/ -lreadline

all: $(NAME)

libft:
	+make -C Libft

clean:
	rm -f $(OBJECTS) $(BOBJECTS) $(EXECUTABLES).o
	+make -C Libft clean

fclean: clean
	rm -rf $(NAME) Libft/libft.a $(EXECUTABLES) minishell.dSYM

re: fclean all

.PHONY: all bonus libft clean fclean re

