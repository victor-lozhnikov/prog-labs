#include <stdio.h>
#include <stdbool.h>
#include <mem.h>
#include <malloc.h>
#include <math.h>

#define INF (int)1e9
#define BUF_SIZE (int)1e4
#define uc unsigned char

typedef struct node node;
struct node {
    node* to1;
    node* to0;
    int sum;
    bool is_leaf;
    uc symb;
    char* code;
};

void char_to_bin (uc c, char* bin_code) {
    int k = 128;
    for (int i = 0; i < 8; ++i) {
        bin_code[i] = (bool)(c&k) ? (char)'1' : (char)'0';
        k>>=1;
    }
    bin_code[8] = '\0';
}

void bin_to_char (const char* bin, uc* c) {
    int res = 0;
    for (int i = 0; i < 8; ++i) {
        res += (int) pow(2, 7-i) * (bin[i] - '0');
    }
    *c = (uc) res;
}

int print_bin (char* bin, FILE* fout, FILE* log) {
    int size = strlen(bin);
    int bin_pos = 0;
    int k = 0;
    size_t zip_pos = 0;
    uc* zipped = (uc*)malloc(sizeof(uc)*(BUF_SIZE + 1));
    if (zipped == NULL) {
        fprintf(log, "mem error");
        exit(0);
    }
    while (bin_pos < size) {
        char bin8[9];
        for (int i = 0; i < 8; i++, bin_pos++) {
            if (bin_pos >= size) {
                bin8[i] = '0';
                k++;
                continue;
            }
            bin8[i] = bin[bin_pos];
        }
        bin8[8] = '\0';
        uc tmp;
        bin_to_char(bin8, &tmp);
        zipped[zip_pos] = tmp;
        zipped[zip_pos+1] = '\0';
        zip_pos++;
    }
    fwrite(zipped, sizeof(unsigned char), zip_pos, fout);
    free(zipped);
    return k;
}

node* build_tree (node* mas, int symb_sum) {
    node* min1 = (mas[0].sum < mas[1].sum) ? &mas[0] : &mas[1];
    node* min2 = (mas[0].sum < mas[1].sum) ? &mas[1] : &mas[0];
    for (int i = 2; i < symb_sum; ++i) {
        if (mas[i].sum < min1->sum && mas[i].sum < min2->sum) {
            min2 = min1;
            min1 = &mas[i];
        }
        else if (mas[i].sum < min2->sum) {
            min2 = &mas[i];
        }
    }

    if (min2->sum == INF) {
        return min1;
    }

    mas[symb_sum].is_leaf = false;
    mas[symb_sum].to1 = min2;
    mas[symb_sum].to0 = min1;
    mas[symb_sum].sum = min1->sum + min2->sum;
    symb_sum++;

    min1->sum = INF;
    min2->sum = INF;

    return build_tree(mas, symb_sum);
}

void get_code (node* cur_node, char cur_code[], int cur_pos) {
    if (cur_node->is_leaf) {
        if (cur_pos == 0) {
            strcpy(cur_node->code, "1\0");
            return;
        }
        cur_code[cur_pos] = '\0';
        strcpy(cur_node->code, cur_code);
        return;
    }

    cur_code[cur_pos] = '0';
    get_code(cur_node->to0, cur_code, cur_pos+1);
    cur_code[cur_pos] = '1';
    get_code(cur_node->to1, cur_code, cur_pos+1);
}

void make_dir (node* cur_node, char* cur_dir, int* cur_size) {
    if (cur_node->is_leaf) {
        cur_dir[*cur_size] = '0';
        (*cur_size)++;
        char bin_code[9];
        char_to_bin(cur_node->symb, &bin_code[0]);
        for (int i = 0; i < 8; ++i) {
            cur_dir[*cur_size + i] = bin_code[i];
            cur_dir[*cur_size + i + 1] = '\0';
        }
        *cur_size += 8;
        return;
    }

    cur_dir[*cur_size] = '1';
    (*cur_size)++;
    if (cur_node->to1->is_leaf) {
        cur_dir[*cur_size] = '0';
        (*cur_size)++;
        char bin_code[9];
        char_to_bin(cur_node->to1->symb, &bin_code[0]);
        for (int i = 0; i < 8; ++i) {
            cur_dir[*cur_size + i] = bin_code[i];
            cur_dir[*cur_size + i + 1] = '\0';
        }
        *cur_size += 8;
    }
    else {
        make_dir(cur_node->to1, cur_dir, cur_size);
    }

    if (cur_node->to0->is_leaf) {
        cur_dir[*cur_size] = '0';
        (*cur_size)++;
        char bin_code[9];
        char_to_bin(cur_node->to0->symb, &bin_code[0]);
        for (int i = 0; i < 8; ++i) {
            cur_dir[*cur_size + i] = bin_code[i];
            cur_dir[*cur_size + i + 1] = '\0';
        }
        *cur_size += 8;
    }
    else {
        make_dir(cur_node->to0, cur_dir, cur_size);
    }
}

