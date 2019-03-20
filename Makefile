################################################################################
# AVR GCC and AVRDUDE makefile
# Made by Jack Massey.
# For the express purpose of programming avr chips.
# Copywrite: None
################################################################################

# AVR options
MCU_TARGET = atmega328
REAL_TARGET = atmega328
PROJ = bclock
PROGRAMMER = avrispmkII
BITRATE = 10

# Compiler options
CC=avr-gcc
CFLAGS=-Wall -mmcu=$(MCU_TARGET) -Os -std=c99
LFLAGS=-Wall
CFLAGS += -DF_CPU=8000000UL

# Directories
BIN = bin
SRC = src
OBJ = obj
MKDIR_P = mkdir -p

# Source
OBJS = \
	$(OBJ)/main.o \
	$(OBJ)/uart.o \
	$(OBJ)/ds3231.o \
	$(OBJ)/datetime.o \
	$(OBJ)/twi.o \
	$(OBJ)/epd2in13.o \
	$(OBJ)/epdif.o \
	$(OBJ)/epdpaint.o \
	$(OBJ)/alarms.o \
	$(OBJ)/pcm_audio.o \
	$(OBJ)/async_delay.o \
	$(OBJ)/view.o \
	$(OBJ)/ui.o \
	$(OBJ)/snd_buzzer.o \
	$(OBJ)/Courier_New12.o \
	$(OBJ)/Courier_New24.o
	#$(OBJ)/snd_wakeup_call.o
LIB =
DEPS = \
	$(SRC)/uart.h \
	$(SRC)/ds3231.h \
	$(SRC)/datetime.h \
	$(SRC)/twi.h \
	$(SRC)/epd2in13.h \
	$(SRC)/epdif.h \
	$(SRC)/epdpaint.h \
	$(SRC)/alarms.h \
	$(SRC)/sounds.h \
	$(SRC)/pcm_audio.h \
	$(SRC)/async_delay.h \
	$(SRC)/callbacks.h \
	$(SRC)/view.h \
	$(SRC)/ui.h \
	$(SRC)/img_bell.h \
	$(SRC)/img_bell_ringing.h \
	$(SRC)/fonts.h

# Output
HEX = $(BIN)/$(PROJ).hex
ELF = $(OBJ)/$(PROJ).elf
MAP = $(OBJ)/$(PROJ).map

all: $(HEX)

$(HEX): $(ELF) $(OBJ) $(BIN)
	avr-size -C --mcu=$(MCU_TARGET) $(ELF)
	avr-objcopy -R .eeprom -O ihex $(ELF) $(HEX)

$(ELF): $(OBJS) $(OBJ)
	avr-gcc $(CFLAGS) -o $(ELF) -Wl,-Map,$(MAP) $(OBJS)

$(OBJS): $(OBJ)/%.o: $(SRC)/%.c $(DEPS) $(OBJ)
	avr-gcc $(CFLAGS) -Os -c -o $@ $<

flash: $(HEX)
	avrdude -v -c $(PROGRAMMER) -P /dev/ttyACM0 -p $(REAL_TARGET) -B $(BITRATE) -F -U flash:w:$(HEX):i

$(BIN) $(OBJ):
	$(MKDIR_P) $@

clean:
	rm -rf $(BIN)/* $(OBJ)/*
