//
// Created by JiangChenyang on 2021/11/11 22.
//

#include "CG_test.h"
void CG_test() {
    char action_path[] = "data/CG_data/action_table.csv";
    char goto_path[] = "data/CG_data/goto_table.csv";
    char src_path[] = "data/CG_data/input_code.txt";
    char token_path[] = "data/CG_data/token.txt";
    char sign_path[] = "data/CG_data/sign.txt";
    char reduce_path[] = "data/CG_data/reduce.txt";
    char quad_path[] = "data/CG_data/quad.txt";
    char inst_path[] = "data/CG_data/inst.asm";
    FILE *ap_f = fopen(action_path, "r");
    FILE *gp_f = fopen(goto_path, "r");
    FILE *sp_f = fopen(src_path, "r");
    FILE *tp_f = fopen(token_path, "w");
    FILE *sgp_f = fopen(sign_path, "w");
    FILE *rp_f = fopen(reduce_path, "w");
    FILE *qp_f = fopen(quad_path, "w");
    FILE *ip_f = fopen(inst_path, "w");

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

    CG cg;
    cg.add(&lr.q_table);
    cg.init();
    cg.analysis();
    cg.print_inst_seq(ip_f);
}

