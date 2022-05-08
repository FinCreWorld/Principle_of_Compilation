#include <bits/stdc++.h>
#include "tool.h"

typedef long long LL;

// 整数进制转换
LL s2d(char *s, int ind) {
    if (!*s) return 0;
    while ((*s == '0' || !isalnum(*s)) && *s) ++s;
    LL num = 0, tmp;
    if (ind < 10)
        while (*s)
            num = num*ind + *(s++) - '0';
    else
        while (*s) {
            tmp = (unsigned char) *(s++);
            if (tmp >= 'a') tmp = tmp - 'a' + 10;
            else if (tmp >= 'A') tmp = tmp -'A' + 10;
            else tmp -= '0';
            num = num*ind + tmp;
        }
    return num;
}

// 浮点数进制转换
double s2f(char *s) {
    char *p = s;
    double num = 0;
    while (*p != '.')
        num = num * 10 + (*(p++) - '0');
    do {++p;} while (isdigit(*p));
    double mum = 0;
    --p;
    while (*p != '.')
        mum = mum * 0.1 + (*(p--) - '0');
    mum *= 0.1;
    return num + mum;
}

// 测试程序：pass
void test() {
    char s1[] = "77";
    char s2[] = "a2";
    char s3[] = "01101100";
    char s4[] = "0.938203";
    printf("%lld\n", s2d(s1, 10));
    printf("%lld\n", s2d(s1, 8));
    printf("%lld\n", s2d(s1, 16));
    printf("%lld\n", s2d(s2, 16));
    printf("%lld\n", s2d(s3, 2));
    printf("%lf\n", s2f(s4));
}

#ifndef ALL

int main() {
    test();
    return 0;
}

#endif