void zip (FILE* fin, FILE* fout, FILE* log) {
    node* nodes = (node*)malloc(sizeof(node)*513);
    if (nodes == NULL) {
        fprintf(log, "mem error");
        exit(0);
    }
    for (int i = 0; i < 256; ++i) {
        nodes[i].is_leaf = false;
        nodes[i].to0 = NULL;
        nodes[i].to1 = NULL;
        nodes[i].symb = (uc)i;
        nodes[i].sum = 0;
        nodes[i].code = (char*)malloc(sizeof(char)*100);
        if (nodes[i].code == NULL) {
            fprintf(log, "mem error");
            exit(0);
        }
    }

    uc* text = (uc*)malloc(sizeof(uc)*(BUF_SIZE + 1));
    if (text == NULL) {
        fprintf(log, "mem error");
        exit(0);
    }
    int to_zip_size = 0;

    while(1) {
        int fsize = fread(text, sizeof(uc), BUF_SIZE, fin);
        if (fsize == 0) break;

        for (int i = 0; i < fsize; ++i) {
            nodes[(int)text[i]].sum++;
            nodes[(int)text[i]].is_leaf = true;
            to_zip_size++;
        }
    }

    for (int i = 0; i < 256; ++i) {
        if (nodes[i].sum == 0) {
            nodes[i].sum = INF;
            continue;
        }
    }
    int symb_sum = 256;

    node* root = build_tree(&nodes[0], symb_sum);

    char start_code[101];
    get_code(root, start_code, 0);

    char* bin_zip = (char*)malloc(sizeof(char)*(8*(BUF_SIZE + 1)) + 1);
    if (bin_zip == NULL) {
        fprintf(log, "mem error");
        exit(0);
    }
    int code_size = 0;
    make_dir(root, bin_zip, &code_size);

    fseek(fin, 0, SEEK_SET);
    unsigned char tmp[4];
    fread(tmp, sizeof(unsigned char), 3, fin);

    int fsize = fread(text, sizeof(uc), BUF_SIZE, fin);

    for (int i = 0; i < fsize; ++i) {
        for (int j = 0; j < strlen(nodes[text[i]].code); ++j) {
            bin_zip[code_size] = nodes[text[i]].code[j];
            bin_zip[code_size + 1] = '\0';
            code_size++;
            if (code_size == 8*BUF_SIZE) {
                print_bin(bin_zip, fout, log);
                code_size = 0;
                bin_zip[0] = '\0';
            }
        }
        if (i == fsize - 1) {
            fsize = fread(text, sizeof(uc), BUF_SIZE, fin);
            i = -1;
        }
    }

    tmp[0] = (uc)(print_bin(bin_zip, fout, log) + '0');
    fwrite(tmp, sizeof(unsigned char), 1, fout);
    free(text);
    free(bin_zip);
}

node* build_unzip_tree (const char* bin, int* cur_pos, node* cur_node, int* free_node_pos, node* nodes) {
    if (bin[*cur_pos] == '0') {
        cur_node->is_leaf = true;
        (*cur_pos)++;
        char bin8[9];
        for (int i = 0; i < 8; i++, (*cur_pos)++) {
            bin8[i] = bin[*cur_pos];
        }
        bin8[8] = '\0';
        bin_to_char(bin8, &cur_node->symb);
    }

    else {
        cur_node->is_leaf = false;
        (*cur_pos)++;
        cur_node->to1 = &nodes[*free_node_pos];
        (*free_node_pos)++;
        build_unzip_tree(bin, cur_pos, cur_node->to1, free_node_pos, nodes);
        cur_node->to0 = &nodes[*free_node_pos];
        (*free_node_pos)++;
        build_unzip_tree(bin, cur_pos, cur_node->to0, free_node_pos, nodes);

    }
}

