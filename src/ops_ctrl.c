/*
** EPITECH PROJECT, 2026
** corewar
** File description:
** Control operations: live, zjmp, fork, lfork, print
*/

#include "my.h"

static void announce_live(vm_t *vm, int abs_id)
{
    int i = 0;

    while (i < vm->nb_players) {
        if (vm->players[i].number == abs_id) {
            vm->players[i].last_live = vm->cycle;
            mini_printf("The player %d (%s) is alive.\n",
                abs_id, vm->players[i].name);
        }
        i++;
    }
}

void op_live(vm_t *vm, process_t *proc)
{
    int player_id = read_mem_int(vm, proc->pc + 1, DIR_SIZE);
    int abs_id = -player_id;

    proc->live_count++;
    vm->live_count++;
    vm->last_live = abs_id;
    announce_live(vm, abs_id);
    proc->pc = (proc->pc + 5) % MEM_SIZE;
}

void op_zjmp(vm_t *vm, process_t *proc)
{
    int offset = read_mem_int(vm, proc->pc + 1, IND_SIZE);

    (void)vm;
    if (proc->carry == 1) {
        proc->pc = (proc->pc + (offset % IDX_MOD) + MEM_SIZE) % MEM_SIZE;
    } else {
        proc->pc = (proc->pc + 3) % MEM_SIZE;
    }
}

static void copy_registers(process_t *dst, process_t *src)
{
    int i = 1;

    while (i <= REG_NUMBER) {
        dst->registers[i] = src->registers[i];
        i++;
    }
}

void op_fork(vm_t *vm, process_t *proc)
{
    int offset = read_mem_int(vm, proc->pc + 1, IND_SIZE);
    int new_pc = (proc->pc + (offset % IDX_MOD) + MEM_SIZE) % MEM_SIZE;
    process_t *child = create_process(new_pc, proc->player_id,
        vm->proc_id_counter, vm);

    if (!child) {
        proc->pc = (proc->pc + 3) % MEM_SIZE;
        return;
    }
    copy_registers(child, proc);
    child->carry = proc->carry;
    vm->proc_id_counter++;
    add_process(vm, child);
    proc->pc = (proc->pc + 3) % MEM_SIZE;
}

void op_lfork(vm_t *vm, process_t *proc)
{
    int offset = read_mem_int(vm, proc->pc + 1, IND_SIZE);
    int new_pc = (proc->pc + offset + MEM_SIZE) % MEM_SIZE;
    process_t *child = create_process(new_pc, proc->player_id,
        vm->proc_id_counter, vm);

    if (!child) {
        proc->pc = (proc->pc + 3) % MEM_SIZE;
        return;
    }
    copy_registers(child, proc);
    child->carry = proc->carry;
    vm->proc_id_counter++;
    add_process(vm, child);
    proc->pc = (proc->pc + 3) % MEM_SIZE;
}

void op_print(vm_t *vm, process_t *proc)
{
    int ra = vm->arena[(proc->pc + 1) % MEM_SIZE];
    int val;

    (void)vm;
    if (ra >= 1 && ra <= REG_NUMBER) {
        val = proc->registers[ra] % 256;
        if (val < 0)
            val += 256;
        mini_printf("%c", (char)val);
    }
    proc->pc = (proc->pc + 2) % MEM_SIZE;
}
