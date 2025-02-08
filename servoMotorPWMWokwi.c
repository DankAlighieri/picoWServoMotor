#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 28
#define WRAP 65535
#define CYCLE180 2.4
#define CYCLE90 1.47
#define CYCLE0 .5
#define PWM_PERIOD 20
#define CLK_DIV 38.147f


// Calcular divisor de clock
/*
    Frequência do PWM = 50Hz
    Período do PWM = 20ms
    clock_div = sys_clock/{freq_pwm * (wrap + 1)}
    wrap = 0xffff (65535)
    sys_clock = 125*10^6
    clock_div = 38,147
*/

void setup_servo_pwm() {
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(SERVO_PIN);
    pwm_config config = pwm_get_default_config();

    pwm_config_set_clkdiv(&config, CLK_DIV);
    pwm_init(slice, &config, true);
}

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

void initial_servo_routine(){
    uint16_t duty_cycle180 = (uint16_t)((CYCLE180/PWM_PERIOD) * (WRAP + 1));
    uint16_t duty_cycle90 = (uint16_t)((CYCLE90/PWM_PERIOD) * (WRAP + 1));
    uint16_t duty_cycle0 = (uint16_t)((CYCLE0/PWM_PERIOD) * (WRAP + 1));

    pwm_set_gpio_level(SERVO_PIN, duty_cycle180); // duty cycle 180
    printf("Servo em 180 graus\n");
    sleep_ms(5000);
    printf("Servo em 90 graus\n");
    pwm_set_gpio_level(SERVO_PIN, duty_cycle90); // duty cycle 90
    sleep_ms(5000);
    printf("Servo em 0 graus\n");
    pwm_set_gpio_level(SERVO_PIN, duty_cycle0); // duty cycle 0
    sleep_ms(5000);
}

void periodic_servo_motion() {

}

int main() {
    stdio_init_all();
    setup_pwm();

    while (true) {
        initial_servo_routine();
    }
}

/*
void wrap_handler(){
    static bool subindo = true;
    static uint nivel = 0;
    pwm_clear_irq(pwm_gpio_to_slice_num(SERVO_PIN));

    if(subindo) {
        nivel++;
        if (nivel > 1000) {
            nivel = 1000;
            subindo = false;
        }
    } else {
        nivel--;
        if(nivel < 0){
            nivel = 0;
            subindo = true;
        }
    }
    pwm_set_gpio_level(SERVO_PIN, nivel * nivel);
}
*/