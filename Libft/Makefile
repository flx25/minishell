# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fvon-nag <fvon-nag@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/11 21:15:11 by melkholy          #+#    #+#              #
#    Updated: 2023/04/28 13:24:17 by fvon-nag         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
NAME = libft.a
CDIRS = .
CFLAGS = -g -Wall -Wextra -Werror

CFILES = ft_atoi.c ft_isdigit.c ft_memchr.c ft_putendl_fd.c \
			ft_striteri.c ft_strncmp.c ft_toupper.c ft_bzero.c \
			ft_isprint.c ft_memcmp.c ft_putnbr_fd.c ft_strjoin.c \
			ft_strnstr.c ft_calloc.c ft_itoa.c ft_memcpy.c \
			ft_putstr_fd.c ft_strlcat.c ft_strrchr.c ft_isalnum.c \
			ft_memmove.c ft_split.c ft_strlcpy.c ft_strtrim.c \
			ft_isalpha.c ft_memset.c ft_strchr.c ft_strlen.c \
			ft_substr.c ft_isascii.c ft_putchar_fd.c ft_strdup.c \
			ft_strmapi.c ft_tolower.c get_next_line.c \
			get_next_line_utils.c \
			ft_lstdelone.c ft_lstiter.c ft_lstlast.c ft_lstadd_back.c \
			ft_lstmap.c ft_lstadd_front.c ft_lstnew.c ft_lstclear.c \
			ft_lstsize.c

OBJ = ${CFILES:.c=.o}

all : $(NAME)

$(NAME) :
	@$(CC) $(CFLAGS) -I$(CDIRS) -c $(CFILES)
	@ar rc $(NAME) $(OBJ)
	@ranlib $(NAME)

clean:
	@rm -f $(OBJ)

fclean : clean
	@rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re
