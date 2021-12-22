# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/05/21 07:20:17 by mhaddi            #+#    #+#              #
#    Updated: 2021/12/22 16:52:23 by mhaddi           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= 	philo
SRCS		=	./src/main.c \
				./src/utils.c \
				./src/data/get_data.c \
				./src/data/utils/is_over_int.c \
				./src/data/utils/is_integer.c \
				./src/data/utils/ft_atoi.c \
				./src/data/utils/ft_isdigit.c \
				./src/data/utils/ft_strlen.c \
				./src/locks/create_locks.c \
				./src/locks/destroy_locks.c \
				./src/threads/create_threads.c \
				./src/supervisor/supervise.c \
				./src/philosopher_routine/routine.c \
				./src/philosopher_routine/actions/eat.c \
				./src/philosopher_routine/actions/pick_forks.c \
				./src/philosopher_routine/actions/release_forks.c \
				./src/philosopher_routine/actions/take_a_nap.c \
				./src/philosopher_routine/actions/think.c 
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
