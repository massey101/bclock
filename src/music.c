#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include "pitches.h"
#include "music.h"


void music_delay_us(uint32_t us) {
    for (int i = 0; i < us; i++) {
        _delay_us(1);
    }
}


void music_init() {
    DDRC |= (1<<PC0);

}


// frequency (in hertz) and duration (in milliseconds).
void tone(unsigned int frequency, unsigned long duration)
{
    long toggle_count = 0;
    uint32_t compare_val = 0;

    // Calculate compare value
    compare_val = F_CPU / frequency / 2 - 1;
    // If compare larger than 16bits, need to prescale (will be DIV64)
    uint8_t prescaler_needed = 0;
    if (compare_val > 0xFFFF){
        // recalculate with new prescaler
        compare_val = F_CPU / frequency / 2 / 64 - 1;
        prescaler_needed = 1;
    }

    // Calculate the toggle count
    if (duration > 0){    // Duration defined
        toggle_count = 2 * frequency * duration / 1000;
    } else {            // Duration not defined -- tone until noTone() call
        toggle_count = -1;
    }

    // Timer settings -- will be type B
    uint8_t status = SREG;
    cli();

    // Disable for now, set clk according to 'prescaler_needed'
    // (Prescaled clock will come from TCA --
    //  by default it should have a prescaler of 64 (250kHz clock)
    // TCA default initialization is in wiring.c -- init()  )
    if(prescaler_needed){
        _timer->CTRLA = TCB_CLKSEL_CLKTCA_gc;
    } else {
        _timer->CTRLA = TCB_CLKSEL_CLKDIV1_gc;
    }

    // Timer to Periodic interrupt mode
    // This write will also disable any active PWM outputs
    _timer->CTRLB = TCB_CNTMODE_INT_gc;

    // Write compare register
    _timer->CCMP = compare_val;

    // Enable interrupt
    _timer->INTCTRL = TCB_CAPTEI_bm;

    timer_outtgl_reg = port_outtgl;
    timer_bit_mask = bit_mask;
    timer_toggle_count = toggle_count;

    // Enable timer
    _timer->CTRLA |= TCB_ENABLE_bm;

    SREG = status;
}


// notes in the song 'Mukkathe Penne'
int melody[] = {
NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_A4,
NOTE_G4, NOTE_C5, NOTE_AS4, NOTE_A4,
NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_FS4, NOTE_DS4, NOTE_D4,
NOTE_C4, NOTE_D4,0,

NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_A4,
NOTE_G4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4,      //29               //8
NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_FS4, NOTE_DS4, NOTE_D4,
NOTE_C4, NOTE_D4,0,

NOTE_D4, NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_DS5, NOTE_D5,
NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_C5,
NOTE_C4, NOTE_D4, NOTE_DS4, NOTE_FS4, NOTE_D5, NOTE_C5,
NOTE_AS4, NOTE_A4, NOTE_C5, NOTE_AS4,             //58

NOTE_D4, NOTE_FS4, NOTE_G4, NOTE_A4, NOTE_DS5, NOTE_D5,
NOTE_C5, NOTE_D5, NOTE_C5, NOTE_AS4, NOTE_C5, NOTE_AS4, NOTE_A4, NOTE_C5, NOTE_G4,
NOTE_A4, 0, NOTE_AS4, NOTE_A4, 0, NOTE_G4,
NOTE_G4, NOTE_A4, NOTE_G4, NOTE_FS4, 0,

NOTE_C4, NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_DS4,
NOTE_C4, NOTE_D4, 0,
NOTE_C4, NOTE_D4, NOTE_G4, NOTE_FS4, NOTE_DS4,
NOTE_C4, NOTE_D4, END

};

// note durations: 8 = quarter note, 4 = 8th note, etc.
int noteDurations[] = {       //duration of the notes
8,4,8,4,
4,4,4,12,
4,4,4,4,4,4,
4,16,4,

8,4,8,4,
4,2,1,1,2,1,1,12,
4,4,4,4,4,4,
4,16,4,

4,4,4,4,4,4,
4,4,4,12,
4,4,4,4,4,4,
4,4,4,12,

4,4,4,4,4,4,
2,1,1,2,1,1,4,8,4,
2,6,4,2,6,4,
2,1,1,16,4,

4,8,4,4,4,
4,16,4,
4,8,4,4,4,
4,20,
};

const int speed=90;  //higher value, slower notes


void music_play_song() {
    for (int i = 0; melody[i] != -1; i++) {
	int noteDuration = speed*noteDurations[i];
	printf("%d\n", melody[i]);
	music_play_tone(melody[i], noteDuration);
    }
}
