export CC        = avr-gcc
export LD        = avr-gcc
export AR        = avr-ar
export OBJCOPY   = avr-objcopy
export SIZE      = avr-size

# MCU
export MCU   = atmega32u4
export FCPU  = 16000000
export FLAGS = -mmcu=$(MCU)

# Paths (adjust if needed)
# i2c expected at ../libs/i2c with subdirs: include/, src/
I2C_DIR        := ../libs/i2c
SSD1306_DIR   := ssd1306

# Includes
export INCLUDES      = -I../include
export I2C_INCLUDES  = -I$(I2C_DIR)/include
export SSD1306_INCLUDES = -I$(SSD1306_DIR)/include
export CFLAGS        = -Wall $(FLAGS) -DF_CPU=$(FCPU) -Os $(INCLUDES) $(I2C_INCLUDES) $(SSD1306_INCLUDES)
export LDFLAGS       = $(FLAGS)

# Programmer
export PROGRAMMER = dfu-programmer

# Targets
TARGET  = main

# Your PN532/app sources (do NOT put i2c sources here)
SOURCES  = $(wildcard *.c ../src/*.c)
OBJECTS  = $(SOURCES:.c=.o)

# i2c static library (compiled once)
I2C_SRCS := $(wildcard $(I2C_DIR)/src/*.c)
I2C_OBJS := $(I2C_SRCS:.c=.o)
LIB_I2C  := libi2c.a

# ssd1306 static library (compiled once)
SSD1306_SRCS := $(wildcard $(SSD1306_DIR)/src/*.c)
SSD1306_OBJS := $(SSD1306_SRCS:.c=.o)
LIB_SSD1306  := libssd1306.a

.PHONY: all clean upload size

all: $(TARGET).hex

# Link final ELF with the static libs
$(TARGET): $(OBJECTS) $(LIB_I2C) $(LIB_SSD1306)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LIB_I2C) $(LIB_SSD1306)

# Build static lib from i2c objects
$(LIB_I2C): $(I2C_OBJS)
	$(AR) rcs $@ $^

# Build static lib from ssd1306 objects
$(LIB_SSD1306): $(SSD1306_OBJS)
	$(AR) rcs $@ $^

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
	rm -f *.o $(TARGET).hex $(TARGET) ../src/*.o $(I2C_OBJS) $(LIB_I2C) $(SSD1306_OBJS) $(LIB_SSD1306)
