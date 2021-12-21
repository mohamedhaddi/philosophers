# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/21 07:20:17 by mhaddi            #+#    #+#              #
#    Updated: 2021/12/21 23:19:58 by mhaddi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= 	philo
SRCS		=	src/main.c \
				src/data/validate_args.c \
				src/data/get_data.c \
				src/data/validate_data.c
RM			= 	rm -f
CC			= 	clang
CFLAGS		= 	-Wall -Wextra -Werror
DFLAGS		= 	-g -fsanitize=address

all:		$(NAME)

$(NAME):
			$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

clean:
			$(RM) $(NAME)

fclean:		clean
			$(RM) -r *.dSYM

re:			fclean all

.PHONY:		all clean fclean re
