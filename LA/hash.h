#ifndef HASH_H
#define HASH_H

#define ALL

#include <bits/stdc++.h>
using namespace std;


const int HASH_MOD = 1009;

/*
 * 对 T 进行 hash 操作
 * 注意，所有传入的 T 都必须实现 get_hashobj() 操作，返回一个用于 hash 的对象
 * 在此处，所有 hash 的对象都必须为字符串类型
 */
template <class T>
class hash_table {
    public:
    // 存储待哈希结构体
    vector<T> table;
    // 存储哈希对象在 table 中的索引
    vector<int> h_table[HASH_MOD];
    int size = 0;

    hash_table();
    // 释放所有内存
    void clear();
    // 调用结构体的 get_hashobj() 函数，获取待 hash 对象
//    static int hash(T);
    // 对字符串进行 hash
    static int hash_str(char *s);
    // 给定字符串，返回被查找结构体在 table 表中的索引
    int find(const char s[]);
    // 插入指定对象
    int insert(T);
};

/*
 * hash 所需要的接口
 */
class hash_interface {
public:
    virtual char * get_hashobj()=0;
};

/*
 * -----------------------------------------------------------
 *               模板类的实现与声明应该在一起
 * -----------------------------------------------------------
 */
template <class T>
int hash_table<T>::hash_str(char* s) {
    int num = 0;
    while(*s)
        num = ((num<<3) + (num<<1) + *(s++)) % HASH_MOD;
    return num;
}

template <class T>
hash_table<T>::hash_table() {
    hash_table::clear();
}

//// 对字符串进行哈希
//template <class T>
//int hash_table<T>::hash(T item) {
//    char *s = item.get_hashobj();
//    int num = hash_str(s);
//    return num;
//}

// 释放哈希表中所有字符串内存，并清空哈希表
template <class T>
void hash_table<T>::clear() {
    for (auto & i : h_table) {
        i.clear();
    }
    table.clear();
    size = 0;
}

// 查找是否已插入该元素，若未插入，返回 -1
template <class T>
int hash_table<T>::find(const char s[]) {
    int h_num = hash_str(const_cast<char *>(s));
    if (!h_table[h_num].empty())
        for (int i : h_table[h_num])
            if (!strcmp(s, table[i].get_hashobj()))
                return i;
    return -1;
}


// 插入一个字符串，返回其在 table 中的位置
template <class T>
int hash_table<T>::insert(T item) {
    char *s = item.get_hashobj();
    int pos;
    if ((pos = find(s)) > 0)
        return pos;
    int h_num = hash_str(s);

    table.push_back(item);
    h_table[h_num].push_back(size++);
    return size - 1;
}

#endif