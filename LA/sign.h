#ifndef SIGN_H
#define SIGN_H

#include <bits/stdc++.h>
#include "hash.h"
#include "tool.h"

using namespace std;

// 结束符
#define END 0
// 标识符
#define IDF 1
// 常数类型
#define DECIMAL 2
#define HEX 3
#define OCT 4
#define BIN 5
#define REALC 6
#define STRC 7
// 关键字
#define INT 8
#define REAL 9
#define SIZEOF 10
#define STRING 11
#define IF 12
#define ELSE 13
#define ELIF 14
#define WHILE 15
#define BREAK 16
#define GOTO 17
#define STRUCT 18
#define RETURN 19
// 运算符
#define PLUS 20
#define MINUS 21
#define MUL 22
#define DIV 23
#define EQU 24
#define BIG 25
#define LESS 26
#define BIGE 27
#define LESSE 28
#define EQUE 29
#define NEQU 30
#define OROR 31
#define ANDAND 32
#define NOT 33
#define OR 34
#define XOR 35
#define AND 36
// 分界符
#define LBKT 37
#define RBKT 38
#define LBTS 39
#define RBTS 40
#define LBRC 41
#define RBRC 42
#define COMMA 43
#define SEMIC 44
#define QUO 45

const int MAX_SIGN_SIZE = 10;
const int SIGN_NUM = 46;
typedef long long LL;


/*
 * 类型编码
 */
class category : public hash_interface{
public:
    char macro_name[MAX_SIGN_SIZE]{}; // 宏名
    char category_name[MAX_SIGN_SIZE]{};  // 类型名
    int code;   // 类型编码
    category();
    category(const char*, const char*, int);
    char * get_hashobj() override;
};

/*
 * 编码表
 */
typedef hash_table<category> code_table;


class sign;
class build_expr;
/*
 * 符号表
 */
typedef hash_table<sign> sign_table;
typedef pair<int, sign> status_n_sign;
typedef my_stack<status_n_sign> stasign_stack;
typedef struct {
    char s1[MAX_SIGN_SIZE];
    char s2[MAX_SIGN_SIZE];
    char s3[MAX_SIGN_SIZE];
    char s4[MAX_SIGN_SIZE];
}quad;
typedef vector<quad> quad_table;
typedef void(*action_fun)(quad_table *q_table, stasign_stack * sstack, build_expr * be, sign * sg, sign_table * s_table);

/*
 * 符号
 */
const int MAX_IDF_NAME = 256;
class sign : public hash_interface{
public:
    // 数据为字符串类型，则存入 sign_name 中
    // 数据为数值类型，则存入 num 中
    char *sign_name;   // 符号名
    int code;   // 类型编码
    void* num;    // 值
    long long attr[10]{};
    action_fun af{};

    sign();
    sign(const char*, int, void*);
    explicit sign(char[]);
//    ~sign();
    char * get_hashobj() override;
};

/*
 * token 表，token[0] = (a, b)
 * 其中 a 表示单词的类别编码，同时也表示该列表在编码表中的位置
 * 若符号类型为常数型（非字符串常量），则 b 表示符号的值
 * 若符号类型为
 */
typedef pair<LL, LL> PAIR;
typedef vector<PAIR> token;

void init_code(code_table &c_table);

// 关键字
//extern const int SIGN_NUM;
//extern const char *SIGNS[];
//extern const int SIGNA[];
//
//void init_kw(hash_table &kw_table);        // 用来初始化关键字表
//void init_code(code_table &c_table);     // 用于初始化编码

#endif