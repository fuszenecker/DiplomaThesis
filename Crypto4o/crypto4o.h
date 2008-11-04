#ifndef __CRYPTO4O_H__
#define __CRYPTO4O_H__

#include <glfsr.h>

typedef struct {
    t_glfsr glfsr_d0;
    t_glfsr glfsr_c0;
    t_glfsr_data init_value_d;
    t_glfsr_data init_value_c;
} t_sprng;

void sprng_init(t_sprng *sprng, t_glfsr_data i_polynom_d, t_glfsr_data i_init_value_d, t_glfsr_data i_polynom_c, t_glfsr_data i_init_value_c);

unsigned char sprng_next_byte(t_sprng *sprng);

#endif
