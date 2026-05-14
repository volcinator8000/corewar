/*
** EPITECH PROJECT, 2026
** corewar
** File description:
** Logic operations: and, or, xor
*/

#include "my.h"

static int raw_to_arg_type(int raw)
{
    if (raw == 1)
        return T_REG;
    if (raw == 2)
        return T_DIR;
    return T_IND;
}

static int raw_to_size(int raw)
{
    if (raw == 1)
        return 1;
    if (raw == 3)
        return IND_SIZE;
    return DIR_SIZE;
}

static int apply_op(int va, int vb, int op)
{
    if (op == 0)
        return va & vb;
    if (op == 1)
        return va | vb;
    return va ^ vb;
}

static void do_logic_op(vm_t *vm, process_t *proc, int op)
{
    int cb = vm->arena[(proc->pc + 1) % MEM_SIZE];
    int t0 = (cb >> 6) & 0x3;
    int t1 = (cb >> 4) & 0x3;
    int off0 = 2;
    int off1 = off0 + raw_to_size(t0);
    int off2 = off1 + raw_to_size(t1);
    int va = decode_arg_mod(vm, proc, raw_to_arg_type(t0), off0);
    int vb = decode_arg_mod(vm, proc, raw_to_arg_type(t1), off1);
    int rc = vm->arena[(proc->pc + off2) % MEM_SIZE];
    int result;

    if (rc < 1 || rc > REG_NUMBER) {
        proc->pc = (proc->pc + off2 + 1) % MEM_SIZE;
        return;
    }
    result = apply_op(va, vb, op);
    proc->registers[rc] = result;
    proc->carry = (result == 0) ? 1 : 0;
    proc->pc = (proc->pc + off2 + 1) % MEM_SIZE;
}

void op_and(vm_t *vm, process_t *proc)
{
    do_logic_op(vm, proc, 0);
}

void op_or(vm_t *vm, process_t *proc)
{
    do_logic_op(vm, proc, 1);
}

void op_xor(vm_t *vm, process_t *proc)
{
    do_logic_op(vm, proc, 2);
}
