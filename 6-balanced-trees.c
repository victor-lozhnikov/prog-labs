#include <stdio.h>
#include <malloc.h>

typedef struct node node;
struct node {
    node* right;
    node* left;
    int val;
    int h;
    int diff;
    int i;
};

int height (node* cur_node) {
    if (cur_node == NULL) {
        return 0;
    }
    return cur_node->h;
}

void update_height (node* cur_node) {
    cur_node->h = (height(cur_node->right) > height(cur_node->left) ? height(cur_node->right) : height(cur_node->left)) + 1;
    cur_node->diff = height(cur_node->right) - height(cur_node->left);
}

node* right_rotate (node* cur_node, node** root) {
    node* q = cur_node->left;
    cur_node->left = q->right;
    q->right = cur_node;
    update_height(cur_node);
    update_height(q);
    if ((*root)->i == cur_node->i) {
        *root = q;
    }
    return q;
}

node* left_rotate (node* cur_node, node** root) {
    node* q = cur_node->right;
    cur_node->right = q->left;
    q->left = cur_node;
    update_height(cur_node);
    update_height(q);
    if ((*root)->i == cur_node->i) {
        *root = q;
    }
    return q;
}

node* balance (node* cur_node, node** root) {
    update_height(cur_node);

    if (cur_node->diff == 2) {
        if (cur_node->right->diff < 0) {
            cur_node->right = right_rotate(cur_node->right, root);
        }
        return left_rotate(cur_node, root);
    }

    if (cur_node->diff == -2) {
        if (cur_node->left->diff > 0) {
            cur_node->left = left_rotate(cur_node->left, root);
        }
        return right_rotate(cur_node, root);
    }

    return cur_node;
}

node* add_vertex (node* cur_node, int val, int i, node* free_node, node** root) {
    if (cur_node == NULL) {
        cur_node = free_node;
        cur_node->h = 1;
        cur_node->diff = 0;
        cur_node->right = NULL;
        cur_node->left = NULL;
        cur_node->val = val;
        cur_node->i = i;
        return cur_node;
    }

    if (val > cur_node->val) {
        cur_node->right = add_vertex(cur_node->right, val, i, free_node, root);
    } else {
        cur_node->left = add_vertex(cur_node->left, val, i, free_node, root);
    }

    return balance(cur_node, root);
}

int main() {
    FILE* fin = fopen("in.txt", "r");
    FILE* fout = fopen("out.txt", "w");

    int n;
    fscanf(fin, "%d", &n);

    if (n == 0) {
        fprintf(fout, "0");
        return 0;
    }

    node* nodes = (node*)malloc(sizeof(node) * n);
    if (nodes == NULL) {
        fprintf(fout, "mem error");
        return 0;
    }
    fscanf(fin, "%d", &nodes[0].val);
    nodes[0].h = 1;
    nodes[0].diff = 0;
    nodes[0].right = NULL;
    nodes[0].left = NULL;
    nodes[0].i = 0;

    int cur_val;
    node* root = &nodes[0];

    for (int i = 1; i < n; ++i) {
        fscanf(fin, "%d", &cur_val);
        add_vertex(root, cur_val, i, &nodes[i], &root);
    }

    fprintf(fout, "%d", root->h);
    return 0;
}
