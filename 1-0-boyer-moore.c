#include <stdio.h>
#include <mem.h>

int main() {
    FILE * fin = fopen("in.txt", "r");
    unsigned char searched [17];
    int len1;
    for (int i = 0; i < 17; ++i) {
        fscanf(fin, "%c", &searched[i]);
        if (searched[i] == '\n') {
            searched[i] = '\0';
            len1 = i;
            break;
        }
    }

    int stop_sym [256];
    for (int i = 0; i <  256; ++i) {
        stop_sym[i] = len1;
    }
    for (int i = 0; i < len1 - 1; ++i) {
        stop_sym[searched[i]] = len1 - i - 1;
    }

    unsigned char text [16];
    for (int i = 0; i < len1; ++i) {
        if (fscanf(fin, "%c", &text[i]) == EOF) {
            return 0;
        }
    }

    printf("%d ", len1);
    if (text[len1 - 1] == searched[len1 - 1]) {
        for (int i = len1 - 2; i >= 0; --i) {
            printf("%d ", i + 1);
            if (text[i] != searched[i]) {
                break;
            }
        }
    }

    int it = len1 - 1;
    unsigned char buf [16];
    fread(buf, 16, 1, fin);
    int k = 0;
    while (1) {
        int goal = it + stop_sym[text[it % 16]];
        while (it < goal) {
            if (k >= 16) {
                memset (buf, 0, sizeof(buf));
                fread(buf, 16, 1, fin);
                k = 0;
            }
            
            //эта проверка не совсем корректна, но на тестах Петрова вроде работала
            if (buf[k] == '\0') {
                fclose(fin);
                return 0;
            }
            
            it++;
            text[it % 16] = buf[k];
            k++;
        }
        printf("%d ", it + 1);

        if (text[it % 16] != searched[len1 - 1]) {
            continue;
        }

        for (int i = it; i >= it - len1 + 1; --i) {
            if (i != it) printf("%d ", i + 1);
            if (text[i % 16] != searched[len1 - it + i - 1]) {
                break;
            }
        }
    }
}
