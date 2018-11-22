# Sources

SRCS = DeviceSupport/system_stm32f2xx.c
S_SRCS = startup/startup_stm32f2xx.s

OUTPATH :=
INCLUDEPATH := $(abspath ./)

AS=$(BINPATH)arm-none-eabi-as
CC=$(BINPATH)arm-none-eabi-gcc
LD=$(BINPATH)arm-none-eabi-gcc
OBJCOPY=$(BINPATH)arm-none-eabi-objcopy
OBJDUMP=$(BINPATH)arm-none-eabi-objdump
SIZE=$(BINPATH)arm-none-eabi-size

CPU = -mcpu=cortex-m3 -mthumb

CFLAGS  = $(CPU) -c -std=gnu99 -g -O2 -Wall

ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
else
CFLAGS += -msoft-float
endif

# Default to STM32F40_41xxx if no device is passed
ifeq ($(DEVICE_DEF), )
DEVICE_DEF = STM32F2xx
endif

CFLAGS += -D$(DEVICE_DEF)
CFLAGS += -I$(INCLUDEPATH)


OBJS = $(SRCS:.c=.o)
OBJS += $(S_SRCS:.s=.o)

all:$(OBJS)

.s.o:
	$(AS) $(CPU) -o $(addprefix $(OUTPATH)/, $(notdir $@)) $<

.c.o:
	$(CC) $(CFLAGS) -std=gnu99 -o $(addprefix  $(OUTPATH)/, $(notdir $@)) $<

