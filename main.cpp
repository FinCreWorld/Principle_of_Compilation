#define ALL

#include "LA/LA.h"
#include "LR/LR.h"
#include "CG/CG.h"

using namespace std;

FILE* my_open(char *s, const char *op) {
    FILE *fd = fopen(s, op);
    if (fd) return fd;
    if (strcmp(op, "r") == 0)
        printf("open input file %s failed.\n", s);
    else
        printf("open output file %s failed.\n", s);
    system("pause");
    exit(-1);
}

void open_file(FILE **ap_f, FILE **gp_f, FILE **sp_f, FILE **tp_f,
               FILE **sgp_f, FILE **rp_f, FILE ** qp_f, FILE **ip_f,
               FILE **kw_f) {
    char action_path[] = "data/action_table.csv";
    char goto_path[] = "data/goto_table.csv";
    char src_path[] = "data/input_code.txt";
    char token_path[] = "data/token.txt";
    char sign_path[] = "data/sign.txt";
    char reduce_path[] = "data/reduce.txt";
    char quad_path[] = "data/quad.txt";
    char inst_path[] = "data/inst.asm";
    char keyword_path[] = "data/kw.txt";
    *ap_f = my_open(action_path, "r");
    *gp_f = my_open(goto_path, "r");
    *sp_f = my_open(src_path, "r");
    *tp_f = my_open(token_path, "w");
    *sgp_f = my_open(sign_path, "w");
    *rp_f = my_open(reduce_path, "w");
    *qp_f = my_open(quad_path, "w");
    *ip_f = my_open(inst_path, "w");
    *kw_f = my_open(keyword_path, "w");
}

int main() {
    FILE *ap_f, *gp_f, *sp_f, *tp_f, *sgp_f, *rp_f, *qp_f, *ip_f, *kw_f;
    open_file(&ap_f, &gp_f, &sp_f, &tp_f, &sgp_f, &rp_f, &qp_f, &ip_f, &kw_f);

    // 词法分析器准备
    code_table c_table;     // 编码表

    init_code(c_table); // 初始化编码表
    LA la(sp_f, &c_table);  // 词法分析器，传入代码文件指针
    la.analyze();

    la.print_kw_table(kw_f);
    la.print_token(tp_f);
    la.print_sign_table(sgp_f);


    // 语法分析器准备
    action_table a_table;   // 动作表
    bexpr_table b_table;    // 表达式表
    goto_table g_table;     // goto 表
    msign_table m_table;    // 非终结符表
    init_msign_table(m_table);  // 初始化非终结符号表
    a_table.add_table(&c_table, &b_table);  // 链接动作表与表达式表
    a_table.parse(ap_f);
    g_table.add_table(&m_table);    // 链接 goto 表与非终结符号表
    g_table.parse(gp_f);

    LR lr;
    lr.add(&a_table, &g_table, &la.s_table, &m_table, &la.tok);
    lr.analysis();

    lr.print_reduce_seq(rp_f);
    lr.print_quad_table(qp_f);


    // 中间代码生成准备
    CG cg;
    cg.add(&lr.q_table);
    cg.init();
    cg.analysis();
    cg.print_inst_seq(ip_f);

    system("pause");
    return 0;
}
