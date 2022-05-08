#ifndef LA_H
#define LA_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <vector>
#include <utility>
#include <string>
#include "buffer.h"
#include "sign.h"
#include "hash.h"
#include "tool.h"

using namespace std;

typedef long long LL;

class LA : buffer{
    private:
        code_table *c_table;

        // 从缓冲区中读取指定进制数字，并放入 token 表中
        void read_digit(int &l, int &r, int attr);

        // 从缓冲区中读取字符串或标识符
        void read_str(const char* s);
    public:
        sign_table s_table;
        token tok;
        LA(FILE* fd, code_table *c_table);

        /*
            函数功能：给定文件句柄 fd，从文件中读取字符流，并进行词法分析
            fd: 文件句柄
            tok: token 表，表示顺序检索到的单词序列
            sign_table: 符号表
            kw_table: 编码表
        */
        void analyze();

        /*
            函数功能：打印出 token 表
        */
       void print_token(FILE* tf);

        /*
            函数功能：打印出符号表
        */
       void print_sign_table(FILE* sf);

        /*
         * 函数功能：打印出编码表
         */
       void print_kw_table(FILE* kf);
};

#endif