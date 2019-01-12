#include <stdio.h>
#include <stdbool.h>

void swap (int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

bool next_perm (int a[], const int size) {
    for (int i = size - 2; i >= 0; --i) {
        if (a[i] >= a[i + 1]) continue;

        int min = i + 1;

        for (int j = i + 1; j < size; ++j) {
            if (a[j] < a[min] && a[j] > a[i]) {
                min = j;
            }
        }
        swap (&a[i], &a[min]);

        for (int j = 1; j <= (size-i)/2; j++) {
            swap (&a[i + j], &a[size - j]);
        }
        return true;
    }
    return false;
}

int main() {
    char in [20];
    int size = 0;
    while (in[size - 1] != '\n') {
        scanf("%c", &in[size]);
        size++;
    }
    in[size - 1] = '\0';
    size--;
    int mas [size];

    bool w[10] = {false};

    for (int i = 0; i < size; ++i) {
        if (in[i] < '0' || in[i] > '9') {
            printf("bad input");
            return 0;
        }
        if (w[in[i] - '0']) {
            printf("bad input");
            return 0;
        }
        mas[i] = in[i] - '0';
        w[mas[i]] = true;
    }

    int count;
    scanf("%d", &count);
    for (int i = 0; i < count; ++i) {
        if (!next_perm(mas, size)) return 0;
        for (int j = 0; j < size; ++j) {
            printf("%d", mas[j]);
        }
        printf("\n");
    }

}
