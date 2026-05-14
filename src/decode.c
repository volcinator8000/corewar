/*
** EPITECH PROJECT, 2026
** corewar
** File description:
** Argument decoding helpers
*/

#include "my.h"

int get_arg_type(int t)
{
    if (t == 1)
        return T_REG;
    if (t == 2)
        return T_DIR;
    return T_IND;
}

int decode_arg_mod(vm_t *vm, process_t *proc, int arg_type, int offset)
{
    int val = 0;
    int reg;

    if (arg_type == T_REG) {
        reg = vm->arena[(proc->pc + offset) % MEM_SIZE];
        if (reg >= 1 && reg <= REG_NUMBER)
            val = proc->registers[reg];
    } else if (arg_type == T_DIR) {
        val = read_mem_int(vm, proc->pc + offset, DIR_SIZE);
    } else if (arg_type == T_IND) {
        val = read_mem_int(vm, proc->pc + offset, IND_SIZE);
        val = read_mem_int(vm, proc->pc + (val % IDX_MOD), DIR_SIZE);
    }
    return val;
}

int decode_arg_at(vm_t *vm, process_t *proc, int arg_type, int offset)
{
    int val = 0;
    int reg;

    if (arg_type == T_REG) {
        reg = vm->arena[(proc->pc + offset) % MEM_SIZE];
        if (reg >= 1 && reg <= REG_NUMBER)
            val = proc->registers[reg];
    } else if (arg_type == T_DIR) {
        val = read_mem_int(vm, proc->pc + offset, DIR_SIZE);
    } else if (arg_type == T_IND) {
        val = read_mem_int(vm, proc->pc + offset, IND_SIZE);
        val = read_mem_int(vm, proc->pc + val, DIR_SIZE);
    }
    return val;
}
