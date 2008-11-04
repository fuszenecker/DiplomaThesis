#include <stdio.h>
#include <stdlib.h>
#include <glfsr.h>
#include <crypto4o.h>

unsigned int bitcounter = 0;

t_sprng sprng;

void sprng_init(t_sprng *sprng, t_glfsr_data polynom_d, t_glfsr_data init_value_d, t_glfsr_data polynom_c, t_glfsr_data init_value_c) {
    glfsr_init(&(sprng->glfsr_d0), polynom_d, init_value_d ? init_value_d : 1);
    glfsr_init(&(sprng->glfsr_c0), polynom_c, init_value_c ? init_value_c : 1);
}

unsigned char sprng_next_byte(t_sprng *sprng) {
    unsigned char byte = 0;
    char bitpos = 7;
    unsigned char bit0, bitc0, bitr;

    do {
        bit0 = glfsr_next_state(&(sprng->glfsr_d0));
        bitc0 = glfsr_next_state(&(sprng->glfsr_c0));

#ifdef __DEBUG__
        if (sprng->glfsr_d0.data == sprng->init_value_d) {
            printf("GLFSR D0 overflow at %d.\n", bitcounter);
        }

        if (sprng->glfsr_c0.data == sprng->init_value_c) {
            printf("GLFSR C0 overflow at %d.\n", bitcounter);
        }

        if ((sprng->glfsr_d0.data == sprng->init_value_d) && (sprng->glfsr_c0.data == sprng->init_value_c)) {
            printf("**** GLFSR overflow at %d. ****\n", bitcounter);
        }

        printf("Next bits (%d): %d, %d\t= ", bitpos, bit0, bitc0);
#endif

        if (bitc0) {
            bitr = bit0;

#ifdef __DEBUG__
            printf("%d\n", bitr);
#endif

            byte |= bitr << bitpos;
            bitpos--;
        } else {
#ifdef __DEBUG__
            printf("dropped\n");
#endif
        }

        bitcounter++;
    } while (bitpos >= 0);

    return byte;
}
