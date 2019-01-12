#include <stdio.h>
#include <mem.h>
#include <math.h>

int hash (const unsigned char s[], const int size) {
    int h = 0, p = 1;
    for (int i = 0; i < size; ++i) {
        h += ((int)s[i] % 3) * p;
        p *= 3;
    }
    return h;
}

int main() {
    FILE * fin = fopen("in.txt", "r");

    fseek(fin, 0, SEEK_END);
    long long fsize = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    unsigned char subs [17];
    int subsize = 0;
    for (int i = 0; i < 17; ++i) {
        fscanf(fin, "%c", &subs[i]);
        if (subs[i] == '\n') {
            subs[i] = '\0';
            subsize = i;
            break;
        }
    }
    int subhash =  hash(subs, subsize);
    printf("%d ", subhash);

    unsigned char text [17];
    for (int i = 0; i < subsize; ++i) {
        if (fscanf(fin, "%c", &text[i]) == EOF) {
            fclose(fin);
            return 0;
        }
    }
    text[subsize] = '\0';
    int texthash = hash(text, subsize);
    int it = 0;

    if (subhash == texthash) {
        for (int i = it; i < subsize; ++i) {
            printf("%d ", i + 1);
            if (subs[i] != text[i]) {
                break;
            }
        }
    }

    int spow = 1;
    for (int i = 0; i < subsize - 1; ++i) {
        spow *= 3;
    }

    unsigned char buf [1000];
    fread(buf, sizeof(char), 1000, fin);
    int k = 0;

    while (1) {
        it++;
        texthash -= (int)text[0] % 3;
        texthash /= 3;
        memcpy(text, &text[1], subsize);

        if (k >= 1000) {
            memset (buf, 0, sizeof(buf));
            fread(buf, sizeof(char), 1000, fin);
            k = 0;
        }

        if (it + subsize + 2 < fsize) {
            text[subsize - 1] = buf[k];
            k++;
        }
        else {
            fclose(fin);
            return 0;
        }

        texthash += ((int)text[subsize - 1] % 3) * spow;

        if (subhash == texthash) {
            for (int i = 0; i < subsize; ++i) {
                printf("%d ", it + 1);
                if (subs[i] != text[i]) {
                    break;
                }
                it++;
            }
        }
    }
}
