//
// Created by JiangChenyang on 2021/10/19 17.
//

#ifndef EXP2_LR_H
#define EXP2_LR_H
#include "LR_table.h"
#include "bits/stdc++.h"
#include "../LA/tool.h"
#include "../LA/sign.h"
#include "../LA/buffer.h"
#include "action_fun.h"

typedef pair<int, sign> status_n_sign;
typedef my_stack<status_n_sign> stasign_stack;


class LR {  // 继承自缓冲区，自带一个缓冲区
private:
    action_table *a_table;    // 链接到到的动作表
    goto_table *g_table;      // 链接到的转移表
    sign_table *s_table;      // 符号表
    msign_table *m_table;     // 非终结符表
    token *t_table;           // token 序列
    stasign_stack s_stack;      // 状态/符号栈
    vector<int> reduce_seq;     // 生成式序列
    void shift(int, int);
    void reduce(int);
    static void wrong();
    static void accept();
public:
    quad_table q_table;
    LR();
    void add(action_table*, goto_table*, sign_table*, msign_table*, token*);
    void analysis();
    void print_reduce_seq(FILE *fd);
    void print_quad_table(FILE *fd);
};


#endif //EXP2_LR_H
