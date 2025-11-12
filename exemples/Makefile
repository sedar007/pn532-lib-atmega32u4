export CC        = avr-gcc
export LD        = avr-gcc
export AR        = avr-ar
export OBJCOPY   = avr-objcopy
export SIZE      = avr-size

# MCU
export MCU   = atmega32u4
export FCPU  = 16000000
export FLAGS = -mmcu=$(MCU)


# I2C
I2C_LIB_DIR = ../libs/libi2c-atmega328p
I2C_INC_DIR = $(I2C_LIB_DIR)/include
LIB_I2C     = $(I2C_LIB_DIR)/libi2c.a

# SSD1306
SSD1306_LIB_DIR = libssd1306-atmega328p
SSD1306_INC_DIR = $(SSD1306_LIB_DIR)/include
LIB_SSD1306     = $(SSD1306_LIB_DIR)/libssd1306.a


# Includes
export INCLUDES      = -I../include

export SSD1306_INCLUDES = -I$(SSD1306_DIR)/include
export CFLAGS        = -Wall $(FLAGS) -DF_CPU=$(FCPU) -Os $(INCLUDES) -I$(SSD1306_INC_DIR) $(SSD1306_INCLUDES) -I$(I2C_INC_DIR) -I$(SSD1306_INC_DIR)
export LDFLAGS       = $(FLAGS) -L$(I2C_LIB_DIR) -li2c -L$(SSD1306_LIB_DIR) -lssd1306

# Programmer
export PROGRAMMER = dfu-programmer

# Targets
TARGET  = main


SOURCES  = $(wildcard *.c ../src/*.c)
OBJECTS  = $(SOURCES:.c=.o)


.PHONY: all clean upload size

all: $(TARGET).hex

# Link final ELF with the static libs
$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LIB_I2C) $(LIB_SSD1306)


# Hex and utility targets
$(TARGET).hex: $(TARGET)
	$(OBJCOPY) -j .text -j .data -O ihex $(TARGET) $(TARGET).hex

upload: $(TARGET).hex
	$(PROGRAMMER) $(MCU) erase
	$(PROGRAMMER) $(MCU) flash $(TARGET).hex
	$(PROGRAMMER) $(MCU) reset

size: $(TARGET)
	$(SIZE) --format=avr --mcu=$(MCU) $(TARGET)

clean:
	rm -f *.o $(TARGET).hex $(TARGET) ../src/*.o $(SSD1306_OBJS)
