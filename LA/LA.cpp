#include <cctype>
#include <cstdio>
#include <string>

#include "LA.h"

// 函数功能：初始化一个词法分析器
LA::LA(FILE *fd, code_table *c_table) {
    this->fd = fd;
    this->c_table = c_table;
}

// 从缓冲区中读取指定进制数字，并放入 token 表中
void LA::read_digit(int &l, int &r, int attr) {
    char s[HALF_BUF_SIZE];
    l = r;
    while (isdigit(BUF[r]))
        mover(r);

    // 对两种小数情况进行特判
    if (attr == DECIMAL && BUF[r] == '.') {
        mover(r);
        while (isdigit(BUF[r]))
            mover(r);
        bufcpy(s, l, r);
        s[r] = 0;
        attr = REALC;
    } else if (attr == REALC) {
        s[0] = '0';
        s[1] = '.';
        bufcpy(s + 2, l, r);
        s[r + 2] = 0;
    } else {
        bufcpy(s, l, r);
        s[r - l] = 0;
    }

    int ind;
    // 完整的字符串
    char ss[HALF_BUF_SIZE];
    int offset = 0;
    switch (attr) {
        case DECIMAL:
            ind = 10;
            break;
        case HEX:
            ss[0] = '0';
            ss[1] = 'x';
            offset = 2;
            ind = 16;
            break;
        case OCT:
            ss[0] = '0';
            offset = 1;
            ind = 8;
            break;
        case BIN:
            ss[0] = '0';
            ss[1] = 'b';
            offset = 2;
            ind = 2;
            break;
        default:;
    }
    strcpy(ss + offset, s);

    // 生成对应的 sign 符号
    void *p;

    // 求值并放入 tok 表中
    if (attr != REALC) {
        p = malloc(sizeof(LL));
        *(LL*)p = s2d(s, ind);
    }
    else {
        p = malloc(sizeof(double));
        *(double*)p = s2f(s);
    }

    // 在符号表中存入记录
    int pos = s_table.insert(sign(ss, attr, p));
    // 写入 token 表中
    tok.push_back(PAIR(attr, pos));
}

// 从缓冲区中读取字符串或标识符
void LA::read_str(const char* s) {
    int pos;
    if ((pos = c_table->find(s)) != -1 && pos > STRC) {
        // 其位于关键字表中，为关键字
        // 同时，其可以为 STRC 之前的关键字
        sign sg(s, IDF, nullptr);
        tok.push_back(PAIR(pos, s_table.insert(sg)));
    }
    else if ((pos = s_table.find(s)) != -1)
        // 其已经存在于符号表中，说明其为标识符
        tok.push_back(PAIR(IDF, pos));
    else {// 该标识符第一次出现
        sign sg(s, IDF, nullptr);
        tok.push_back(PAIR(IDF, s_table.insert(sg)));
    }
}

/*
            函数功能：给定文件句柄 fd，从文件中读取字符流，并进行词法分析
            fd: 文件句柄
            tok: token 表，表示顺序检索到的单词序列
            sign_table: 符号表
            kw_table: 编码表
        */
