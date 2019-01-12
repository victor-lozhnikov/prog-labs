#include <stdio.h>
#include <malloc.h>

void swap (int* a, int* b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition (int a[], int l, int r) {
    int v = a[(l + r)/2];
    int i = l;
    int j = r;
    while (i <= j) {
        while (a[i] < v) {
            i++;
        }
        while (a[j] > v) {
            j--;
        }
        if (i >= j) {
            break;
        }
        swap (&a[i], &a[j]);
        i++;
        j--;
    }
    return j;
}

void quickSort (int a[], int l, int r) {
    if (l < r) {
        int q = partition(a, l, r);
        quickSort(a, l, q);
        quickSort(a, q + 1, r);
    }
}

int main() {
    int size;
    scanf ("%d", &size);
    int *a = (int*) malloc(size * sizeof(int));
    for (int i = 0; i < size; ++i) {
        scanf("%d", &a[i]);
    }
    quickSort(a, 0, size - 1);

    for (int i = 0; i < size; ++i) {
        printf("%d ", a[i]);
    }
}
