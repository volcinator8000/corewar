/*
** EPITECH PROJECT, 2026
** corewar
** File description:
** Main virtual machine loop
*/

#include "my.h"

static void check_liveness(vm_t *vm)
{
    if (vm->checks < vm->cycle_to_die)
        return;
    if (vm->live_count >= NBR_LIVE)
        vm->cycle_to_die -= CYCLE_DELTA;
    remove_dead_processes(vm);
    vm->live_count = 0;
    vm->checks = 0;
}

static void announce_winner(vm_t *vm)
{
    int winner = vm->last_live;
    int i = 0;

    while (i < vm->nb_players) {
        if (vm->players[i].number == winner) {
            mini_printf("The player %d (%s) has won.\n",
                winner, vm->players[i].name);
            return;
        }
        i++;
    }
    mini_printf("Player %d has won.\n", winner);
}

static void tick_process(vm_t *vm, process_t *cur)
{
    int op_byte;

    if (cur->wait_cycles > 0) {
        cur->wait_cycles--;
        return;
    }
    op_byte = vm->arena[cur->pc % MEM_SIZE];
    cur->current_op = op_byte;
    if (op_byte >= 1 && op_byte <= 16)
        cur->wait_cycles = op_tab[op_byte].nbr_cycles - 1;
    execute_process(vm, cur);
}

static void run_one_cycle(vm_t *vm)
{
    process_t *cur = vm->processes;
    process_t *snapshot_end = NULL;
    process_t *tmp = vm->processes;

    while (tmp && tmp->next)
        tmp = tmp->next;
    snapshot_end = tmp;
    while (cur) {
        tick_process(vm, cur);
        if (cur == snapshot_end)
            break;
        cur = cur->next;
    }
    vm->cycle++;
    vm->checks++;
}

static int should_dump(vm_t *vm)
{
    return (vm->dump_cycle != -1 && vm->cycle >= vm->dump_cycle);
}

void run_vm(vm_t *vm)
{
    while (vm->nb_processes > 0) {
        run_one_cycle(vm);
        if (should_dump(vm)) {
            dump_state(vm);
            return;
        }
        check_liveness(vm);
    }
    announce_winner(vm);
}
