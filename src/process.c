/*
** EPITECH PROJECT, 2026
** corewar
** File description:
** Process creation, linking, and removal
*/

#include <stdlib.h>
#include "my.h"

process_t *create_process(int pc, int player_id, int id, vm_t *vm)
{
    process_t *proc = malloc(sizeof(process_t));

    (void)vm;
    if (!proc)
        return NULL;
    proc->id = id;
    proc->player_id = player_id;
    proc->pc = ((pc % MEM_SIZE) + MEM_SIZE) % MEM_SIZE;
    for (int i = 0; i <= REG_NUMBER; i++)
        proc->registers[i] = 0;
    proc->carry = 0;
    proc->live_count = 0;
    proc->wait_cycles = 0;
    proc->current_op = 0;
    proc->next = NULL;
    return proc;
}

void add_process(vm_t *vm, process_t *proc)
{
    process_t *cur;

    if (!proc)
        return;
    if (!vm->processes) {
        vm->processes = proc;
        vm->nb_processes++;
        return;
    }
    cur = vm->processes;
    while (cur->next)
        cur = cur->next;
    cur->next = proc;
    vm->nb_processes++;
}

static process_t *handle_dead_process(vm_t *vm, process_t *cur, process_t *prev)
{
    process_t *next = cur->next;

    if (prev)
        prev->next = next;
    else
        vm->processes = next;
    free(cur);
    vm->nb_processes--;
    return prev;
}

void remove_dead_processes(vm_t *vm)
{
    process_t *cur = vm->processes;
    process_t *prev = NULL;
    process_t *next;

    while (cur) {
        next = cur->next;
        if (cur->live_count == 0) {
            prev = handle_dead_process(vm, cur, prev);
        } else {
            cur->live_count = 0;
            prev = cur;
        }
        cur = next;
    }
}
