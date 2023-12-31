# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/28 10:47:26 by fvon-nag          #+#    #+#              #
#    Updated: 2023/09/26 15:52:41 by fvon-nag         ###   ########.fr        #
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
			builtin.c \
			fork.c \
			input_analysis_2.c \
			parsing_redirection_2.c \
			heredoc.c \
			fd_redirection.c \
			child_executor.c \
			piping.c \


OBJECTS = $(SOURCES:.c=.o)
NAME = minishell
LIBFT = ./Libft/libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I $(HOME)/goinfre/.brew/opt/readline/include/

all: $(NAME)

$(NAME): $(LIBFT) $(OBJECTS)
	$(CC) $(CFLAGS) $(LIBFT) -L$(HOME)/goinfre/.brew/opt/readline/lib/ -lreadline $(OBJECTS) -o $(NAME)

$(LIBFT):
	make -C Libft

clean:
	make -C Libft clean
	rm -f *.o
fclean: clean
	make fclean -C Libft
	rm -rf $(NAME) minishell.dSYM

re: fclean all

.PHONY: all bonus libft clean fclean re

