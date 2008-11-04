#include <glfsr.h>

void glfsr_init(t_glfsr *glfsr, t_glfsr_data polynom, t_glfsr_data initial_value) {
    t_glfsr_data initial_mask;
    unsigned int c;

    glfsr->polynom = polynom | 1;
    glfsr->data = initial_value;

    c = 8 * sizeof(t_glfsr_data) - 1;

    initial_mask = 1;
    initial_mask <<= c;

    for (; c >= 0; c--) {
        if (polynom & initial_mask) {
            glfsr->mask = initial_mask;
            break;
        }

        initial_mask >>= 1;
    }
}

unsigned int glfsr_next_state(t_glfsr *glfsr) {
    unsigned int retval;
    glfsr->data <<= 1;

    if (glfsr->data & glfsr->mask) {
        retval = 1;
        glfsr->data ^= glfsr->polynom;
    } else
        retval = 0;

    return retval;
}