void decode (FILE* fout, char* bin, int* cur_pos, node* root, int end_pos, int last_byte, FILE* log, FILE* fin, uc* zip_text) {
    node* cur_node = root;
    uc* out = (uc*)malloc(sizeof(uc)*(8*(BUF_SIZE + 1)));
    if (out == NULL) {
        fprintf(log, "mem error");
        exit(0);
    }

    size_t out_pos = 0;

    for (; *cur_pos < end_pos; ++(*cur_pos)) {
        if (end_pos - (*cur_pos) == last_byte && end_pos < 8*(BUF_SIZE-1)) {
            fwrite(out, sizeof(uc), out_pos, fout);
            break;
        }

        if (root->is_leaf) {
            out[out_pos] = root->symb;
            out[out_pos + 1] = '\0';
            out_pos++;
        }

        else {
            if (bin[*cur_pos] == '1') {
                cur_node = cur_node->to1;
            }

            else {
                cur_node = cur_node->to0;
            }

            if (cur_node->is_leaf) {
                out[out_pos] = cur_node->symb;
                out[out_pos + 1] = '\0';
                out_pos++;
                cur_node = root;
            }
        }

        if (*cur_pos == end_pos - 1) {
            fwrite(out, sizeof(uc), out_pos, fout);
            int fsize = fread(zip_text, sizeof(uc), (size_t)(BUF_SIZE-1), fin);
            if (fsize == 0) break;
            char bin_code[9];
            char_to_bin((uc)(last_byte + '0'), &bin_code[0]);
            for (int j = 0; j < 9; ++j) {
                bin[j] = bin_code[j];
            }
            end_pos = 8;

            for (int i = 0; i < fsize - 1; ++i) {
                char_to_bin(zip_text[i], &bin_code[0]);
                for (int j = 0; j < 9; ++j) {
                    bin[end_pos + j] = bin_code[j];
                }
                end_pos += 8;
            }
            last_byte = zip_text[fsize - 1] - '0';

            *cur_pos = -1;
            out_pos = 0;
        }
    }
}

void unzip (FILE* fin, FILE* fout, FILE* log) {
    uc* zip_text = (uc*)malloc(sizeof(uc)*(BUF_SIZE + 1));
    if (zip_text == NULL) {
        fprintf(log, "mem error");
        exit(0);
    }
    char* bin_zip = (char*)malloc(sizeof(char)*(8*(BUF_SIZE + 1) + 1));
    if (bin_zip == NULL) {
        fprintf(log, "mem error");
        exit(0);
    }

    bin_zip[0] = '\0';
    int unzip_size = 0;
    int last_byte = 0;

    int fsize = fread(zip_text, sizeof(uc), BUF_SIZE, fin);
    for (int i = 0; i < fsize - 1; ++i) {
        char bin_code[9];
        char_to_bin(zip_text[i], &bin_code[0]);
        for (int j = 0; j < 9; ++j) {
            bin_zip[unzip_size + j] = bin_code[j];
        }
        unzip_size += 8;
    }
    last_byte = zip_text[fsize - 1] - '0';

    if (unzip_size == 0) exit(0);

    int cur_pos = 0;
    node* nodes = (node*)malloc(sizeof(node)*512);
    if (nodes == NULL) {
        fprintf(log, "mem error");
        exit(0);
    }

    int free_node_pos = 1;
    build_unzip_tree(bin_zip, &cur_pos, &nodes[0], &free_node_pos, &nodes[0]);
    decode(fout, bin_zip, &cur_pos, &nodes[0], unzip_size, last_byte, log, fin, zip_text);
    free(bin_zip);

}

int main() {
    FILE* fin = fopen("in.txt", "rb");
    FILE* fout = fopen("out.txt", "wb");
    FILE* log = fopen("log.txt", "w");

    fseek(fin, 0, SEEK_END);
    int file_size = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    if (file_size == 3) exit(0);

    unsigned char type[4];
    fread(type, sizeof(unsigned char), 3, fin);

    if (type[0] == 'c') {
        zip(fin, fout, log);
    }

    if (type[0] == 'd') {
        unzip(fin, fout, log);
    }
    return 0;
}
