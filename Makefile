# Configuration
CC = avr-gcc
AR = avr-ar
MCU = atmega328p
F_CPU = 16000000UL
CFLAGS = -Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU)

# Dossiers
SRC_DIR = src
INC_DIR = include
LIB_DIR = pn532-libs

# I2C (pre-built library)
I2C_LIB_DIR = libs/libi2c-atmega328p
I2C_INC_DIR = $(I2C_LIB_DIR)/include
I2C_LIB = $(I2C_LIB_DIR)/libi2c.a

# Fichiers sources (sans i2c.c car déjà compilé)
SOURCES = $(SRC_DIR)/pn532.c $(SRC_DIR)/pn532_i2.c
OBJECTS = $(SOURCES:.c=.o)

# Cible de la bibliothèque
TARGET = $(LIB_DIR)/libpn532.a

.PHONY: all clean install

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(LIB_DIR)/include
	$(AR) rcs $@ $^ $(I2C_LIB)
	cp $(INC_DIR)/*.h $(LIB_DIR)/include/
	cp $(I2C_INC_DIR)/*.h $(LIB_DIR)/include/
	@echo "Bibliothèque créée: $@"

%.o: %.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(I2C_INC_DIR) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -rf $(LIB_DIR)/include

install: $(TARGET)
	@echo "Bibliothèque prête dans $(LIB_DIR)/"
