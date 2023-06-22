# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llefranc <llefranc@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/21 17:32:55 by llefranc          #+#    #+#              #
#    Updated: 2023/06/22 15:54:09 by llefranc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
FLAGS = -g -Wall -Werror -Wextra

all:
	$(CC) $(FLAGS) -o test0 test0.c ../src/free.c ../src/malloc.c ../src/realloc.c ../src/show_alloc_mem.c ../src/lmmap.c ../src/chunk.c ../src/mutex.c ../src/utils.c
	$(CC) $(FLAGS) -o test1 test1.c ../src/free.c ../src/malloc.c ../src/realloc.c ../src/show_alloc_mem.c ../src/lmmap.c ../src/chunk.c ../src/mutex.c ../src/utils.c
	$(CC) $(FLAGS) -o test2 test2.c ../src/free.c ../src/malloc.c ../src/realloc.c ../src/show_alloc_mem.c ../src/lmmap.c ../src/chunk.c ../src/mutex.c ../src/utils.c

fclean:
	rm test0 test1 test2