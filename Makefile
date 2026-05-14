##
## EPITECH PROJECT, 2025
## corewar
## File description:
## Makefile
##

SRC = mini_printf/my_putchar.c \
      mini_printf/my_putstr.c \
      mini_printf/my_put_nbr.c \
      mini_printf/my_strlen.c \
      mini_printf/mini_printf.c \
      src/dump.c \
      src/execute.c \
      src/load_champions.c \
      src/main.c \
      src/memory.c \
      src/op.c \
      src/ops_arith.c \
      src/ops_ctrl.c \
      src/ops_logic.c \
      src/ops_mem.c \
      src/ops_mem2.c \
      src/parse_args.c \
      src/process.c \
      src/run_vm.c \
      src/decode.c \
      lib/my_strcmp.c \
      lib/my_strncpy.c \
      lib/display_error.c \
      lib/hexa_converter.c \
      lib/my_atoi.c \

OBJ = $(SRC:.c=.o)

NAME = corewar

CFLAGS = -Wall -Wextra -I./include

all: $(NAME)

$(NAME): $(OBJ)
	epiclang -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
