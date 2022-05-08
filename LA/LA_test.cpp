//
// Created by JiangChenyang on 2021/10/14 09.
//
#include "LA_test.h"
int LA_test() {
    // 白痴 Clion，不知道文件怎么打开的
    char path_in[] = "./data/LA_data/input_code.txt";
    char path_token[] = "./data/LA_data/token.txt";
    char path_sign[] = "./data/LA_data/sign.txt";
    char path_kw[] = "./data/LA_data/kw.txt";

    FILE *f_in = fopen(path_in, "r");
    FILE *f_token = fopen(path_token, "w");
    FILE *f_sign = fopen(path_sign, "w");
    FILE *f_kw = fopen(path_kw, "w");

    code_table c_table;
    init_code(c_table);
    LA la(f_in, &c_table);
    la.analyze();
    la.print_token(f_token);
    la.print_sign_table(f_sign);
    la.print_kw_table(f_kw);

    fclose(f_in);
    fclose(f_token);
    fclose(f_sign);
    fclose(f_kw);
    return 0;
}