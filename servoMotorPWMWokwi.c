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
#include "hardware/timer.h"

#define SERVO_PIN 12
#define WRAP 65535

#define ACTIVE_CYCLE180 2.4
#define ACTIVE_CYCLE90 1.47
#define ACTIVE_CYCLE0 .5

#define PWM_PERIOD 20
#define CLK_DIV 38.147f

#define DUTY_CYCLE_180 7864
#define DUTY_CYCLE_90 4817
#define DUTY_CYCLE_0 1638

// enum para facilitar a logica na primeira rotina temporizada
typedef enum {
    FIRST_ROUTINE_90, 
    FIRST_ROUTINE_0, 
    FIRST_ROUTINE_FINISHED
} first_routine_state;

volatile first_routine_state current_state = FIRST_ROUTINE_90;

// Variaveis para manipular o servo durante a segunda rotina temporizada
float active_cycle;
uint16_t duty_cycle;

uint setup_servo_pwm();

// Temporizadores para controler o servo
bool first_timer_callback(struct repeating_timer *t);
bool second_timer_callback(struct repeating_timer *t);

static struct repeating_timer first_timer;
static struct repeating_timer second_timer;

int main() {
    stdio_init_all();
    setup_servo_pwm();

    // Chamando primeira rotina
    add_repeating_timer_ms(5000, first_timer_callback, NULL, &first_timer);

    // Inicializando o servo em 180 graus
    pwm_set_gpio_level(SERVO_PIN, DUTY_CYCLE_180);
    printf("Servo em 180 graus\n");

    while (true) {
        // No op
        tight_loop_contents();
    }
}

uint setup_servo_pwm() {
    // Inicializando o GPIO como PWM
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(SERVO_PIN);

    // Utilizando configuracoes padroes
    pwm_config config = pwm_get_default_config();

    pwm_config_set_clkdiv(&config, CLK_DIV);
    pwm_init(slice, &config, true);

    return slice;
}

bool first_timer_callback(struct repeating_timer *t) {
    switch (current_state) {
        // First routine
        case FIRST_ROUTINE_90:
            // Configura o servo para 90 graus
            printf("Servo em 90 graus\n");
            pwm_set_gpio_level(SERVO_PIN, DUTY_CYCLE_90);        
            current_state = FIRST_ROUTINE_0;
            break;
        case FIRST_ROUTINE_0:
            // Configura o servo para 0 graus
            printf("Servo em 0 graus\n");
            pwm_set_gpio_level(SERVO_PIN, DUTY_CYCLE_0);
            current_state = FIRST_ROUTINE_FINISHED;
            break;
        case FIRST_ROUTINE_FINISHED:
            // Necessario cancelar timer atual para nao interferir no proximo
            cancel_repeating_timer(&first_timer);
            active_cycle = ACTIVE_CYCLE0;
            // Chamando a segunda rotina
            add_repeating_timer_ms(10, second_timer_callback, NULL, &second_timer);
            break;
        default:
            break;
    }

    return true;
}

bool second_timer_callback(struct repeating_timer *t) {

    // Segunda rotina
    active_cycle += 0.005;
    // Caso o servo chegue em 180 graus
    if (active_cycle >= ACTIVE_CYCLE180) {
        // Cancelar para nao interferir no proximo timer
        cancel_repeating_timer(&second_timer);
        printf("Servo em 180 graus\n");
        // Reinicializando o ciclo ativo
        active_cycle = ACTIVE_CYCLE0;
        // Resetando a flag para controlar as transições de estado do servo na primeira rotina
        current_state = FIRST_ROUTINE_90;
        // Chamando a segunda rotina
        add_repeating_timer_ms(5000, first_timer_callback, NULL, &first_timer);

        return false;
    }
    
    // Calculando o duty cycle
    uint16_t duty_cycle = (uint16_t)((active_cycle/PWM_PERIOD) * (WRAP + 1));
    pwm_set_gpio_level(SERVO_PIN, duty_cycle);
    printf("Duty: %d (%.3f ms)\n", duty_cycle, active_cycle);

    return true;
}