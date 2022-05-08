//
// Created by JiangChenyang on 2021/10/14 13.
//

#ifndef EXP2_LR_TABLE_H
#define EXP2_LR_TABLE_H

#include <bits/stdc++.h>
#include "../LA/sign.h"
#include "build_expr.h"

/*
 * 定义非终结符编码
 */
#define SENTENCE 0
#define STATE 1
#define TYPE 2
#define IDS 3
#define ASSIGN 4
#define EXPR 5
#define TT 6
#define FF 7
#define SENTENCES 8
#define M 9
#define N 10
#define P 11
#define MSIGN_NUM 12

/*
 * 定义动作表中的动作
 * 约定 action = 0, num = -1 时无操作
 */
class action {
public:
    char action;    // action = "r" 表示归约，"s" 表示移入
    int  num;       // action = "r" 时，表示第 num 个产生式
                    // action = "s" 时，表示移入后加入的状态
};

/*
 * 定义动作表
 */
const int END_SIGN_NUM = 100;
const int STATUS_NUM = 200;
class action_table {
private:
    // 链接到的编码表
    int status_num;
    code_table * c_table;
    action actions[STATUS_NUM][END_SIGN_NUM]{};

    /*
     * 解析状态表行首
     */
    static inline int read_head(FILE*, char[]);

    /*
     * 解析状态表主体部分
     */
    inline int read_body(FILE* fd, action&) const;
public:
    action_table();
    bexpr_table * b_table{};
    /*
     * 链接编码表
     */
    void add_table(code_table * c_table, bexpr_table *b_table);
    /*
     * 给定文件描述符，读入文件，并解析出动作表
     */
    void parse(FILE* fd);
    /*
     * 给定当前状态与终结符在符号表中位置，返回一个 action
     */
    action get_action(int, int);
};

typedef hash_table<sign> msign_table;
void init_msign_table(msign_table& m_table);

/*
 * 定义 GOTO 表
 */
const int MID_SIGN_NUM = 200;
class goto_table {
private:
    int status_num;
    // 连接到的非终结符表
    msign_table * m_table;
    int gotos[STATUS_NUM][MID_SIGN_NUM]{};
public:
    goto_table();
    /*
     * 链接非终结符表
     */
    void add_table(msign_table * m_table);
    /*
     * 解析 goto 表行首
     */
    static inline int read_head(FILE *, char[]);
    /*
     * 解析 goto 表主体部分
     */
    static inline int read_body(FILE *, int&);
    /*
     * 给定文件描述符，解析文件中的 goto 表
     */
    void parse(FILE* fd);
    /*
     * 根据状态编码与非终结符得到入栈状态
     */
    int get_goto(int, int);
};

#endif //EXP2_LR_TABLE_H
