//
// Created by JiangChenyang on 2021/11/10 10.
//

#include "action_fun.h"
void gencode(quad_table *q_table, const char *op, const char *addr1, const char *addr2, const char *result) {
    quad q;
    strcpy(q.s1, op);
    strcpy(q.s2, addr1);
    strcpy(q.s3, addr2);
    strcpy(q.s4, result);
    q_table->push_back(q);
}

char* newtemp() {
    char *s = (char*)malloc(sizeof(char) * MAX_SIGN_SIZE);
    sprintf(s, "t%d", name_cnt++);
    return s;
}

void wrong(const string& s) {
    cout << "function " << s << " matched the wrong param" << endl;
    system("pause");exit(-1);
}

// 对 M 进行归约
void m_fun(__attribute__((unused))quad_table *q_table, stasign_stack * sstack, __attribute__((unused)) build_expr * be, sign * sg, __attribute__((unused))sign_table * s_table) {
    // M.value_type = TYPE.value_type
    sg->attr[0] = sstack->top().second.attr[0];
    // M.width = TYPE.width
    sg->attr[0] = sstack->top().second.attr[1];
}

// 对 TYPE 进行归约
void type_fun(__attribute__((unused))quad_table *q_table, __attribute__((unused))stasign_stack * sstack, __attribute__((unused))build_expr * be, sign * sg, __attribute__((unused))sign_table * s_table) {
    // TYPE -> int
    if (strcmp(be->right[0], "int") == 0) {
        sg->attr[0] = INT;  // TYPE.value_type = INT
        sg->attr[1] = 4;    // TYPE.width = 4
    }
        // TYPE -> real
    else if (strcmp(be->right[0], "real") == 0) {
        sg->attr[0] = REAL; // TYPE.value_type = REAL
        sg->attr[1] = 4;    // TYPE.width = 4
    } else {
        wrong(string("type_fun"));
    }
}

// 对 IDS 进行归约
void ids_fun(__attribute__((unused))quad_table *q_table, stasign_stack * sstack, build_expr * be, __attribute__((unused))sign * sg, sign_table * s_table) {
    // IDS -> IDS, N id or IDS -> P id
    if (strcmp(be->right[0], "IDS") == 0 || strcmp(be->right[0], "P") == 0) {
        int pos = s_table->find(sstack->top().second.sign_name);
        // enter(id.name, id.value_type, id.width, offset)
        // offset += id.width
        s_table->table[pos].attr[0] = sstack->top(2).second.attr[0];
        s_table->table[pos].attr[1] = sstack->top(2).second.attr[1];
        s_table->table[pos].attr[2] = offset;
        offset += sstack->top(2).second.attr[1];
    }
    else {
        wrong(string("ids_fun"));
    }
}

// 对 N 进行归约
void n_fun(__attribute__((unused))quad_table *q_table, stasign_stack * sstack, __attribute__((unused))build_expr * be, sign * sg, __attribute__((unused))sign_table * s_table) {
    // N.value_type = IDS.value_type
    sg->attr[0] = sstack->top(3).second.attr[0];
    // N.width = IDS.width
    sg->attr[1] = sstack->top(3).second.attr[1];
}

// 对 P 进行归约
void p_fun(__attribute__((unused))quad_table *q_table, stasign_stack * sstack, __attribute__((unused))build_expr * be, sign * sg, __attribute__((unused))sign_table * s_table) {
    // P.value_type = IDS.value_type
    sg->attr[0] = sstack->top(1).second.attr[0];
    // P.width = IDS.width
    sg->attr[1] = sstack->top(1).second.attr[1];
}

// 对 ASSIGN 进行归约
void assign_fun(quad_table *q_table, stasign_stack * sstack, __attribute__((unused))build_expr * be, __attribute__((unused))sign * sg, __attribute__((unused))sign_table * s_table) {
    char *p1;
    const char *p2;
    char *res;
    p1 = (char*)sstack->top().second.attr[0];
    p2 = "";
    res = (char*)sstack->top(3).second.sign_name;
    gencode(q_table, "=", p1, p2, res);
}

// 对 EXPR 进行归约
void expr_fun(quad_table *q_table, stasign_stack * sstack, build_expr * be, sign * sg, __attribute__((unused))sign_table * s_table) {
    char *p1;
    char *p2;
    char *res;
    if (strcmp(be->right[0], "EXPR") == 0) {
        sg->attr[0] = (long long) newtemp();
        p1 = (char*)sstack->top(3).second.attr[0];
        p2 = (char*)sstack->top(1).second.attr[0];
        res = (char*)sg->attr[0];
        gencode(q_table, (char*)sstack->top(2).second.sign_name, p1, p2, res);
    } else if (strcmp(be->right[0], "TT") == 0) {
        sg->attr[0] = sstack->top().second.attr[0];
    } else {
        wrong(string("expr_fun"));
    }
}

// 对 TT 进行归约
void tt_fun(quad_table *q_table, stasign_stack * sstack, build_expr * be, sign * sg, __attribute__((unused))sign_table * s_table) {
    char *p1;
    char *p2;
    char *res;
    if (strcmp(be->right[0], "TT") == 0) {
        sg->attr[0] = (long long)newtemp();
        p1 = (char*)sstack->top(3).second.attr[0];
        p2 = (char*)sstack->top().second.attr[0];
        res = (char*)sg->attr[0];
        gencode(q_table, (char*)sstack->top(2).second.sign_name, p1, p2, res);
    } else if (strcmp(be->right[0], "FF") == 0) {
        sg->attr[0] = sstack->top().second.attr[0];
        char s[10];
        strcpy(s, (char*)sg->attr[0]);
        int a=1;
    } else {
        wrong(string("tt_fun"));
    }
}

// 对 FF 进行归约
void ff_fun(quad_table *q_table, stasign_stack * sstack, build_expr * be, sign * sg, __attribute__((unused))sign_table * s_table) {
    char *p1;
    const char *p2;
    char *res;
    if (strcmp(be->right[0], "(") == 0) {
        sg->attr[0] = sstack->top(2).second.attr[0];
    } else if (strcmp(be->right[0], "id") == 0) {
        sg->attr[0] = (long long)sstack->top().second.sign_name;
        char s[10];
        strcpy(s, (char*)sg->attr[0]);
        int a=1;
    } else if (strcmp(be->right[0], "dec") == 0 || strcmp(be->right[0], "hex") == 0 ||
               strcmp(be->right[0], "oct") == 0 || strcmp(be->right[0], "bin") == 0 ) {
        sg->attr[0] = (long long)newtemp();
        p1 = sstack->top().second.sign_name;
        p2 = "";
        res = (char*)sg->attr[0];
        gencode(q_table, "=", p1, p2, res);
    } else {
        wrong(string("ff_fun"));
    }
}

