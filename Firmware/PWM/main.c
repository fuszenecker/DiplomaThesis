// ---------------------------------------------------------------------------
// This file contains the "main()" function of the microcontroller.
// ---------------------------------------------------------------------------

#include <config.h>
#include <sysinit.h>

#include <irq.h>
#include <gpio.h>
#include <clock.h>
#include <pwm.h>

extern const unsigned int f_cpu;                      // Frequency of CPU clock

#define N   50000000

// ---------------------------------------------------------------------------
// The "main()" function is started by the "sysinit()" routine.
// The "sysinit()" is the function that is started when the MCU starts.
// ---------------------------------------------------------------------------

int main() {
    unsigned int value = 0, g;

    // pwm_set_duty_cycle_ch1(PWM_PERIOD / 2);

    // Until I realize nirvana...
    while (1) {
        pwm_set_duty_cycle_ch1(PWM_PERIOD / 2);

        for (g = 0; g < N; g++) 
            asm ("nop");

        pwm_set_duty_cycle_ch1(3 * PWM_PERIOD / 4);

        for (g = 0; g < N; g++) 
            asm ("nop");
    }
}

