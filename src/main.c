// This code is designed to support the bclock or Barrukka Clock.

#include <util/delay.h>
#include <stdint.h>
#include <avr/io.h>
#include <stdio.h>

#include "uart.h"
#include "spi.h"
#include "st7735.h"
#include "st7735_gfx.h"
#include "st7735_font.h"

#include "fonts/tom_thumb.h"


void delayms(uint16_t ms) {
    while (ms) {
        _delay_ms(1);
        ms--;
    }
}


int main(void)
{
	/* init */
        spi_init();
        st7735_init();
        st7735_set_orientation(ST7735_LANDSCAPE);
	uart_init(38400);
	stdout = &uart_stdout;
        stdin = &uart_input;

        DDRC |= (1 << DDC5);

        // Reset with all white
        st7735_fill_rect(0, 0, 128, 128, ST7735_COLOR_WHITE);
        st7735_draw_text(
            5,
            12,
            "Time: ",
            &TomThumb,
            1,
            ST7735_COLOR_BLACK
        );
        st7735_draw_text(
            5,
            30,
            "Yo Alex.",
            &TomThumb,
            1,
            ST7735_COLOR_BLACK
        );

        char input;

        while(1) {
            delayms(2000);

            PORTC |= (1 << PORTC5);
            printf("\nPlease select a character: ");
            input = getchar();
            printf("%c\n", input);
            delayms(500);
            PORTC &= ~(1 << PORTC5);
            printf("You chose %c.\n", input);
        }

	return 0;
}
