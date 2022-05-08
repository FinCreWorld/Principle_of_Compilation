//
// Created by JiangChenyang on 2021/10/19 17.
//

#include "LR.h"
void LR::add(action_table *a, goto_table *g, sign_table *s, msign_table *m, token *t) {
    a_table = a;
    g_table = g;
    s_table = s;
    m_table = m;
    t_table = t;
}


/*
 * 给定压入状态与符号位置，使其入栈
 */
void LR::shift(int status, int sign_pos) {
    sign sg = s_table->table[sign_pos];
    s_stack.push(status_n_sign(status, sg));
}


/*
 *
 */
void LR::reduce(int bexpr_num) {
    reduce_seq.push_back(bexpr_num);
    build_expr bexpr = a_table->b_table->table[bexpr_num];

    // 执行归约动作
    int left_sign_pos = m_table->find(bexpr.left);
    sign left_sign = m_table->table[left_sign_pos]; // 得到将要载入的非终结符
    if (left_sign.af) {
        left_sign.af(&q_table, &s_stack, &bexpr, &left_sign, s_table);
    }

    int bsize = bexpr.size;
    for (int i = bsize-1; i >= 0; --i) {
        s_stack.pop();
    }

    int status = g_table->get_goto(s_stack.top().first, left_sign.code);
    if (status == -1) {
        printf("It seems you have read an empty goto\n");
        printf("stack top status: %d\nleft sign: %s\nleft sign code: %d\n",
               s_stack.top().first, left_sign.sign_name, left_sign.code);
        system("pause");exit(-1);
    }
    s_stack.push(status_n_sign(status, left_sign));
}

void LR::wrong() {
    printf("There seems to appear an unrecognized status\n");
    system("pause");exit(-1);
}

void LR::accept() {
    printf("Congratulation! You have finished all the program analysis!\n");
}

void LR::analysis() {
    sign end_sign("$", END, nullptr);
    s_stack.push(status_n_sign(0, end_sign));
    int i = 0;
     while (i < t_table->size()) {
        PAIR t = (*t_table)[i];
        int status = s_stack.top().first;
        action act = a_table->get_action(status, (int)t.first);
        if (act.num == -1 && act.action != 'a') {
            printf("It seems you have read an empty action\n");
            printf("stack top status: %d\nvalue type: %d\n", status, (int)t.first);
            system("pause");exit(-1);
        }
        switch (act.action) {
            case 's':
                ++i;
                shift(act.num, (int)t.second);
                break;
            case 'r':
                reduce(act.num);
                break;
            case 'a':
                ++i;
                accept();
                break;
            default:
                wrong();
        }
    }
}

void LR::print_reduce_seq(FILE *fd) {
    for (auto &v : reduce_seq) {
        build_expr bexpr = a_table->b_table->table[v];
        fprintf(fd, "%s -> ", bexpr.left);
        for (int i = 0; i < bexpr.size; ++i)
            fprintf(fd, "%s ", bexpr.right[i]);
        fprintf(fd, "\n");
        // 下方这条语句不具有普适性
        // 仅可用于当前特定文法
        if (strcmp(bexpr.right[bexpr.size-1], "semic") == 0)
            fprintf(fd, "\nAccept a sentence\n\n");
    }
    fprintf(fd, "\nFinish all\n\n");
}

void LR::print_quad_table(FILE *fd) {
    for (auto &v : q_table) {
        fprintf(fd, "%10s%10s%10s%10s\n", v.s1, v.s2, v.s3, v.s4);
    }
}

LR::LR() {
    a_table = nullptr;
    g_table = nullptr;
    s_table = nullptr;
    m_table = nullptr;
    t_table = nullptr;
    while (!s_stack.empty()) s_stack.pop();
    reduce_seq.clear();
}
