//
// Created by JiangChenyang on 2021/10/16 12.
//

#ifndef EXP2_BUILD_EXPR_H
#define EXP2_BUILD_EXPR_H
#include "../LA/sign.h"
#include "../LA/hash.h"
#include "bits/stdc++.h"
/*
 * 生成表达式
 */

const int MAX_RIGHT_SIZE = 1000;
class build_expr : hash_interface{
public:
    char left[MAX_SIGN_SIZE]{};
    char right[MAX_RIGHT_SIZE][MAX_SIGN_SIZE]{};
    // 专门供哈希用，别问，问就是代码初期没有设计好
    char hash[MAX_RIGHT_SIZE * MAX_SIGN_SIZE + MAX_SIGN_SIZE]{};
    int size = 0;   // 产生式右部字符个数
    int ishashed = 0;

    char* get_hashobj() override;
    void hash_init();
    static void parse(build_expr&, const char*);
};

/*
 * 定义生成式检索表
 */
typedef hash_table<build_expr> bexpr_table;

#endif //EXP2_BUILD_EXPR_H
