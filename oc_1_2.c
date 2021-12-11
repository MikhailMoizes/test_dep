#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

const int code[32] = { 30, 0, 1, 22, 4, 5, 20, 3, 21, 8, 9, 10, 11, 12, 13, 14, 15, 31, 16, 17, 18, 19, 6, 2, 28, 27, 7, 24, 29, 25, 23, 26 };

unsigned char recode(unsigned char sym);
unsigned char* input();
unsigned char* reloc(unsigned char* str, size_t size);

void output(unsigned char *str);
void decode(unsigned char *str);

int main(int argc, unsigned char ** argv) {
    unsigned char *str;
    if ((str = input()) == NULL) {
        return -1;
    }
    // output(str);
    decode(str);
    output(str);
    return 0;
}

void decode(unsigned char *str) {
    size_t i = 0, len = strlen(str);
    for (i; i < len; ++i) {
        str[i] = recode(str[i]);
    }
}

unsigned char recode(unsigned char sym) {
    unsigned char c = sym;
    if (c > 191) {
        if (c < 224 && c > 191) {
            c = code[c - 192] + 224;
        } else {
            c = code[c-224] + 192;
        }
    }
    return c;

}

void output(unsigned char *str) {
    size_t i = 0, len = strlen(str);
    write(1, str, len);
    putchar('\n');
}

unsigned char* input() {
    size_t i = 0;
    unsigned char sym, *str;
    if ((str = (unsigned char*)malloc(1)) == NULL) {
        return NULL;
    }
    while ((sym = getchar()) != '\n') {
        str[i] = sym;
        str = reloc(str, i);
        ++i;
    }
    str[i] = '\0';
    return str;

}

unsigned char* reloc(unsigned char* str, size_t size) {
    unsigned char* new_str;
    if ((new_str= (unsigned char*)malloc(size)) == NULL) {
        return NULL;
    }

    strcpy(new_str, str);
    free(str);

    return new_str;
}