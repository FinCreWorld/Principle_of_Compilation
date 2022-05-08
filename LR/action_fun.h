//
// Created by JiangChenyang on 2021/11/9 19.
//

#ifndef EXP2_ACTION_FUN_H
#define EXP2_ACTION_FUN_H
#include "../LA/sign.h"
#include "../LA/tool.h"
#include "build_expr.h"
#include "string"

typedef pair<int, sign> status_n_sign;
typedef my_stack<status_n_sign> stasign_stack;


static long long offset = 0;
static int name_cnt = 0;

void gencode(quad_table *q_table, const char *op, const char *addr1, const char *addr2, const char *result);

char* newtemp();

void wrong(const string& s);

// 对 M 进行归约
void m_fun(__attribute__((unused))quad_table *q_table, stasign_stack * sstack, __attribute__((unused)) build_expr * be, sign * sg, __attribute__((unused))sign_table * s_table);

// 对 TYPE 进行归约
void type_fun(__attribute__((unused))quad_table *q_table, __attribute__((unused))stasign_stack * sstack, __attribute__((unused))build_expr * be, sign * sg, __attribute__((unused))sign_table * s_table);

// 对 IDS 进行归约
void ids_fun(__attribute__((unused))quad_table *q_table, stasign_stack * sstack, build_expr * be, __attribute__((unused))sign * sg, sign_table * s_table);

// 对 N 进行归约
void n_fun(__attribute__((unused))quad_table *q_table, stasign_stack * sstack, __attribute__((unused))build_expr * be, sign * sg, __attribute__((unused))sign_table * s_table);

// 对 P 进行归约
void p_fun(__attribute__((unused))quad_table *q_table, stasign_stack * sstack, __attribute__((unused))build_expr * be, sign * sg, __attribute__((unused))sign_table * s_table);

// 对 ASSIGN 进行归约
void assign_fun(quad_table *q_table, stasign_stack * sstack, __attribute__((unused))build_expr * be, __attribute__((unused))sign * sg, __attribute__((unused))sign_table * s_table);

// 对 EXPR 进行归约
void expr_fun(quad_table *q_table, stasign_stack * sstack, build_expr * be, sign * sg, __attribute__((unused))sign_table * s_table);

// 对 TT 进行归约
void tt_fun(quad_table *q_table, stasign_stack * sstack, build_expr * be, sign * sg, __attribute__((unused))sign_table * s_table);

// 对 FF 进行归约
void ff_fun(quad_table *q_table, stasign_stack * sstack, build_expr * be, sign * sg, __attribute__((unused))sign_table * s_table);

#endif //EXP2_ACTION_FUN_H
