/*
** EPITECH PROJECT, 2026
** corewar
** File description:
** Memory operations: ld, st, lld, ldi, lldi
*/

#include "my.h"

void op_ld(vm_t *vm, process_t *proc)
{
    int t0 = ((vm->arena[(proc->pc + 1) % MEM_SIZE]) >> 6) & 0x3;
    int value;
    int rc;
    int raw;

    if (t0 == 2) {
        value = read_mem_int(vm, proc->pc + 2, DIR_SIZE);
        rc = vm->arena[(proc->pc + 6) % MEM_SIZE];
        proc->pc = (proc->pc + 7) % MEM_SIZE;
    } else {
        raw = read_mem_int(vm, proc->pc + 2, IND_SIZE);
        value = read_mem_int(vm, proc->pc + (raw % IDX_MOD), REG_SIZE);
        rc = vm->arena[(proc->pc + 4) % MEM_SIZE];
        proc->pc = (proc->pc + 5) % MEM_SIZE;
    }
    if (rc < 1 || rc > REG_NUMBER)
        return;
    proc->registers[rc] = value;
    proc->carry = (value == 0) ? 1 : 0;
}

static void st_to_reg(vm_t *vm, process_t *proc, int ra)
{
    int rb = vm->arena[(proc->pc + 3) % MEM_SIZE];

    if (rb >= 1 && rb <= REG_NUMBER)
        proc->registers[rb] = proc->registers[ra];
    proc->pc = (proc->pc + 4) % MEM_SIZE;
}

static void st_to_mem(vm_t *vm, process_t *proc, int ra)
{
    int raw = read_mem_int(vm, proc->pc + 3, IND_SIZE);
    int addr = proc->pc + (raw % IDX_MOD);

    write_mem_int(vm, addr, proc->registers[ra], REG_SIZE);
    proc->pc = (proc->pc + 5) % MEM_SIZE;
}

void op_st(vm_t *vm, process_t *proc)
{
    int t1 = ((vm->arena[(proc->pc + 1) % MEM_SIZE]) >> 4) & 0x3;
    int ra = vm->arena[(proc->pc + 2) % MEM_SIZE];

    if (ra < 1 || ra > REG_NUMBER) {
        proc->pc = (proc->pc + 5) % MEM_SIZE;
        return;
    }
    if (t1 == 1)
        st_to_reg(vm, proc, ra);
    else
        st_to_mem(vm, proc, ra);
}

void op_lld(vm_t *vm, process_t *proc)
{
    int t0 = ((vm->arena[(proc->pc + 1) % MEM_SIZE]) >> 6) & 0x3;
    int value;
    int rc;
    int raw;

    if (t0 == 2) {
        value = read_mem_int(vm, proc->pc + 2, DIR_SIZE);
        rc = vm->arena[(proc->pc + 6) % MEM_SIZE];
        proc->pc = (proc->pc + 7) % MEM_SIZE;
    } else {
        raw = read_mem_int(vm, proc->pc + 2, IND_SIZE);
        value = read_mem_int(vm, proc->pc + raw, REG_SIZE);
        rc = vm->arena[(proc->pc + 4) % MEM_SIZE];
        proc->pc = (proc->pc + 5) % MEM_SIZE;
    }
    if (rc < 1 || rc > REG_NUMBER)
        return;
    proc->registers[rc] = value;
    proc->carry = (value == 0) ? 1 : 0;
}

static int ldi_arg_size(int raw)
{
    if (raw == 1)
        return 1;
    return IND_SIZE;
}

void op_ldi(vm_t *vm, process_t *proc)
{
    int cb = vm->arena[(proc->pc + 1) % MEM_SIZE];
    int t0 = (cb >> 6) & 0x3;
    int t1 = (cb >> 4) & 0x3;
    int off = 2;
    int v0 = decode_arg_mod(vm, proc, get_arg_type(t0), off);
    int v1;
    int rc;

    off += ldi_arg_size(t0);
    v1 = decode_arg_mod(vm, proc, get_arg_type(t1), off);
    off += ldi_arg_size(t1);
    rc = vm->arena[(proc->pc + off) % MEM_SIZE];
    if (rc >= 1 && rc <= REG_NUMBER)
        proc->registers[rc] = read_mem_int(vm,
            proc->pc + ((v0 + v1) % IDX_MOD), REG_SIZE);
    proc->pc = (proc->pc + off + 1) % MEM_SIZE;
}

static int lldi_arg_size(int raw)
{
    if (raw == 1)
        return 1;
    if (raw == 3)
        return IND_SIZE;
    return DIR_SIZE;
}

void op_lldi(vm_t *vm, process_t *proc)
{
    int cb = vm->arena[(proc->pc + 1) % MEM_SIZE];
    int t0 = (cb >> 6) & 0x3;
    int t1 = (cb >> 4) & 0x3;
    int off = 2;
    int v0 = decode_arg_at(vm, proc, get_arg_type(t0), off);
    int v1;
    int rc;

    off += lldi_arg_size(t0);
    v1 = decode_arg_at(vm, proc, get_arg_type(t1), off);
    off += lldi_arg_size(t1);
    rc = vm->arena[(proc->pc + off) % MEM_SIZE];
    if (rc >= 1 && rc <= REG_NUMBER) {
        proc->registers[rc] = read_mem_int(vm, proc->pc + v0 + v1, REG_SIZE);
        proc->carry = (proc->registers[rc] == 0) ? 1 : 0;
    }
    proc->pc = (proc->pc + off + 1) % MEM_SIZE;
}
