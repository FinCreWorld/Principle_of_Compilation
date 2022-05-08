#include "sign.h"

//const char *SIGNS[SIGN_NUM] = {"int", "real", "sizeof", "string", "if", "else",
//                        "elif", "while", "break", "goto", "struct", "return",
//                        "+", "-", "*", "/", "=", ">", "<", ">=", "<=", "==", "!=", "||", "&&",
//                        "!", "|", "^", "&", "(", ")", "[", "]", "{", "}", ",", ";", "\""};
//const int SIGNA[SIGN_NUM] = {INT, REAL, SIZEOF, STRING, IF, ELSE,
//                        ELIF, WHILE, BREAK, GOTO, STRUCT, RETURN,
//                        PLUS, MINUS, MUL, DIV, EQU, BIG, LESS, BIGE, LESSE, EQUE, NEQU, OROR, ANDAND,
//                        NOT, OR, XOR, AND, LBKT, RBKT, LBTS, RBTS, LBRC, RBRC, COMMA, SEMIC, QUO};
//
//void init_kw(hash_table& kw_table) {        // 用来初始化关键字表
//    kw_table.clear();
//    for (auto & i : SIGNS)
//        kw_table.insert(i);
//}

category::category() {
    this->macro_name[0] = '\0';
    this->category_name[0] = '\0';
    this->code = -1;
}

category::category(const char* m_name, const char* c_name, int code) {
    strcpy(macro_name, m_name);
    strcpy(category_name, c_name);
    this->code = code;
}

char * category::get_hashobj() {
    return category_name;
}

sign::sign() {
    sign_name = nullptr;
    code = -1;
    num = nullptr;
    af = nullptr;
}

sign::sign(const char* s_name, int code, void* p) {
    this->sign_name = (char*) malloc(sizeof(char) * (strlen(s_name) + 1));
    if (s_name)
        strcpy(sign_name, s_name);
    this->code = code;
    this->num = p;
    this->af = nullptr;
}

//sign::~sign() {
//    if (num)
//        free(num);
//}

char * sign::get_hashobj() {
    return sign_name;
}

sign::sign(char s[]) {
    this->sign_name = (char*) malloc(sizeof(char)*(strlen(s)+1));
    code = 0;
    num = nullptr;
    strcpy(sign_name, s);
}

void init_code(code_table &c_table) {
    int sign_value[] = {    END, IDF, DECIMAL, HEX, OCT, BIN, REALC, STRC,
                            INT, REAL, SIZEOF, STRING, IF, ELSE,
                            ELIF, WHILE, BREAK, GOTO, STRUCT, RETURN,
                            PLUS, MINUS, MUL, DIV, EQU, BIG, LESS, BIGE, LESSE, EQUE, NEQU, OROR, ANDAND,
                            NOT, OR, XOR, AND, LBKT, RBKT, LBTS, RBTS, LBRC, RBRC, COMMA, SEMIC, QUO
    };
    // ; 号无法在编译工作台中直接使用，因此使用 semic 代替
    char category_name[][MAX_SIGN_SIZE] = {"$", "id", "dec", "hex", "oct", "bin", "realc", "strc",
                            "int", "real", "sizeof", "string", "if", "else",
                            "elif", "while", "break", "goto", "struct", "return",
                            "+", "-", "*", "/", "=", ">", "<", ">=", "<=", "==", "!=", "||", "&&",
                            "!", "|", "^", "&", "(", ")", "[", "]", "{", "}", ",", "semic", "\""
    };
    char macro_name[][MAX_SIGN_SIZE] = {"END", "IDF", "DECIMAL", "HEX", "OCT", "BIN", "REALC", "STRC",
                          "INT", "REAL", "SIZEOF", "STRING", "IF", "ELSE", "ELIF",
                          "WHILE", "BREAK", "GOTO", "STRUCT", "RETURN", "PLUS", "MINUS",
                          "MUL", "DIV", "EQU", "BIG", "LESS", "BIGE", "LESSE", "EQUE",
                          "NEQU", "OROR", "ANDAND", "NOT", "OR", "XOR", "AND", "LBKT",
                          "RBKT", "LBTS", "RBTS", "LBRC", "RBRC", "COMMA", "SEMIC", "QUO"};
    for (int i = 0; i < SIGN_NUM; ++i) {
        category ctg = category(macro_name[i], category_name[i], sign_value[i]);
        c_table.insert(ctg);
    }
}