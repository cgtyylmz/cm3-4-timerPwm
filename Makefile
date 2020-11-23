PROJECT = main

# MCU
MCU = cortex-m0

# Debug Level
DBG = -g3

# Optimisation Level
OPT = -O0

# Definations
DEF = -DSTM32F0

# Linbrary
LIB =opencm3_stm32f0


#####################################################################
#                      DEFAULT DIRECTORIES                          #
#####################################################################
# Source Files
SRC = ./src

# Object Files
OBJ_DIR = ./obj

# Binary Files
BUILD_DIR = ./build

# Include directories
INCDIR = ./inc ./libopencm3/include

# Library directories
LIBDIR = ./libopencm3/lib

# Linker script file
LINKER = ./Stm32f0.ld

# Add all source files from src/ directory
SRC_FILES = $(wildcard $(SRC)/*.c)

# Header file
INC = $(patsubst %,-I%, $(INCDIR))


################################################################
# TOOLS
################################################################

PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
GDB = $(PREFIX)gdb
CP = $(PREFIX)objcopy
AS = $(PREFIX)gcc -x assembler-with-cpp
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S


################################################################
# FILES
################################################################

OBJ_FILES = $(addprefix $(OBJ_DIR)/,$(notdir $(SRC_FILES:.c=.o)))
vpath %.c $(sort $(dir $(SRC_FILES)))

################################################################
# FLAGS
################################################################

# Common Flags
COMFLAGS = -mcpu=$(MCU) -msoft-float $(DBG) -mthumb

# Compiler Flags
CPFLAGS = $(OPT) $(COMFLAGS) $(INC) $(DEF) -MD -Wall -Wundef
CPFLAGS	+= -Wextra -Wshadow -Wimplicit-function-declaration
CPFLAGS	+= -Wredundant-decls #-Wmissing-prototypes -Wstrict-prototypes
CPFLAGS	+= -fno-common -ffunction-sections -fdata-sections
CPFLAGS	+= -fno-builtin -fno-exceptions -fno-unwind-tables


# Linker Flags
LDFLAGS += --static -nostartfiles -T$(LINKER) -mthumb -mcpu=cortex-m0
LDFLAGS += -msoft-float -ggdb3 -Wl,-Map=$(BUILD_DIR)/$(PROJECT).map -Wl,--cref -Wl,--gc-sections
LDFLAGS += -L$(LIBDIR)
LIBFLAGS += -l$(LIB) -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group


################################################################
# RULES
################################################################

all:  $(BUILD_DIR) $(OBJ_FILES) $(BUILD_DIR)/$(PROJECT).elf  $(BUILD_DIR)/$(PROJECT).hex $(BUILD_DIR)/$(PROJECT).bin

$(OBJ_DIR)/%.o:: %.c
	$(info CC	$<)
	@$(CC) $(CPFLAGS) -o $@ -c $<

$(BUILD_DIR)/%.elf: $(OBJ_FILES)
	$(info LD	$@)
	@$(CC)  $(LDFLAGS) $(INC) $(OBJ_FILES) $(LIBFLAGS) -o $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	$(info HEX	$<)
	@$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	$(info BIN	$<)
	@$(BIN) $< $@

$(BUILD_DIR):
	$(info Creating directories...)
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJ_DIR)

flash: $(BUILD_DIR)/$(PROJECT).bin
	st-flash write ./build/main.bin 0x08000000

clean:
	rm -fR ./build ./obj

test:
	echo $(SRC_FILES) $(OBJ_FILES)
