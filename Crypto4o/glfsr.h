#ifndef __GLFSR_H__
#define __GLFSR_H__

#include <stdint.h>

typedef uint64_t t_glfsr_data;

typedef struct {
    t_glfsr_data data;
    t_glfsr_data polynom;
    t_glfsr_data mask;
} t_glfsr;

void glfsr_init(t_glfsr *glfsr, t_glfsr_data polynom, t_glfsr_data initial_value);
unsigned int glfsr_next_state(t_glfsr *glfsr);

#endif
