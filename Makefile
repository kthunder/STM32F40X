# Modify names below
SRC_DIR := ./src
BLD_DIR := ./build
OBJ_DIR := $(BLD_DIR)/objs/src
DEP_DIR	:= $(BLD_DIR)/deps/src
ENV_DIR := ./env

TARGET := $(BLD_DIR)/main.elf

CROSS_COMPILE = arm-none-eabi-
AS  = $(CROSS_COMPILE)as
LD  = $(CROSS_COMPILE)ld
CC  = $(CROSS_COMPILE)gcc
CPP = $(CC) -E
AR  = $(CROSS_COMPILE)ar
NM  = $(CROSS_COMPILE)nm

STRIP    = $(CROSS_COMPILE)strip
OBJCOPY  = $(CROSS_COMPILE)objcopy
OBJDUMP  = $(CROSS_COMPILE)objdump

CPU = cortex-m4
ifeq ($(CPU), cortex-m3)
	LD_FILE = STM32F103C6TX_FLASH.ld
	BOARD = STM32F103x6
else
	LD_FILE = STM32F407VETX_FLASH.ld
	BOARD = STM32F407xx
endif

CFLAGS := -Wall -O0 -g
CFLAGS += -mcpu=$(CPU) -mthumb -mthumb-interwork
CFLAGS += -ffunction-sections -fdata-sections -fno-common -fmessage-length=0 -fomit-frame-pointer
CFLAGS += -D $(BOARD)

CFLAGS += -I ./src/Core/Inc
CFLAGS += -I ./src/Core/SpiFlash
CFLAGS += -I ./src/Drivers/HAL_Driver/Inc
CFLAGS += -I ./src/MinRTOS/Inc
# CFLAGS += -I ./src/Drivers/FREE_RTOS/portable/GCC_ARM_CM3
CFLAGS += -I ./src/Drivers/CMSIS/Include
CFLAGS += -I ./src/Drivers/CMSIS/Device/ST/STM32F1xx/Include

LDFLAGS += -T $(ENV_DIR)/$(LD_FILE)
# LDFLAGS += -nostartfiles -Xlinker -MMD -MP
LDFLAGS += -Wl,--gc-sections,--print-memory-usage,-Map="$(basename $(TARGET)).map"
LDFLAGS += -mcpu=$(CPU) -mthumb -mthumb-interwork
LDFLAGS += --specs=nosys.specs --specs=nano.specs

SRC_C = $(shell find $(SRC_DIR) -name "*.c")
SRC_S = $(shell find $(SRC_DIR) -name "*.s")

SRC := $(SRC_C) $(SRC_S)
OBJ := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_C))
OBJ += $(patsubst $(SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(SRC_S))
DEP := $(patsubst $(SRC_DIR)/%.c, $(DEP_DIR)/%.d, $(SRC_C))

.PHONY : all
all : $(TARGET)

include $(DEP)

$(TARGET) : $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@
#	$(OBJCOPY) -Oihex $(TARGET) > $(basename $(TARGET)).hex
#	$(OBJCOPY) -Obinary $(TARGET) > $(basename $(TARGET)).bin
#	$(OBJDUMP) -D $(TARGET) > $(basename $(TARGET)).dis

$(DEP_DIR)/%.d: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) -MM $(CFLAGS) $< | sed 's,\($(notdir $*)\)\.o[ :]*,$(OBJ_DIR)/$*.o $@ : ,g' > $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.s
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY : clean
clean :
	-rm -rf $(DEP_DIR) $(OBJ_DIR)  $(BLD_DIR)

.PHONY : download
download :
	make -j12
	$(ENV_DIR)\openocd.cmd $(abspath $(TARGET))

.PHONY : gen
gen :
	wsl -e bear -- make -j12