void LA::analyze() {
    fill_half_buf(0); // 填充前一半
    int l = 0, r = 0;
    char s[HALF_BUF_SIZE];
    for (;;) {
        // 读取空白字符
        while (isspace(BUF[r]))
            mover(r);
        l = r;

        if (BUF[r] == 0 || BUF[r] == EOF)
            break;

        // 如果第一个是字符，则可读出标识符或关键字
        // 若字符串位于编码表中，则其为关键字
        // 否则其为标识符，需要在符号表中继续查重
        if (isalpha(BUF[r])) {
            while (isalnum(BUF[r]))
                mover(r);
            // 分析出标识符 or 关键字
            bufcpy(s, l, r);
            s[r - l] = 0;
            read_str(s);
        }

            // 若第一个字符为数字，则读取整个数字
        else if (isdigit(BUF[r])) {
            if (BUF[r] > '0')   // 首字符非 0，必为十进制数或小数
                read_digit(l, r, DECIMAL);
            else {              // 首字符为 0
                mover(r);
                if (BUF[r] == 'x' || BUF[r] == 'X') { // 十六进制
                    mover(r);
                    read_digit(l, r, HEX);
                } else if (BUF[r] == 'b' || BUF[r] == 'B') { // 二进制
                    mover(r);
                    read_digit(l, r, BIN);
                } else if (BUF[r] >= '1' && BUF[r] <= '7') { // 八进制
                    read_digit(l, r, OCT);
                } else if (BUF[r] == '.') { // 小数
                    mover(r);
                    read_digit(l, r, REALC);
                } else { // 为 0
                    void *p = malloc(sizeof(LL));
                    *(LL*)p = 0;
                    sign sg("0", DECIMAL, (void *) p);
                    tok.push_back(PAIR(DECIMAL, s_table.insert(sg)));
                }
            }
        }

            // 读取除号或注释
        else if (BUF[r] == '/') {
            mover(r);
            if (BUF[r] == '/')
                while (BUF[r] != '\n')
                    mover(r);
            else {
                sign sg("/", DIV, nullptr);
                tok.push_back(PAIR(DIV, s_table.insert(sg)));
            }
        }

            // 读取字符串常量
        else if (BUF[r] == '\"') {
            do {
                mover(r);
            } while(BUF[r] != '\"');
            mover(r);
            bufcpy(s, l, r);
            s[r - l] = 0;
            int pos = s_table.insert(sign(s, STRC, nullptr));
            tok.push_back(PAIR(STRC, pos));
        }

            // 运算符、分界符
        else {
            // 假设当前为二元运算符
            char lc, rc;
            lc = BUF[r];
            mover(r);
            rc = BUF[r];
            char bs[3] = {lc, rc, 0};

            // 判断当前二元运算符是否存在
            int pos;
            if ((pos = c_table->find(bs)) != -1) {
                sign sg(bs, pos, nullptr);
                tok.push_back(PAIR(pos, s_table.insert(sg)));
                mover(r);
            } else {
                bs[1] = 0;
                pos = c_table->find(bs);
                sign sg(bs, pos, nullptr);
                tok.push_back(PAIR(pos, s_table.insert(sg)));
            }
        }
    }
}

/*
    函数功能：打印出 token 表
             token 表第一列表示符号编码，第二列表示属性值
*/
void LA::print_token(FILE *tf) {
    fprintf(tf, "%-20s%-20s%-20s%-20s\n", "type num", "value index", "type", "value");
    LL a, b;
    for (auto v : tok) {
        a = v.first;
        b = v.second;
        char * name = c_table->table[a].macro_name;
        if (a == IDF || a == STRC)
            fprintf(tf, "%-20d%-20lld%-20s%-20s\n", a, b, name, s_table.table[b].sign_name);
        else if (a > IDF && a < REALC)
            fprintf(tf, "%-20d%-20lld%-20s%-20lld\n", a, b, name, *(LL*)s_table.table[b].num);
        else if (a == REALC)
            fprintf(tf, "%-20d%-20lf%-20s%-20lf\n", a, b, name, *(double *)&b);
        else
            fprintf(tf, "%-20d%-20lld%-20s%-20d\n", a, b, name, -1);
    }
}

/*
    函数功能：打印出符号表
             第一列打印出属性值索引，第二列打印出对应字符串
*/
void LA::print_sign_table(FILE *sf) {
    unsigned int sz = s_table.table.size();
    for (int i = 0; i < sz; ++i) {
        sign sg = s_table.table[i];
        fprintf(sf, "%-20d%-20s%-20d", i, sg.sign_name, sg.code);
        if (sg.code == IDF || sg.code == STRC)
            fprintf(sf, "%-20s\n", sg.sign_name);
        else if (sg.code > IDF && sg.code < REALC)
            fprintf(sf, "%-20lld\n", *(LL*)sg.num);
        else if (sg.code == REALC)
            fprintf(sf, "%-20lf\n", *(double*)sg.num);
        else
            fprintf(sf, "%-20s\n", "-1");
    }
}

/*
    函数功能：打印出编码表
             第一列打印出编码，第二列为符号
*/
void LA::print_kw_table(FILE *kf) {
    fprintf(kf, "%-20s%-20s%-20s\n", "sign", "macro name", "macro value");
    fprintf(kf, "------------------------------------------------------\n");
    for (int i = 1; i < c_table->size; ++i)
        fprintf(kf, "%-20s%-20s%-20d\n", c_table->table[i].category_name, c_table->table[i].macro_name
                , c_table->table[i].code);
}
