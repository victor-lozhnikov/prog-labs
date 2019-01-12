#include <stdio.h>
#include <malloc.h>

void swap (int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void siftDown (int a[], const int size, int i) {
    while (2 * i + 1 < size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int j = left;
        if (right < size && a[right] > a[left]) {
            j = right;
        }
        if (a[i] >= a[j]) {
            break;
        }
        swap(&a[i], &a[j]);
        i = j;
    }
}

void heapify (int a[], const int size) {
    for (int i = size / 2; i >= 0; --i) {
        siftDown(a, size, i);
    }
}

int main() {
    int size;
    scanf ("%d", &size);
    int *a = (int*) malloc(size * sizeof(int));
    for (int i = 0; i < size; ++i) {
        scanf("%d", &a[i]);
    }

    heapify(a, size);
    int heapsize = size;

    for (int i = 0; i < size - 1; ++i) {
        swap(&a[0], &a[size - 1 - i]);
        heapsize--;
        siftDown(a, heapsize, 0);
    }

    for (int i = 0; i < size; ++i) {
        printf("%d ", a[i]);
    }
    free(a);
}
