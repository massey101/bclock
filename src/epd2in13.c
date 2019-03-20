/**
 *  @filename   :   epd2in13.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     September 9 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdlib.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "epd2in13.h"

volatile unsigned int width;
volatile unsigned int height;
volatile unsigned int reset_pin;
volatile unsigned int dc_pin;
volatile unsigned int cs_pin;
volatile unsigned int busy_pin;
volatile unsigned int busy;
volatile unsigned int asleep;
const unsigned char* volatile lut;


int epd_Init(const unsigned char* _lut) {
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
    width = EPD_WIDTH;
    height = EPD_HEIGHT;
    busy = 0;
    asleep = 0;
    lut = _lut;

    // this calls the peripheral hardware interface, see epdif
    if (epd_if_Init() != 0) {
        return -1;
    }
    // EPD hardware init start
    epd_Reset();
    epd_SendCommand(DRIVER_OUTPUT_CONTROL);
    epd_SendData((EPD_HEIGHT - 1) & 0xFF);
    epd_SendData(((EPD_HEIGHT - 1) >> 8) & 0xFF);
    epd_SendData(0x00);                     // GD = 0; SM = 0; TB = 0;
    epd_SendCommand(BOOSTER_SOFT_START_CONTROL);
    epd_SendData(0xD7);
    epd_SendData(0xD6);
    epd_SendData(0x9D);
    epd_SendCommand(WRITE_VCOM_REGISTER);
    epd_SendData(0xA8);                     // VCOM 7C
    epd_SendCommand(SET_DUMMY_LINE_PERIOD);
    epd_SendData(0x1A);                     // 4 dummy lines per gate
    epd_SendCommand(SET_GATE_TIME);
    epd_SendData(0x08);                     // 2us per line
    epd_SendCommand(DATA_ENTRY_MODE_SETTING);
    epd_SendData(0x03);                     // X increment; Y increment
    epd_SetLut(lut);
    return 0;
}


/**
 *  @brief: basic function for sending commands
 */
void epd_SendCommand(unsigned char command) {
    epd_if_DigitalWrite(dc_pin, LOW);
    epd_if_SpiTransfer(command);
}


/**
 *  @brief: basic function for sending data
 */
void epd_SendData(unsigned char data) {
    epd_if_DigitalWrite(dc_pin, HIGH);
    epd_if_SpiTransfer(data);
}


int busy_cb(int value) {
    if (busy && value == 0) {
        busy = 0;
        return 1;
    }

    return 0;
}


/**
 *  @brief: Wait until the busy_pin goes LOW
 */
void epd_WaitUntilIdle(void) {
    if (epd_if_DigitalRead(busy_pin) == LOW) {
        return;
    }
    busy = 1;
    // Wait for interrupt on PIN PCINT21 and confirm that it is low
    // LOW: idle, HIGH: busy
    epd_if_enable_busy_interrupt(&busy_cb);
    if (epd_if_DigitalRead(busy_pin) == LOW) {
        busy = 0;
        epd_if_disable_busy_interrupt();
        return;
    }
}


/**
 *  @brief: Wait until the busy_pin goes LOW
 */
void epd_WaitUntilIdleShort(void) {
    busy = 1;
    // Wait for interrupt on PIN PCINT21 and confirm that it is low
    // LOW: idle, HIGH: busy
    while (epd_if_DigitalRead(busy_pin) == HIGH);
    busy = 0;
}


int epd_IsBusy(void) {
    return busy;
}


int epd_IsAsleep(void) {
    return asleep;
}


/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see epd_Sleep();
 */
void epd_Reset(void) {
    // Module reset
    epd_if_DigitalWrite(reset_pin, LOW);
    _delay_ms(20);
    epd_if_DigitalWrite(reset_pin, HIGH);
    _delay_ms(20);
}


/**
 *  @brief: set the look-up table register
 */
void epd_SetLut(const unsigned char* _lut) {
    lut = _lut;
    epd_SendCommand(WRITE_LUT_REGISTER);

    // The length of look-up table is 30 bytes
    for (int i = 0; i < 30; i++) {
        epd_SendData(lut[i]);
    }
}


/**
 *  @brief: put an image buffer to the frame memory.
 *          this won't update the display.
 */
void epd_SetFrameMemory(
    const unsigned char* image_buffer,
    int x,
    int y,
    int image_width,
    int image_height
) {
    int x_end;
    int y_end;

    if (
        image_buffer == NULL ||
        x < 0 || image_width < 0 ||
        y < 0 || image_height < 0
    ) {
        return;
    }

    // x point must be the multiple of 8 or the last 3 bits will be ignored
    x &= 0xF8;
    image_width &= 0xF8;
    if (x + image_width >= width) {
        x_end = width - 1;
    } else {
        x_end = x + image_width - 1;
    }

    if (y + image_height >= height) {
        y_end = height - 1;
    } else {
        y_end = y + image_height - 1;
    }

    epd_SetMemoryArea(x, y, x_end, y_end);

    // set the frame memory line by line
    for (int j = y; j <= y_end; j++) {
        epd_SetMemoryPointer(x, j);
        epd_SendCommand(WRITE_RAM);
        for (int i = x / 8; i <= x_end / 8; i++) {
            int buffer_i = (i - x / 8) + (j - y) * (image_width / 8);
            epd_SendData(image_buffer[buffer_i]);
        }
    }
}


