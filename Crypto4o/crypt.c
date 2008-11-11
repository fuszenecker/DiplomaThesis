#include <stdio.h>
#include <stdlib.h>
#include <glfsr.h>
#include <crypto4o.h>

extern unsigned int bitcounter;
t_sprng sprng;

unsigned int hex2bin(char ch) {
    switch (ch | 0x20) {
        case '0': return 0; break;
        case '1': return 1; break;
        case '2': return 2; break;
        case '3': return 3; break;
        case '4': return 4; break;
        case '5': return 5; break;
        case '6': return 6; break;
        case '7': return 7; break;
        case '8': return 8; break;
        case '9': return 9; break;
        case 'a': return 10; break;
        case 'b': return 11; break;
        case 'c': return 12; break;
        case 'd': return 13; break;
        case 'e': return 14; break;
        case 'f': return 15; break;
    }

    return -1;
}

t_glfsr_data hex2num(char *str) {
    t_glfsr_data retval = 0;
    char *p;

    for (p = str; *p; p++) {
        retval <<= 4;
        retval |= hex2bin(*p);
    }

    return retval;
}

int main(int argc, char **argv) {
    FILE *f, *g;
    unsigned int input_ch, random_ch;
    unsigned int bytecounter;
    t_glfsr_data data_polynom, data_init_value, 
                 control_polynom, control_init_value;

    if (argc < 7) {
        fprintf(stderr, "Too few arguments: input_file output_file data_polynom data_init_value control_polynom control_init_value\n");
        exit(1);
    }

    f = fopen(argv[1], "r");

    if (f == NULL) {
        fprintf(stderr, "Cannot open input data file (%s).\n", argv[1]);
        exit(2);
    }

    g = fopen(argv[2], "w");

    if (f == NULL) {
        fprintf(stderr, "Cannot open output data file (%s).\n", argv[2]);
        exit(2);
    }

    if ((argv[3][0] != '0') || ((argv[3][1] | 0x20) != 'x')) {
        fprintf(stderr, "Argument 3 is not in hexadecimal format (%s).\n", argv[3]);
        exit(2);
    }

    if ((argv[4][0] != '0') || ((argv[4][1] | 0x20) != 'x')) {
        fprintf(stderr, "Argument 4 is not in hexadecimal format (%s).\n", argv[4]);
        exit(2);
    }

    if ((argv[5][0] != '0') || ((argv[5][1] | 0x20) != 'x')) {
        fprintf(stderr, "Argument 5 is not in hexadecimal format (%s).\n", argv[5]);
        exit(2);
    }

    if ((argv[6][0] != '0') || ((argv[6][1] | 0x20) != 'x')) {
        fprintf(stderr, "Argument 6 is not in hexadecimal format (%s).\n", argv[6]);
        exit(2);
    }

    data_polynom = hex2num(argv[3] + 2);
    data_init_value = hex2num(argv[4] + 2);
    control_polynom = hex2num(argv[5] + 2);
    control_init_value = hex2num(argv[6] + 2);

    printf("GLFSR D0: using polynom 0x%08X%08X, initial value: 0x%08X%08X.\n",
                    (unsigned int) (data_polynom >> 32),
                    (unsigned int) (data_polynom & 0xffffffff),
                    (unsigned int) (data_init_value >> 32),
                    (unsigned int) (data_init_value & 0xffffffff));

    printf("GLFSR C0: using polynom 0x%08X%08X, initial value: 0x%08X%08X.\n",
                    (unsigned int) (control_polynom >> 32),
                    (unsigned int) (control_polynom & 0xffffffff),
                    (unsigned int) (control_init_value >> 32),
                    (unsigned int) (control_init_value & 0xffffffff));

    sprng_init(&sprng, data_polynom, data_init_value,
               control_polynom, control_init_value);

    for (bytecounter = 0; !feof(f); bytecounter++) {
        input_ch = fgetc(f) & 0xff;

        if ((input_ch == EOF) || feof(f))
            break;

        random_ch = sprng_next_byte(&sprng);

#ifdef __DEBUG__
        printf("Next bytes: 0x%02X, 0x%02X\t= 0x%02X.\n",
            input_ch, random_ch, (input_ch ^ random_ch) & 0xff);
#endif

        fputc((input_ch ^ random_ch) & 0xff, g);
    }

    fclose(f);
    fclose(g);

    printf("En/DeCrypted: %d bytes (%d bits, using %d random bits [%0.3f]).\n",
        bytecounter, bytecounter * 8, bitcounter,
        1.0 * bitcounter / (bytecounter * 8));

    return 0;
}
