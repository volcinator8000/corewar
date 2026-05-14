/*
** EPITECH PROJECT, 2026
** corewar
** File description:
** Entry point of the virtual machine
*/

#include <stdlib.h>
#include "my.h"

static void init_players(vm_t *vm)
{
    int i = 0;

    while (i < MAX_PLAYERS) {
        vm->players[i].number = -1;
        vm->players[i].load_addr = -1;
        vm->players[i].prog_size = 0;
        vm->players[i].last_live = 0;
        i++;
    }
}

static void init_arena(vm_t *vm)
{
    int i = 0;

    while (i < MEM_SIZE) {
        vm->arena[i] = 0;
        i++;
    }
}

static vm_t *init_vm(void)
{
    vm_t *vm = malloc(sizeof(vm_t));

    if (!vm)
        return NULL;
    init_arena(vm);
    init_players(vm);
    vm->nb_players = 0;
    vm->processes = NULL;
    vm->nb_processes = 0;
    vm->cycle = 0;
    vm->cycle_to_die = CYCLE_TO_DIE;
    vm->last_live = 0;
    vm->live_count = 0;
    vm->checks = 0;
    vm->dump_cycle = -1;
    vm->proc_id_counter = 1;
    return vm;
}

int main(int ac, char **av)
{
    vm_t *vm = init_vm();
    int ret;

    if (!vm) {
        display_error("Error: memory allocation failed\n");
        return ERR_CODE;
    }
    ret = parse_args(ac, av, vm);
    if (ret == 1) {
        free(vm);
        return SUCCESS;
    }
    if (ret != SUCCESS || load_champions(vm) != SUCCESS) {
        free(vm);
        return ERR_CODE;
    }
    run_vm(vm);
    free(vm);
    return SUCCESS;
}
