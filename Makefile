ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so
LIBRARY_NAME = libft_malloc.so

LIBFTPRINTF = libftprintf.a
LIBFTPRINTF_PATH = ./

FLAGS = -Wall -Wextra -Werror
FLAGS_LIBRARY = -shared



SRC = block.c free.c heap_header.c malloc.c show_alloc.c utils.c
OBJ = $(SRC:.c=.o)


all: $(NAME)

$(NAME): $(OBJ)
	gcc $(FLAGS) $(FLAGS_LIBRARY) -L$(LIBFTPRINTF_PATH) -lftprintf -o $(NAME) $(OBJ) 
	ln -s $(NAME) $(LIBRARY_NAME)

%.o: %.c
	gcc $(FLAGS) -I$(LIBFTPRINTF_PATH) -o $@ -c $< -fPIC

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) $(LIBRARY_NAME)

re: fclean all

.PHONY: all clean fclean re