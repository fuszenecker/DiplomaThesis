#include <num_conv.h>

unsigned int hex_decode(char digit) {
    switch(digit) {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'A':
        case 'a':
            return 10;
        case 'B':
        case 'b':
            return 11;
        case 'C':
        case 'c':
            return 12;
        case 'D':
        case 'd':
            return 13;
        case 'E':
        case 'e':
            return 14;
        case 'F':
        case 'f':
            return 15;
        default:
            return -1;
    }
}

unsigned int hex2num(char *str, int digits) {
    int k;
    unsigned int value = 0;

    for (k = 0; k < digits; k++) {
        value <<= 4;
        value |= hex_decode(str[k]);
    }

    return value;
}

const char xtab[] = "0123456789ABCDEF";

unsigned int num2hex(unsigned int num, char *str, unsigned int digits) {
    int k;

    for (k = digits - 1; k >= 0; k--) {
        str[digits - k - 1] = xtab[(num >> (4 * k)) & 0x0f];
    }

    str[digits] = 0;

    return 0;
}

