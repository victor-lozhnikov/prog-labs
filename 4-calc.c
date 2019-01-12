#include <stdio.h>
#include <mem.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct stack {
    int mas[10000];
    int size;
} stack;

void push (stack *st, const int val) {
    st->mas[st->size] = val;
    st->size++;
}

int pop (stack *st) {
    if (st->size == 0) {
        printf("syntax error");
        exit(0);
    }
    st->size--;
    return st->mas[st->size];
}

int peek (stack *st) {
    if (st->size <= 0) {
        printf("syntax error");
        exit(0);
    }
    return st->mas[st->size - 1];
}

bool is_operator (char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')';
}

bool is_digit (char c) {
    return c >= '0' && c <= '9';
}

bool good_symb (char c) {
    return is_operator(c) || is_digit(c) || c == '\n';
}


int priority (char c) {
    if (c == '(' || c == ')') return 0;
    if (c == '+' || c == '-') return 1;
    if (c == '*' || c == '/') return 2;
}

void to_postfix (char* str, int size, char* output) {
    int out_size = 0;
    stack oper_stack;
    oper_stack.size = 0;

    for (int i = 0; i < size; ++i) {
        if (is_digit(str[i])) {
            while (!is_operator(str[i])) {
                output[out_size] = str[i];
                out_size++;
                i++;
                if (i == size) break;
            }
            output[out_size] = ' ';
            out_size++;
            i--;
        }

        if (is_operator(str[i])) {
            if (str[i] == '(') {
                push(&oper_stack, (int)str[i]);
            }
            else if (str[i] == ')') {
                if (i == 0 || str[i-1] == '(') {
                    printf("syntax error");
                    exit(0);
                }
                char s = (char)pop(&oper_stack);
                while (s != '(') {
                    output[out_size] = s;
                    output[out_size + 1] = ' ';
                    out_size += 2;
                    s = (char)pop(&oper_stack);
                }
            }
            else {
                if (oper_stack.size > 0 && priority(str[i]) <= priority((char)peek(&oper_stack))) {
                    output[out_size] = (char)pop(&oper_stack);
                    output[out_size + 1] = ' ';
                    out_size += 2;
                }
                push(&oper_stack, (int)str[i]);
            }
        }
    }

    while (oper_stack.size > 0) {
        output[out_size] = (char)pop(&oper_stack);
        output[out_size + 1] = ' ';
        out_size += 2;
    }
    output[out_size] = '\0';
}

int count (char* str) {
    int res = 0;
    stack st;
    st.size = 0;
    int str_size = strlen(str);

    for (int i = 0; i < str_size; ++i) {
        if (is_digit(str[i])) {
            int a = 0;
            while (!is_operator(str[i]) && str[i] != ' ') {
                a *= 10;
                a += (int)(str[i] - '0');
                i++;
                if (i == str_size) {
                    break;
                }
            }
            push(&st, a);
            i--;
        }
        else if (is_operator(str[i])) {
            int a = pop(&st);
            int b = pop(&st);

            if (str[i] == '+') {
                res = b + a;
            }
            if (str[i] == '-') {
                res = b - a;
            }
            if (str[i] == '*') {
                res = b * a;
            }
            if (str[i] == '/') {
                if (a == 0) {
                    printf("division by zero");
                    exit(0);
                }
                res = b / a;
            }

            push(&st, res);
        }
    }
    return peek(&st);
}

int main() {
    FILE * fin = fopen("in.txt", "r");

    fseek(fin, 0, SEEK_END);
    int size = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    char str[1000];
    for (int i = 0; i < size; ++i) {
        fscanf(fin, "%c", &str[i]);
        if (str[i] == '\n') size-=2;
        if (!good_symb(str[i])) {
            printf("syntax error");
            return 0;
        }
    }
    str[size] = '\0';

    char postfix [1000];
    to_postfix(str, size, postfix);
    printf("%d", count(postfix));
}
