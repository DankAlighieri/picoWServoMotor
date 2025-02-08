// Calcular divisor de clock
/*
    Frequência do PWM = 50Hz
    Período do PWM = 20ms
    clock_div = sys_clock/{freq_pwm * (wrap + 1)}
    wrap = 0xffff (65535)
    sys_clock = 125*10^6
    clock_div = 38,147
*/

// Calcular duty cycle p/ cada ciclo ativo
/*
    Ton 180 = 2400us = 2,4ms
    Ton 90 = 1470us = 1,47ms
    Ton 0 = 500us = 0,5ms
    duty_cycle = (Ton/period_pwm) * (wrap + 1)
    period_pwm = 20ms
    wrap = 65535
    duty_cycle 180 = (2,4/20) *(65335 + 1) =~ 7864
    duty_cycle 90 = (1,47/20) *(65335 + 1) =~ 4817
    duty_cycle 0 = (0,5/20) *(65335 + 1) =~ 1638
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 28
#define WRAP 65535

#define ACTIVE_CYCLE180 2.4
#define ACTIVE_CYCLE90 1.47
#define ACTIVE_CYCLE0 .5

#define PWM_PERIOD 20
#define CLK_DIV 38.147f

#define DUTY_CYCLE_180 7864
#define DUTY_CYCLE_90 4817
#define DUTY_CYCLE_0 1638

uint setup_servo_pwm() {
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(SERVO_PIN);
    pwm_config config = pwm_get_default_config();

    pwm_config_set_clkdiv(&config, CLK_DIV);
    pwm_init(slice, &config, true);

    return slice;
}

void initial_servo_routine(){
    pwm_set_gpio_level(SERVO_PIN, DUTY_CYCLE_180);
    printf("Servo em 180 graus\n");
    sleep_ms(1000);
    printf("Servo em 90 graus\n");
    pwm_set_gpio_level(SERVO_PIN, DUTY_CYCLE_90);
    sleep_ms(1000);
    printf("Servo em 0 graus\n");
    pwm_set_gpio_level(SERVO_PIN, DUTY_CYCLE_0);
    sleep_ms(1000);
}

void periodic_servo_motion() {
    float active_cycle = ACTIVE_CYCLE0; // 500us
    uint16_t duty_cycle;
    bool full_range = false;

    while (!full_range) {
        active_cycle += 0.005; // Increment the active cycle by 5us
        printf("Ciclo ativo: %.2f\n", active_cycle);
        duty_cycle = (uint16_t)((active_cycle / PWM_PERIOD) * (WRAP + 1));
        //printf("Duty cycle = %d\n", duty_cycle);
        pwm_set_gpio_level(SERVO_PIN, duty_cycle);
        if (active_cycle >= ACTIVE_CYCLE180) full_range = true;
        sleep_ms(10);
    }
}

int main() {
    stdio_init_all();
    setup_servo_pwm();

    while (true) {
        //initial_servo_routine();
        periodic_servo_motion();
    }
}