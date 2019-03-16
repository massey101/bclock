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
	$(OBJ)/twi.o \
	$(OBJ)/epd2in13b.o \
	$(OBJ)/epdif.o \
	$(OBJ)/epdpaint.o \
	$(OBJ)/DejaVuSansMono24.o \
	$(OBJ)/DejaVuSansMono48.o
LIB =
DEPS = \
	$(SRC)/uart.h \
	$(SRC)/ds3231.h \
	$(SRC)/twi.h \
	$(SRC)/epd2in13b.h \
	$(SRC)/epdif.h \
	$(SRC)/epdpaint.h \
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
