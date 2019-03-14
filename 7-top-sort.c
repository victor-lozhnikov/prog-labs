#include <stdio.h>
#include <malloc.h>

typedef struct node {
    struct node* next;
    int id;
} node;

void add_edge (FILE* fout, node** head, int id) {
    node* new_node = malloc(sizeof(node));
    if (new_node == NULL) {
        fprintf(fout, "mem error");
        exit(0);
    }

    new_node->id = id;
    new_node->next = *head;
    *head = new_node;
}

void dfs (FILE* fout, short* used, node** graph, int cur, int* output, int* cur_out) {
    if (*(used + cur) == 2) {
        return;
    }

    if (*(used + cur) == 1) {
        fprintf(fout, "impossible to sort");
        exit(0);
    }

    *(used + cur) = 1;

    node* head = *(graph + cur);
    while (head != NULL) {
        if (graph + head->id != NULL) {
            dfs(fout, used, graph, head->id, output, cur_out);
        }
        head = head->next;
    }
    *(used + cur) = 2;
    *(output + *cur_out) = cur;
    (*cur_out)++;
}

int main() {
    FILE* fin = fopen ("in.txt", "r");
    FILE* fout = fopen ("out.txt", "w");

    int n, m;
    if (fscanf (fin, "%d", &n) == EOF) {
        fprintf(fout, "bad number of lines");
        return 0;
    }
    if (fscanf (fin, "%d", &m) == EOF) {
        fprintf(fout, "bad number of lines");
        return 0;
    }

    if (n < 0 || n > 1000) {
        fprintf (fout, "bad number of vertices");
        return 0;
    }

    if (m < 0 || m > n * (n + 1) / 2) {
        fprintf (fout, "bad number of edges");
        return 0;
    }

    node** graph = malloc (sizeof (node*) * n);
    if (graph == NULL) {
        fprintf (fin, "mem error");
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        *(graph + i) = NULL;
    }

    int from, to;
    for (int i = 0; i < m; ++i) {
        if (fscanf (fin, "%d%d", &from, &to) == EOF) {
            fprintf (fout, "bad number of lines");
            return 0;
        }

        if (from < 1 || from > n || to < 1 || to > n) {
            fprintf (fout, "bad vertex");
            return 0;
        }

        add_edge (fout, &graph[from - 1], to - 1);
    }

    short* used = malloc (sizeof(short) * n);
    for (int i = 0; i < n; ++i) {
        used[i] = 0;
    }

    int* output = malloc (sizeof(int) * n);
    int cur_out = 0;

    for (int i = 0; i < n; ++i) {
        if (graph[i] != NULL) {
            dfs (fout, used, graph, i, output, &cur_out);
        }
    }

    for (int i = cur_out - 1; i >= 0; --i) {
        fprintf(fout, "%d ", output[i] + 1);
    }
    for (int i = 0; i < n; ++i) {
        if (used[i] == 0) {
            fprintf(fout, "%d ", i + 1);
        }
    }
    return 0;
}
