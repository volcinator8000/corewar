/*
** EPITECH PROJECT, 2026
** corewar
** File description:
** parse_args.c
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "my.h"

static void print_usage(int fd)
{
    if (fd == 1) {
        mini_printf("USAGE\n./corewar [-dump nbr_cycle] ");
        mini_printf("[[-n prog_number] [-a load_address] prog_name]...\n");
        mini_printf("DESCRIPTION\n");
        mini_printf(" -dump nbr_cycle dumps the VM state after nbr_cycle\n");
        mini_printf(" -n prog_number sets the next program's number\n");
        mini_printf(" -a load_address sets the next program's load address\n");
    }
    if (fd == 2) {
        display_error("USAGE\n./corewar [-dump nbr_cycle] ");
        display_error("[[-n prog_number] [-a load_address] prog_name]...\n");
        display_error("DESCRIPTION\n");
        display_error(" -dump nbr_cycle dumps the VM state after nbr_cycle\n");
        display_error(" -n prog_number sets the next program's number\n");
        display_error(" -a load_address sets the next program's load address\n");
    }
}

static int handle_dump(int i, int ac, char **av, vm_t *vm)
{
    if (i + 1 >= ac) {
        display_error("Error: -dump requires an argument\n");
        return ERR_CODE;
    }
    vm->dump_cycle = my_atoi(av[i + 1]);
    return SUCCESS;
}

static int handle_n(int i, int ac, char **av, vm_t *vm)
{
    int nb;

    if (i + 1 >= ac) {
        display_error("Error: -n requires an argument\n");
        return ERR_CODE;
    }
    nb = my_atoi(av[i + 1]);
    if (nb < 1 || nb > MAX_PLAYERS) {
        display_error("Error: invalid player number");
        return ERR_CODE;
    }
    vm->players[vm->nb_players].number = nb;
    return SUCCESS;
}

static int handle_a(int i, int ac, char **av, vm_t *vm)
{
    if (i + 1 >= ac) {
        display_error("Error: -a requires an argument\n");
        return ERR_CODE;
    }
    vm->players[vm->nb_players].load_addr = my_atoi(av[i + 1]) % MEM_SIZE;
    return SUCCESS;
}

static int add_champion(char *path, vm_t *vm)
{
    int idx = vm->nb_players;

    if (idx >= MAX_PLAYERS) {
        display_error("Error: too many champions\n");
        return ERR_CODE;
    }
    if (vm->players[idx].number == -1)
        vm->players[idx].number = idx + 1;
    my_strncpy(vm->players[idx].name, path, PROG_NAME_LENGTH);
    vm->nb_players++;
    return SUCCESS;
}

static int process_arg_flag(int *i, int ac, char **av, vm_t *vm)
{
    if (my_strcmp(av[*i], "-dump") == 0)
        return handle_dump(*i, ac, av, vm);
    if (my_strcmp(av[*i], "-n") == 0)
        return handle_n(*i, ac, av, vm);
    if (my_strcmp(av[*i], "-a") == 0)
        return handle_a(*i, ac, av, vm);
    return -1;
}

static int process_arg(int *i, int ac, char **av, vm_t *vm)
{
    int ret = process_arg_flag(i, ac, av, vm);

    if (ret != -1) {
        if (ret != SUCCESS)
            return ERR_CODE;
        *i += 2;
        return SUCCESS;
    }
    if (add_champion(av[*i], vm) != SUCCESS)
        return ERR_CODE;
    *i += 1;
    return SUCCESS;
}

int parse_args(int ac, char **av, vm_t *vm)
{
    int i = 1;

    if (ac == 2 && my_strcmp(av[1], "-h") == 0) {
        print_usage(1);
        return 1;
    }
    while (i < ac) {
        if (process_arg(&i, ac, av, vm) != SUCCESS)
            return ERR_CODE;
    }
    if (vm->nb_players == 0) {
        print_usage(2);
        return ERR_CODE;
    }
    return SUCCESS;
}
