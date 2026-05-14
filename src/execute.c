/*
** EPITECH PROJECT, 2026
** corewar
** File description:
** Instruction decode and dispatch
*/

#include "my.h"

static const op_func_t OP_FUNCS[] = {
    NULL, op_live, op_ld, op_st, op_add, op_sub,
    op_and, op_or, op_xor, op_zjmp, op_ldi, op_sti,
    op_fork, op_lld, op_lldi, op_lfork, op_print
};

static int raw_to_type(int raw)
{
    if (raw == 1)
        return T_REG;
    if (raw == 2)
        return T_DIR;
    if (raw == 3)
        return T_IND;
    return 0;
}

static int type_to_size(int type, int op_idx)
{
    int is_short = (op_idx == 9 || op_idx == 12 || op_idx == 15 ||
        op_idx == 10 || op_idx == 11 || op_idx == 14);

    if (type == T_REG)
        return 1;
    if (type == T_IND)
        return IND_SIZE;
    return is_short ? IND_SIZE : DIR_SIZE;
}

static int parse_coding_byte(int cb, int *types, int nb_args)
{
    int shift = 6;
    int i = 0;

    while (i < nb_args) {
        types[i] = raw_to_type((cb >> shift) & 0x3);
        if (types[i] == 0)
            return ERR_CODE;
        shift -= 2;
        i++;
    }
    return SUCCESS;
}

static int validate_types(int op_idx, int *types, int nb_args)
{
    int i = 0;

    while (i < nb_args) {
        if (!(op_tab[op_idx].type[i] & types[i]))
            return ERR_CODE;
        i++;
    }
    return SUCCESS;
}

static int compute_skip(int *types, int nb_args, int op_idx)
{
    int offset = 2;
    int i = 0;

    while (i < nb_args) {
        offset += type_to_size(types[i], op_idx);
        i++;
    }
    return offset;
}

static int is_no_coding_byte(int op_idx)
{
    return (op_idx == 1 || op_idx == 9 || op_idx == 12 || op_idx == 15);
}

static void do_execute_process(vm_t *vm, process_t *proc, int op_idx)
{
    int types[MAX_ARGS_NUMBER];
    int cb;
    int nb_args;
    int skip;

    nb_args = op_tab[op_idx].nbr_args;
    cb = vm->arena[(proc->pc + 1) % MEM_SIZE];
    if (parse_coding_byte(cb, types, nb_args) != SUCCESS
        || validate_types(op_idx, types, nb_args) != SUCCESS) {
        skip = compute_skip(types, nb_args, op_idx);
        proc->pc = (proc->pc + skip) % MEM_SIZE;
        return;
    }
    OP_FUNCS[op_idx](vm, proc);
}

void execute_process(vm_t *vm, process_t *proc)
{
    int op_idx = proc->current_op;

    if (op_idx < 1 || op_idx > 16 || !OP_FUNCS[op_idx]) {
        proc->pc = (proc->pc + 1) % MEM_SIZE;
        return;
    }
    if (is_no_coding_byte(op_idx)) {
        OP_FUNCS[op_idx](vm, proc);
        return;
    }
    do_execute_process(vm, proc, op_idx);
}
