//
// Created by JiangChenyang on 2021/10/19 11.
//

#include "build_expr.h"

/*
 * 初始化内部 hash 数组
 */
void build_expr::hash_init() {
    if (ishashed) return;
    ishashed = 1;

    char *p = hash;
    strcpy(p, left);
    p += strlen(p);

    for (int i = 0; i < size; ++i) {
        strcpy(p, right[i]);
        p += strlen(right[i]);
    }
}

/*
 * 实现接口
 */
char *build_expr::get_hashobj() {
    hash_init();
    return hash;
}

/*
 * 解析生成式
 */
void build_expr::parse(build_expr &be, const char * s) {
    char *p = const_cast<char *>(s + 7);    // 跳转到 "reduce " 之后
    int i, j = 0;
    while (isspace(*p)) ++p;    // 读取前置空白字符

    while (!isspace(*p))        // 读取产生式左部
        be.left[j++] = *p++;
    be.left[j] = 0;

    while (*p != '-') ++p;      // 读取 '->' 以及周围空格
    p += 2;
    while (isspace(*p)) ++p;

    i = j = 0;                  // 逐个读取产生式右部
    while (*p) {
        while (!isspace(*p) && *p)
            be.right[i][j++] = *p++;
        be.right[i++][j] = 0;
        j = 0;
        while (isspace(*p)) ++p;
    }

    be.size = i;
}



