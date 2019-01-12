#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

typedef unsigned long long li64;

int main() {
    int b1, b2;
    char num1[20], num2[100];

    scanf("%d%d%s", &b1, &b2, num1);

    if (b1 < 2 || b1 > 16 || b2 < 2 || b2 > 16) {
        printf ("bad input");
        return 0;
    }

    int dotIndex = -1, numSize = 0;
    bool withDot = false;

    for (int i = 0; num1[i] != '\0'; i++) {
        num1[i] = tolower(num1[i]);
        numSize++;
        if (num1[i] == '.') {
            if (withDot) {
                printf("bad input");
                return 0;
            }
            dotIndex = i;
            withDot = true;
            continue;
        }

        if (num1[i] - '0' < 10 && (num1[i] - '0' > b1 - 1 || num1[i] - '0' < 0)) {
            printf("bad input");
            return 0;
        }

        if (num1[i] - '0' > 10 && (num1[i] - 'a' + 10 > b1 - 1 || num1[i] - 'a' + 10 < 10)) {
            printf("bad input");
            return 0;
        }
    }

    if (dotIndex == 0 || dotIndex == numSize - 1) {
        printf("bad input");
        return 0;
    }

    if (dotIndex == -1) {
        dotIndex = numSize;
        withDot = false;
    }

    if (b1 == b2) {
        printf("%s", num1);
        return 0;
    }

    li64 beforeDot = 0;
    double afterDot = 0, b1Pow = 1;

    //Переводим в десятичную часть до точки
    for (int i = dotIndex - 1; i >= 0; i--) {
        if (num1[i] - '0' < 10) {
            beforeDot += (num1[i] - '0') * (li64)b1Pow;
        }
        else {
            beforeDot += (num1[i] - 'a' + 10) * (li64)b1Pow;
        }
        b1Pow *= b1;
    }

    b1Pow = 1.0/b1;

    //Переводим в десятичную часть после точки
    for (int i = dotIndex + 1;  i < numSize; i++) {
        if (num1[i] - '0' < 10) {
            afterDot += (num1[i] - '0') * b1Pow;
        }
        else {
            afterDot += (num1[i] - 'a' + 10) * b1Pow;
        }
        b1Pow /= b1;
    }

    int tempNum[100];
    numSize = 0;

    if (beforeDot == 0) {
        tempNum[0] = 0;
        numSize = 1;
    }

    //Переводим в b2-ичную часть до точки
    for (int i = 0; beforeDot > 0; i++) {
        tempNum[i] = beforeDot % b2;
        numSize++;
        beforeDot /= b2;
    }

    //Делаем разворот и переводим в char часть до точки
    for (int i = 0; i < numSize; i++) {
        if (tempNum[numSize - i - 1] < 10) {
            num2[i] = tempNum[numSize - i - 1] + '0';
        }
        else {
            num2[i] = tempNum[numSize - i - 1] + 'a' - 10;
        }
    }

    if (!withDot || afterDot == 0.0) {
        num2[numSize] = '\0';
        printf ("%s", num2);
        return 0;
    }

    num2[numSize] = '.';

    //Переводим в b2-ичную часть после точки
    for (int i = numSize + 1; i < numSize + 13; i++) {
        afterDot *= b2;
        tempNum[i] = (int)afterDot;
        afterDot -= (int)afterDot;
        if (tempNum[i] < 10) {
            num2[i] = tempNum[i] + '0';
        }
        else {
            num2[i] = tempNum[i] - 10 + 'a';
        }
    }

    num2[numSize + 13] = '\0';
    printf ("%s", num2);
    return 0;
}
