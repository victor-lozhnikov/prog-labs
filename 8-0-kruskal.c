#include <stdio.h>
#include <malloc.h>

#define INT_MAX 2147483647

typedef struct edge {
    int from;
    int to;
    int weight;
} edge;

void swap(void *a, void *b, size_t size) {
    char tmp;
    size_t i;
    for (i = 0; i < size; i++) {
        tmp = *((char*) b + i);
        *((char*) b + i) = *((char*) a + i);
        *((char*) a + i) = tmp;
    }
}

int partition (edge* a, int l, int r) {
    edge v = a[(l + r)/2];
    int i = l;
    int j = r;
    while (i <= j) {
        while (a[i].weight < v.weight) {
            i++;
        }
        while (a[j].weight > v.weight) {
            j--;
        }
        if (i >= j) {
            break;
        }
        swap (&a[i], &a[j], sizeof(edge));
        i++;
        j--;
    }
    return j;
}

void quickSort (edge* a, int l, int r) {
    if (l < r) {
        int q = partition(&a[0], l, r);
        quickSort(a, l, q);
        quickSort(a, q + 1, r);
    }
}

int find_set (int* parent, int v) {
    if (v == parent[v]) {
        return v;
    }
    return parent[v] = find_set(parent, parent[v]);
}

void union_sets (int* parent, int* rank, int v1, int v2) {
    v1 = parent[v1];
    v2 = parent[v2];
    if (v1 == v2) {
        return;
    }

    if (rank[v1] < rank[v2]) {
        swap(&v1, &v2, sizeof(int));
    }
    parent[v2] = v1;
    if (rank[v1] == rank[v2]) {
        rank[v1]++;
    }
}

int main() {
    FILE* fin = fopen ("in.txt", "r");
    FILE* fout = fopen ("out.txt", "w");

    int n, m;
    fscanf(fin, "%d\n%d\n", &n, &m);

    if (n < 0 || n > 5000) {
        fprintf(fout, "bad number of vertices");
        return 0;
    }

    if (m < 0 || m > n*(n-1)/2) {
        fprintf(fout, "bad number of edges");
        return 0;
    }

    if (n == 0 || (m == 0 && n != 1)) {
        fprintf(fout, "no spanning tree");
        return 0;
    }

    int* parent = (int*)malloc(sizeof(int)*n);
    int* rank = (int*)malloc(sizeof(int)*n);
    int* frame = (int*)malloc(sizeof(int)*m);
    edge* edges = (edge*)malloc(sizeof(edge)*m);

    if (parent == NULL || rank == NULL || frame == NULL || edges == NULL) {
        fprintf(fout, "mem error");
        return 0;
    }

    for (int i = 0; i < m; ++i) {
        if (fscanf(fin, "%d %d %d\n", &edges[i].from, &edges[i].to, &edges[i].weight) == EOF) {
            fprintf(fout, "bad number of lines");
            return 0;
        }
        if (edges[i].from < 1 || edges[i].from > n || edges[i].to < 1 || edges[i].to > n) {
            fprintf(fout, "bad vertex");
            return 0;
        }
        if (edges[i].weight < 0 || edges[i].weight > INT_MAX) {
            fprintf(fout, "bad length");
            return 0;
        }
        edges[i].from--;
        edges[i].to--;
    }

    if (n == 1) {
        return 0;
    }

    if (m < n - 1) {
        fprintf(fout, "no spanning tree");
        return 0;
    }

    quickSort(&edges[0], 0, m - 1);

    for (int i = 0; i < n; ++i) {
        parent[i] = i;
        rank[i] = 0;
    }

    int frame_pos = 0;

    for (int i = 0; i < m; ++i) {
        if (find_set(&parent[0], edges[i].from) == find_set(&parent[0], edges[i].to)) {
            continue;
        }
        union_sets(&parent[0], &rank[0], edges[i].from, edges[i].to);
        frame[frame_pos] = i;
        frame_pos++;
    }

    int root = parent[0];
    for (int i = 0; i < n; ++i) {
        if (root != find_set(&parent[0], i)) {
            fprintf(fout, "no spanning tree");
            return 0;
        }
    }

    for (int i = 0; i < frame_pos; ++i) {
        fprintf(fout, "%d %d\n", edges[frame[i]].from + 1, edges[frame[i]].to + 1);
    }
}
