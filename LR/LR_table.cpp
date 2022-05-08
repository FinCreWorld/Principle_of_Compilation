//
// Created by JiangChenyang on 2021/10/14 13.
//
#include "LR_table.h"
#include "action_fun.h"

/*
 * 实现动作表
 */

void action_table::add_table(code_table * cc_table, bexpr_table * bb_table) {
    this->c_table = cc_table;
    this->b_table = bb_table;
}

/*
 * 读取状态表行首终止符，如果达到行尾，则返回 0
 */
inline int action_table::read_head(FILE* fd, char s[]) {
    // 这个字符串读入我给满分，请叫我大聪明
//    int status = fscanf(fd, "%[^,\n]%*c", s);
    // ↑ 这个设计很垃圾，已经修改
    char ch;
    fscanf(fd, "%[^;\n]", s);
    ch = (char)fgetc(fd);
    char *p = s + strlen(s) - 1;
    while(isspace(*p) || iscntrl(*p))
        --p;
    *++p = '\0';
    return ch != '\n';
}

/*
 * 读取状态表中一个单独的表格
 */
int action_table::read_body(FILE* fd, action& action) const {
    char cell[MAX_SIGN_SIZE * MAX_RIGHT_SIZE]={0};
    char ch;

    // 读取单个 cell
    // 读到 ',' 或 '\n' 时停止读入，并读入其后一个字符
    fscanf(fd, "%[^;\n]", cell);
    ch = (char)fgetc(fd);
    if (ch <= 0)
        return 0;

    if(cell[0] == 'r') {    // 如果当前式子表示归约
        build_expr be;
        build_expr::parse(be, cell);
        int pos = b_table->insert(be);
        action.action = 'r';
        action.num = pos;
    } else if (cell[0] == 's'){                // 如果当前式子表示移入
        action.action = 's';
        char *p = cell;
        char *endp;
        while (!isdigit(*p)) ++p;
        action.num = strtol(p, &endp, 10);
    } else if (cell[0] == 'a') {
        action.action = 'a';
        action.num = -1;
    } else {
        action.action = 0;
        action.num = -1;
    }

    return 1;
}


/*
 * 解析动作表
 * 约定动作表格式：
 *  1. 第一行表示所有出现的终结符号（宏名），定义最后一个终结符为 END
 *  2. 接下来 n 行表示从状态 0 到状态 n-1 的动作
 *     第 i 行 j 列为 shift $num 或 reduce $num
 *     表示状态为 i，第 j 列的终结符对应的状态
 *     reduce -1 表示非法状态
 */
void action_table::parse(FILE* fd) {
    int trans[END_SIGN_NUM];    // trans[i] 表格中第 i 列终结符在编码表中的编号
    char sign[MAX_SIGN_SIZE];
    int col = 0;
    int ret;

    // 读取终止符
    do {
        ret = read_head(fd, sign);
        trans[col] = c_table->find(sign);   // 要求终止符必须在 c_table 中出现
        if (trans[col++] == -1) {
            printf("unmatched end sign: %s\n", sign);
            system("pause");exit(-1);
        }
    } while (ret);

    action action{};
    // 读取表格的主体部分
    // 状态从 0 开始
    for(;;) {
        if (!read_body(fd, action))
            break;
        actions[status_num][trans[0]] = action;
        for (int i = 1; i < col; ++i)
            read_body(fd, actions[status_num][trans[i]]);
        ++status_num;
    }
}

action action_table::get_action(int status, int sign_pos) {
    return actions[status][sign_pos];
}

action_table::action_table() {
    status_num = 0;
    c_table = nullptr;
    memset(actions, 0, sizeof(actions));
}


/*
 * 实现 goto 表
 */

void goto_table::add_table(msign_table * table) {
    this->m_table = table;
}

inline int goto_table::read_head(FILE *fd, char s[]) {
    char ch;
    fscanf(fd, "%[^;\n]", s);
    ch = (char)fgetc(fd);
    char *p = s + strlen(s) - 1;
    while (isspace(*p) || iscntrl(*p))
        --p;
    *++p = 0;
    return ch != '\n';
}

int goto_table::read_body(FILE *fd, int &num) {
    char cell[MAX_SIGN_SIZE] = {0};
    char ch;

    fscanf(fd, "%[^;\n]", cell);
    ch = (char)fgetc(fd);
    if (ch <= 0)
        return 0;

    if (!cell[0]) {
        num = -1;
    } else {
        char *p;
        char *endp;
        p = cell;
        while ((!isdigit(*p)) && *p)
            ++p;
        if (!cell[0])
            num = -1;
        else
            num = strtol(p, &endp, 10);
    }
    return 1;
}

void goto_table::parse(FILE* fd) {
    int trans[MID_SIGN_NUM];
    char sign[MAX_SIGN_SIZE];
    int col = 0;
    int ret;

    // 读取表头
    do {
        ret = read_head(fd, sign);
        trans[col] = m_table->find(sign);
        if (trans[col++] == -1) {
            printf("unmatched middle sign: %s\n", sign);
            system("pause");exit(-1);
        }
    } while (ret);

    // 读取 goto 表的主体部分
    int num;
    for (;;) {
        if (!read_body(fd, num))
            break;
        gotos[status_num][trans[0]] = num;
        for (int i = 1; i < col; ++i) {
            read_body(fd, num);
            gotos[status_num][trans[i]] = num;
        }
        ++status_num;
    }
}

int goto_table::get_goto(int status, int num) {
    return gotos[status][num];
}

goto_table::goto_table() {
    status_num = 0;
    m_table = nullptr;
    memset(gotos, 0, sizeof(gotos));
}

/*
 * 定义非终结符，定义语义动作
 */
void init_msign_table(msign_table& m_table) {
    int msign_value[] = {
            SENTENCE, STATE, TYPE, IDS, ASSIGN, EXPR, TT, FF, SENTENCES, M, N, P
    };
    char sign_name[][MAX_IDF_NAME] = {
            "SENTENCE", "STATE", "TYPE", "IDS", "ASSIGN", "EXPR", "TT", "FF", "SENTENCES", "M", "N", "P"
    };
    action_fun action_funs[MSIGN_NUM] = {
            nullptr, nullptr, type_fun, ids_fun, assign_fun, expr_fun, tt_fun, ff_fun, nullptr, m_fun, n_fun, p_fun
    };
    for (int i = 0; i < MSIGN_NUM; ++i) {
        sign sg(sign_name[i], msign_value[i], nullptr);
        sg.af = action_funs[i];
        m_table.insert(sg);
    }
}


