//
// Created by JiangChenyang on 2021/10/18 15.
//

#ifndef EXP2_LR_TEST_H
#define EXP2_LR_TEST_H

#include "LR_table.h"
#include "../LA/LA.h"
#include "LR.h"

/*
 * 测试 LR1 action 表的解析
 */
void action_parse_test() {
    char action_path[] = "data/LR_data/action_table.csv";
    FILE* fd = fopen(action_path, "r");

    code_table c_table;
    init_code(c_table);

    bexpr_table b_table;
    b_table.clear();
    action_table a_table;
    a_table.add_table(&c_table, &b_table);

    a_table.parse(fd);
}

/*
 * 测试 LR1 goto 表的解析
 */
void goto_parse_test() {
    char goto_path[] = "data/LR_data/goto_table.csv";
    FILE* fd = fopen(goto_path, "r");

    // 定义非终结符
    const int NUM = 9;
    char m_signs[NUM][MAX_SIGN_SIZE] = {
            "S", "SENTENCE", "STATE", "ASSIGN", "TYPE",
            "EXPR", "CON", "A", "B"
    };
    msign_table m_table;

    m_table.clear();
    for (auto & m_sign : m_signs) {
        sign ms(m_sign);
        m_table.insert(ms);
    }

    goto_table g_table{};
    g_table.add_table(&m_table);
    g_table.parse(fd);
}

void LR_test() {
    char action_path[] = "data/LR_data/action_table.csv";
    char goto_path[] = "data/LR_data/goto_table.csv";
    char src_path[] = "data/LR_data/input_code.txt";
    char token_path[] = "data/LR_data/token.txt";
    char sign_path[] = "data/LR_data/sign.txt";
    char reduce_path[] = "data/LR_data/reduce.txt";
    char quad_path[] = "data/LR_data/quad.txt";
    FILE *ap_f = fopen(action_path, "r");
    FILE *gp_f = fopen(goto_path, "r");
    FILE *sp_f = fopen(src_path, "r");
    FILE *tp_f = fopen(token_path, "w");
    FILE *sgp_f = fopen(sign_path, "w");
    FILE *rp_f = fopen(reduce_path, "w");
    FILE *qp_f = fopen(quad_path, "w");

    code_table c_table;
    bexpr_table b_table;
    action_table a_table;
    goto_table g_table;
    msign_table m_table;

    init_code(c_table);
    LA la(sp_f, &c_table);
    la.analyze();
    la.print_token(tp_f);
    la.print_sign_table(sgp_f);
    fflush(tp_f);
    fflush(sgp_f);

    init_msign_table(m_table);
    a_table.add_table(&c_table, &b_table);
    a_table.parse(ap_f);
    g_table.add_table(&m_table);
    g_table.parse(gp_f);

    LR lr;
    lr.add(&a_table, &g_table, &la.s_table, &m_table, &(la.tok));
    lr.analysis();
    lr.print_reduce_seq(rp_f);
    lr.print_quad_table(qp_f);
}

#endif //EXP2_LR_TEST_H
