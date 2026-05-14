/*
** EPITECH PROJECT, 2026
** corewar
** File description:
** Memory operations continued: sti
*/

#include "my.h"

static int sti_arg_size(int raw)
{
    if (raw == 1)
        return 1;
    return IND_SIZE;
}

void op_sti(vm_t *vm, process_t *proc)
{
    int cb = vm->arena[(proc->pc + 1) % MEM_SIZE];
    int t1 = (cb >> 4) & 0x3;
    int t2 = (cb >> 2) & 0x3;
    int ra = vm->arena[(proc->pc + 2) % MEM_SIZE];
    int off = 3;
    int v1;
    int v2;
    int addr;

    if (ra < 1 || ra > REG_NUMBER) {
        proc->pc = (proc->pc + 6) % MEM_SIZE;
        return;
    }
    v1 = decode_arg_mod(vm, proc, get_arg_type(t1), off);
    off += sti_arg_size(t1);
    v2 = decode_arg_mod(vm, proc, get_arg_type(t2), off);
    off += sti_arg_size(t2);
    addr = proc->pc + ((v1 + v2) % IDX_MOD);
    write_mem_int(vm, addr, proc->registers[ra], REG_SIZE);
    proc->pc = (proc->pc + off) % MEM_SIZE;
}
