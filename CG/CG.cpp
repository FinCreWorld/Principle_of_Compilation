//
// Created by JiangChenyang on 2021/11/11 19.
//
#include "CG.h"

variable::variable() {
    r = -1;
    refcnt = 0;
    pos.clear();
    p = 0;
}

variable::variable(char *s) {
    strncpy(name, s, MAX_NAME_SIZE);
    r = -1;
    refcnt = 0;
    pos.clear();
    p = 0;
}

char *variable::get_hashobj() {
    return name;
}

// 向变量表中插入变量
void CG::insert_v_table(int mccnt, char s[]) {
    int pos = v_table.find(s);
    // 如果变量表中不存在该变量则插入
    if (pos == -1) {
        variable var(s);
        pos = v_table.insert(var);
    }
    // 添加变量的引用次数
    ++v_table.table[pos].refcnt;
    // 添加变量的引用位置
    v_table.table[pos].pos.push_back(mccnt);
}

CG::CG() {
    inst_seq.clear();
}

void CG::init() {
    for (int i = 0; i < q_table->size(); ++i){
        // 根据 quad_table 添加变量，或者更新变量信息
        quad q = (*q_table)[i];
        if (strcmp("", q.s2) != 0)
            insert_v_table(i, q.s2);
        if (strcmp("", q.s3) != 0)
            insert_v_table(i, q.s3);
        if (strcmp("", q.s4) != 0)
            insert_v_table(i, q.s4);
    }
}

// 形如 (op, p1, p2, res)
// 根据目标地址和最左边的参数获得寄存器资源
int CG::get_reg(char *p1, char *res) {
    int p1_pos = v_table.find(p1);
    int res_pos = v_table.find(res);
    variable p1v = v_table.table[p1_pos];
    variable resv = v_table.table[res_pos];

    // 如果 p1 已经在寄存器中，同时 p1 在之后没有引用
    if (p1v.r >= 0 && p1v.refcnt == 1)
        return p1v.r;

    // 如果存在空闲寄存器，则将 p1 的值放入寄存器中
    // 将 res 与该寄存器连接起来
    // 并返回该寄存器
    for (int i = 0; i < REG_NUM; ++i) {
        if (reg[i] == -1) {
            if (p1v.r != -1)
                inst_seq.emplace_back("MOV", reg_name[p1v.r], reg_name[i]);
            else
                inst_seq.emplace_back("MOV", p1, reg_name[i]);
            // 更新寄存器信息
            reg[i] = res_pos;
            // 更新变量描述符
            v_table.table[res_pos].r = i;
            return i;
        }
    }

    // 如果不存在空闲寄存器，则进行抢占
    // 选择最远未引用的寄存器
    int des_reg = 0;
    int inv = -1;
    for (int i = 0; i < REG_NUM; ++i) {
        variable v = v_table.table[reg[i]];

        // 如果之后再也不会引用到
        if (v.p == v.pos.size() - 1) {
            des_reg = i;
            break;
        }

        // 判断下一次引用间隔是否大于 inv
        if (v.pos[v.p + 1] - v.pos[v.p] > inv) {
            inv = v.pos[v.p + 1] - v.pos[v.p];
            des_reg = i;
        }
    }

    // 将寄存器的值放入变量的内存位置，同时更新变量描述符
    inst_seq.emplace_back("MOV", reg_name[des_reg], v_table.table[reg[des_reg]].name);
    v_table.table[reg[des_reg]].r = -1;
    // 将 p1 值移入当前寄存器
    inst_seq.emplace_back("MOV", p1, reg_name[des_reg]);
    // 更新 res 的变量描述符
    v_table.table[res_pos].r = des_reg;
    // 更新 des_reg 的寄存器描述符
    reg[des_reg] = res_pos;
    return des_reg;
}

void CG::move_inst(char *p1, char *res) {
    int des_reg = get_reg(p1, res);
    inst_seq.emplace_back("MOV", reg_name[des_reg], res);
}

void CG::binop_inst(char *op, char *p1, char *p2, char *res) {
    int des_reg = get_reg(p1, res);
    char oper[10];
    if (strcmp(op, "+") == 0)
        strcpy(oper, "ADD");
    else if (strcmp(op, "-") == 0)
        strcpy(oper, "SUB");
    else if (strcmp(op, "/") == 0)
        strcpy(oper, "DIV");
    else if (strcmp(op, "*") == 0)
        strcpy(oper, "MUL");
    else {
        printf("unexpected operation %s\n", op);
        system("pause");exit(-1);
    }
    inst_seq.emplace_back(oper, reg_name[des_reg], p2);
    inst_seq.emplace_back("MOV", reg_name[des_reg], res);
}


// 处理完一行中间代码后
// 对涉及到的符号信息进行更新
void CG::updatev(char *p1, char *p2, char *res) {
    int p1_pos = v_table.find(p1);
    int p2_pos = v_table.find(p2);
    int res_pos = v_table.find(res);
    variable *p1v = &v_table.table[p1_pos];
    variable *p2v = &v_table.table[p2_pos];
    variable *resv = &v_table.table[res_pos];

    // 引用次数减一
    p1v->refcnt -= 1;
    p2v->refcnt -= 1;
    resv->refcnt -= 1;

    // 更新当前位置索引
    p1v->p += 1;
    p2v->p += 1;
    resv->p += 1;

    // 若引用次数为 0，则将寄存器值写入内存
    // 并释放占用的寄存器
    // 将寄存器值放入内存
    if (p1v->refcnt == 0 && p1v->r > -1) {
//        inst_seq.emplace_back("MOV", reg_name[p1v.r], p1v.name);
        reg[p1v->r] = -1;
    }
    if (p2v->refcnt == 0 && p2v->r > -1) {
//        inst_seq.emplace_back("MOV", reg_name[p2v.r], p2v.name);
        reg[p2v->r] = -1;
    }
    if (resv->refcnt == 0 && resv->r > -1) {
//        inst_seq.emplace_back("MOV", reg_name[resv.r], resv.name);
        reg[resv->r] = -1;
    }
}

void CG::wrong(char *s) {
    printf("unrecognized sign %s\n", s);
}

void CG::analysis() {
    for (auto &mc : *q_table) {
        char *op = mc.s1;
        char *p1 = mc.s2;
        char *p2 = mc.s3;
        char *res = mc.s4;
        if (strcmp(op, "=") == 0)
            move_inst(p1, res);
        else if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0 ||
                 strcmp(op, "*") == 0 || strcmp(op, "/") == 0)
            binop_inst(op, p1, p2, res);
        else
            wrong(op);
        updatev(p1, p2, res);
    }
}

void CG::print_inst_seq(FILE *fd) {
    for (auto &v : inst_seq) {
        fprintf(fd, "%10s%10s,%10s\n", v.op, v.s1, v.s2);
    }
}

void CG::add(quad_table* qt) {
    this->q_table = qt;
}