//
// Created by JiangChenyang on 2021/11/11 19.
//

/*
 * 生成目标代码
 */
#ifndef EXP2_CG_H
#define EXP2_CG_H

#include "../LA/hash.h"
#include "../LA/sign.h"

const int MAX_NAME_SIZE = 100;
const int REG_NUM = 4;

// 用于存储变量
class variable : hash_interface{
public:
    char name[MAX_NAME_SIZE]{};
    int  r; // r = -1 表示不在寄存器中，否则表示在第 r 个寄存器中
    int refcnt; // 用来表示其还剩下的引用次数
    int p;  // 指示当前引用位置
    vector<int> pos;    // 存储所有被引用到的位置
    char *get_hashobj() override;
    variable();
    explicit variable(char s[]);
};

// 名字地址描述表
typedef hash_table<variable> variable_table;

// 指令
typedef struct instruction {
    char op[10]{};    // 操作符
    char s1[10]{};    // 操作对象 1
    char s2[10]{};    // 操作对象 2
    instruction(const char *op, const char *s1, const char *s2) {
        strncpy(this->op, op, 10);
        strncpy(this->s1, s1, 10);
        strncpy(this->s2, s2, 10);
    }
} instruction;

const char reg_name[4][10] = {"ax", "bx", "cx", "dx"};
class CG {
private:
    // 寄存器描述表
    int reg[REG_NUM]{-1, -1, -1, -1}; // reg[0] = ax, reg[1] = bx, reg[2] = cx, reg[4] = dx
    // 名字地址描述表
    variable_table v_table;
    // 指令缓存
    quad_table *q_table{};
    // 指令序列
    vector<instruction> inst_seq;
    // 将传入的变量名生成 variable，并加入 v_table，更新引用信息
    void insert_v_table(int mccnt, char s[]);
    // 返回一个可用的寄存器
    int get_reg(char *p1, char *res);
    // 更新变量内容
    void updatev(char *p1, char *p2, char *res);
    // 生成 MOV 指令
    void move_inst(char *p1, char *res);
    // 生成二元运算指令
    void binop_inst(char *op, char *p1, char *p2, char *res);
    // 错误处理
    static void wrong(char *s);
public:
    CG();
    void add(quad_table* qt);
    void init();
    void analysis();
    void print_inst_seq(FILE *fd);
};


#endif //EXP2_CG_H
