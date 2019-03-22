// This code is designed to support the bclock or Barrukka Clock.

#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>

#include "tasks.h"
#include "uart.h"
#include "reactor.h"
#include "buttons.h"

static volatile char input = 0;
ms_t ms_since_last_input = 0;

void button_press(char _input) {
    input = _input;

    reactor_trigger(TASK_PRINT);
    reactor_update();
}


void flash_led_task(ms_t realms) {
    PORTC ^= _BV(PC3);
    reactor_call_later(TASK_FLASH_LED, &flash_led_task, 1000);
}

void print_task(ms_t realms) {
    ms_since_last_input += realms;

    if (input == 0) {
        printf("No chars for %lds\n", ms_since_last_input/1000);
    } else {
        printf("Got: %c (%ldms since last)\n", input, ms_since_last_input);
        input = 0;
        ms_since_last_input = 0;
    }

    reactor_call_later(TASK_PRINT, &print_task, 20000);
}

int main(void) {
    /* init */
    uart_init_interrupt(38400, &button_press);
    stdout = &uart_stdout;
    stdin = &uart_input;
    buttons_init(&button_press);
    printf("init\n");
    DDRC |= _BV(PC3);

    printf("Test3\n");
    reactor_call_later(TASK_FLASH_LED, &flash_led_task, 1000);
    reactor_call_later(TASK_PRINT, &print_task, 1000);

    _delay_ms(2000);
    reactor();

    return 0;
}

