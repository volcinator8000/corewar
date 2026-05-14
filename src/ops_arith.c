/*
** EPITECH PROJECT, 2026
** corewar
** File description:
** Arithmetic operations: add, sub
*/

#include "my.h"

static int get_reg(vm_t *vm, process_t *proc, int off)
{
    return (int)vm->arena[(proc->pc + off) % MEM_SIZE];
}

static int regs_valid(int ra, int rb, int rc)
{
    return (ra >= 1 && ra <= REG_NUMBER
        && rb >= 1 && rb <= REG_NUMBER
        && rc >= 1 && rc <= REG_NUMBER);
}

void op_add(vm_t *vm, process_t *proc)
{
    int ra = get_reg(vm, proc, 2);
    int rb = get_reg(vm, proc, 3);
    int rc = get_reg(vm, proc, 4);
    int result;

    if (!regs_valid(ra, rb, rc)) {
        proc->pc = (proc->pc + 5) % MEM_SIZE;
        return;
    }
    result = proc->registers[ra] + proc->registers[rb];
    proc->registers[rc] = result;
    proc->carry = (result == 0) ? 1 : 0;
    proc->pc = (proc->pc + 5) % MEM_SIZE;
}

void op_sub(vm_t *vm, process_t *proc)
{
    int ra = get_reg(vm, proc, 2);
    int rb = get_reg(vm, proc, 3);
    int rc = get_reg(vm, proc, 4);
    int result;

    if (!regs_valid(ra, rb, rc)) {
        proc->pc = (proc->pc + 5) % MEM_SIZE;
        return;
    }
    result = proc->registers[ra] - proc->registers[rb];
    proc->registers[rc] = result;
    proc->carry = (result == 0) ? 1 : 0;
    proc->pc = (proc->pc + 5) % MEM_SIZE;
}