/**
 *  @brief: put an image buffer to the frame memory.
 *          this won't update the display.
 *
 *          Question: When do you use this function instead of
 *          void SetFrameMemory(
 *              const unsigned char* image_buffer,
 *              int x,
 *              int y,
 *              int image_width,
 *              int image_height
 *          );
 *          Answer: SetFrameMemory with parameters only reads image data
 *          from the RAM but not from the flash in AVR chips (for AVR chips,
 *          you have to use the function pgm_read_byte to read buffers
 *          from the flash).
 */
void epd_SetFrameMemoryPgm(const unsigned char* image_buffer) {
    epd_SetMemoryArea(0, 0, width - 1, height - 1);

    // set the frame memory line by line
    for (int j = 0; j < height; j++) {
        epd_SetMemoryPointer(0, j);
        epd_SendCommand(WRITE_RAM);
        for (int i = 0; i < width / 8; i++) {
            int buffer_i = i + j * (width / 8);
            epd_SendData(pgm_read_byte(&image_buffer[buffer_i]));
        }
    }
}


/**
 *  @brief: clear the frame memory with the specified color.
 *          this won't update the display.
 */
void epd_ClearFrameMemory(unsigned char color) {
    epd_SetMemoryArea(0, 0, width - 1, height - 1);

    // set the frame memory line by line
    for (int j = 0; j < height; j++) {
        epd_SetMemoryPointer(0, j);
        epd_SendCommand(WRITE_RAM);
        for (int i = 0; i < width / 8; i++) {
            epd_SendData(color);
        }
    }
}


/**
 *  @brief: update the display
 *          there are 2 memory areas embedded in the e-paper display
 *          but once this function is called,
 *          the the next action of SetFrameMemory or ClearFrame will
 *          set the other memory area.
 */
void epd_DisplayFrame(void) {
    epd_SendCommand(DISPLAY_UPDATE_CONTROL_2);
    epd_SendData(0xC4);
    epd_SendCommand(MASTER_ACTIVATION);
    epd_SendCommand(TERMINATE_FRAME_READ_WRITE);
    epd_WaitUntilIdle();
}


/**
 *  @brief: private function to specify the memory area for data R/W
 */
void epd_SetMemoryArea(int x_start, int y_start, int x_end, int y_end) {
    epd_SendCommand(SET_RAM_X_ADDRESS_START_END_POSITION);

    // x point must be the multiple of 8 or the last 3 bits will be ignored
    epd_SendData((x_start >> 3) & 0xFF);
    epd_SendData((x_end >> 3) & 0xFF);
    epd_SendCommand(SET_RAM_Y_ADDRESS_START_END_POSITION);
    epd_SendData(y_start & 0xFF);
    epd_SendData((y_start >> 8) & 0xFF);
    epd_SendData(y_end & 0xFF);
    epd_SendData((y_end >> 8) & 0xFF);
}


/**
 *  @brief: private function to specify the start point for data R/W
 */
void epd_SetMemoryPointer(int x, int y) {
    epd_SendCommand(SET_RAM_X_ADDRESS_COUNTER);

    // x point must be the multiple of 8 or the last 3 bits will be ignored
    epd_SendData((x >> 3) & 0xFF);
    epd_SendCommand(SET_RAM_Y_ADDRESS_COUNTER);
    epd_SendData(y & 0xFF);
    epd_SendData((y >> 8) & 0xFF);
    epd_WaitUntilIdleShort();
}


/**
 *  @brief: After this command is transmitted, the chip would enter the
 *          deep-sleep mode to save power.
 *          The deep sleep mode would return to standby by hardware reset.
 *          You can use epd_Init() to awaken
 */
void epd_Sleep() {
    asleep = 1;
    epd_SendCommand(DEEP_SLEEP_MODE);
    epd_SendData(0x01);
}


/**
 *  @brief: Get the width in pixels of the epaper display.
 */
unsigned int epd_GetWidth() {
    return width;
}


/**
 *  @brief: Get the height in pixels of the epaper display.
 */
unsigned int epd_GetHeight() {
    return height;
}


const unsigned char lut_full_update[] =
{
    0x22, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x11,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00
};


const unsigned char lut_partial_update[] =
{
    0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


/* END OF FILE */
