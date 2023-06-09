# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lucaslefrancq <lucaslefrancq@student.42    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/20 12:10:01 by lucaslefran       #+#    #+#              #
#    Updated: 2023/06/09 16:14:02 by lucaslefran      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

CC = gcc

# lib
LIB_NAME = ft_malloc
LIB_LINK_NAME = lib$(LIB_NAME).so
LIB_FULL_NAME = lib$(LIB_NAME)_$(HOSTTYPE).so
LIB_FLAGS = -fPIC -g -Wall -Werror -Wextra

LIB_SRCS = free.c malloc.c realloc.c show_alloc_mem.c lmmap.c chunk.c
LIB_OBJS = $(LIB_SRCS:.c=.o)
LIB_PATH = src/
LIB_HEADS = allocator.h lmmap.h chunk.h

# tester
TEST_FLAGS = -g -Wall -Werror -Wextra
TEST_NAME = tester

TEST_SRCS = main.c utils.c test_lmmap.c test_chunk.c test_show_alloc_mem.c \
		test_malloc.c test_malloc_tiny.c test_malloc_small.c test_malloc_large.c
TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_PATH = test/


all: $(LIB_NAME)

$(addprefix $(LIB_PATH), $(LIB_OBJS)): $(addprefix $(LIB_PATH), $(LIB_HEADS))

# Building lib
$(LIB_NAME): $(addprefix $(LIB_PATH), $(LIB_OBJS))
	$(CC) -shared -o $(LIB_FULL_NAME) $(LIB_FLAGS) $(addprefix $(LIB_PATH), $(LIB_OBJS))
	ln -sf $(LIB_FULL_NAME) $(LIB_LINK_NAME)

# Building tester
$(TEST_NAME): $(LIB_NAME) $(addprefix $(TEST_PATH), $(TEST_OBJS))
	$(CC) -o $(TEST_NAME) $(TEST_FLAGS) $(addprefix $(TEST_PATH), $(TEST_SRCS)) \
	-L. -Wl,-rpath $(PWD) -l$(LIB_NAME)

clean:
	rm -rf $(addprefix $(LIB_PATH), $(LIB_OBJS)) $(addprefix $(TEST_PATH), $(TEST_OBJS))

fclean: clean
	rm -rf $(LIB_FULL_NAME) $(LIB_LINK_NAME) $(TEST_NAME)

re: fclean all

re_tester: fclean tester

.PHONY:	all clean fclean re re_tester

#compiling .o with flags
$(LIB_PATH)%.o: $(LIB_PATH)%.c
		$(CC) $(LIB_FLAGS) -o $@ -c $<

#compiling .o with flags
$(TEST_PATH)%.o: $(TEST_PATH)%.c
		$(CC) $(TEST_FLAGS) -o $@ -c $<