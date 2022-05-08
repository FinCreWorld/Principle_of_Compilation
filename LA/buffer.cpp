//
// Created by JiangChenyang on 2021/10/19 17.
//

#include "buffer.h"

// 用于填满缓冲区的一半，part = 0 填满前一半，否则填满后一半
void buffer::fill_half_buf(int part) {
    char *p = part ? BUF + HALF_BUF_SIZE : BUF;
    int cnt = HALF_BUF_SIZE;
    while (cnt--) {
        *p = (char) fgetc(fd);
        if (*(p++) == EOF) break;
    }
}

// 用于控制指针移动
void buffer::mover(int &r) {
    if (r == HALF_BUF_SIZE - 1)
        fill_half_buf(1);
    else if (r == BUF_SIZE - 1)
        fill_half_buf(0);
    r = (r + 1) % BUF_SIZE;
}

// 从缓冲区中复制指定区域到指定指针
void buffer::bufcpy(char s[], int l, int r) {
    if (l < r) {
        memcpy(s, BUF + l, r - l);
    } else {
        memcpy(s, BUF + l, BUF_SIZE - l);
        memcpy(s + BUF_SIZE - 1, BUF, r);
    }
}

