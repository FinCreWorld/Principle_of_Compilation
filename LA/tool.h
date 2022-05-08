#ifndef TOOL_H
#define TOOL_H

#define ALL

#include <vector>
using namespace std;

typedef long long LL;
LL s2d(char *s, int ind);
double s2f(char *s);

template <class T>
class my_stack {
public:
    vector<T> table;
    int top_pos;
    my_stack();
    void pop();
    void push(T);
    T top();
    T top(int pos);
    bool empty();
};

template <class T>
my_stack<T>::my_stack() {
    table.clear();
    top_pos = 0;
}

template<class T>
void my_stack<T>::push(T t) {
    if (table.size() <= top_pos)
        table.push_back(t);
    else
        table[top_pos] = t;
    ++top_pos;
}

template<class T>
void my_stack<T>::pop() {
    --top_pos;
    if (top_pos < 0) {
        printf("stack is already empty, but still pop\n");
        system("pause");exit(-1);
    }
}

template<class T>
T my_stack<T>::top() {
    return table[top_pos - 1];
}

template<class T>
T my_stack<T>::top(int pos) {
    if (top_pos - pos < 0) {
        printf("wrong stack top num\n");
        system("pause");exit(-1);
    }
    return table[top_pos - pos];
}

template<class T>
bool my_stack<T>::empty() {
    return top_pos <= 0;
}


#endif