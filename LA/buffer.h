//
// Created by JiangChenyang on 2021/10/19 17.
//

#ifndef EXP2_BUFFER_H
#define EXP2_BUFFER_H
#include <bits/stdc++.h>

typedef long long LL;

const int BUF_SIZE = 1024;
const int HALF_BUF_SIZE = BUF_SIZE/2;

class buffer {
protected:
    FILE * fd = nullptr;
    char BUF[BUF_SIZE]{};
public:
    // 用于填满缓冲区的一半，part = 0 填满前一半，否则填满后一半
    void fill_half_buf(int part);

    // 用于控制指针移动
    void mover(int &r);

    // 从缓冲区中复制指定区域到指定指针
    void bufcpy(char s[], int l, int r);
};


#endif //EXP2_BUFFER_H
