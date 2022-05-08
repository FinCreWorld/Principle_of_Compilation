#include <cstdlib>
#include <cstring>
#include "hash.h"

using namespace std;



#ifndef ALL

void test() {
    hash_table ht;
    int n = 5;
    char *s[n] = {
        "hello world",
        "hi",
        "my name is Jcyoung",
        "my name is iron man",
        "hi"
    };
    for (int i = 0; i < n; ++i) 
        ht.insert(s[i]);
    for (int i = 0; i < n; ++i)
        printf("%s\n", ht.table[ht.find(s[i])]);
    printf("%d\n", ht.size);
    ht.clear();
    printf("%d\n", ht.size);
    return;
}

int main() {
    test();
    return 0;
}

#endif