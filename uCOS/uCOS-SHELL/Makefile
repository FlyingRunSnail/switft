BINPATH=
OUTPATH=

CC=$(BINPATH)arm-none-eabi-gcc
AR=$(BINPATH)arm-none-eabi-ar

# Check for valid float argument
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
#override FLOAT_TYPE = hard
override FLOAT_TYPE = soft
endif
endif

vpath %.c src

CFLAGS  = -g -O2 -Wall
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m3

ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
else
CFLAGS += -msoft-float
endif

CFLAGS += -ffreestanding -nostdlib

# Includes
CFLAGS += -Iinc/ 
CFLAGS += -I$(BASEDIR)/HW/ 
CFLAGS += -I$(BASEDIR)/APP/inc/
CFLAGS += -I$(BASEDIR)/BSP/inc/
CFLAGS += -I$(BASEDIR)/BSP/bsp_stm32F2X7/inc/
CFLAGS += -I$(BASEDIR)/Lib/CMSIS/DeviceSupport/
CFLAGS += -I$(BASEDIR)/Lib/STM32F2xx_StdPeriph_Driver/inc/
CFLAGS += -I$(BASEDIR)/Lib/STM32F2x7_ETH_Driver/inc/
CFLAGS += -I$(BASEDIR)/Lib/CMSIS/
CFLAGS += -I$(BASEDIR)/uCOS/uC-LIB/
CFLAGS += -I$(BASEDIR)/uCOS/uC-CPU/
CFLAGS += -I$(BASEDIR)/uCOS/uC-CPU/ARM-Cortex-M3/GNU/
CFLAGS += -I$(BASEDIR)/uCOS/uCOS-III/Source/
CFLAGS += -I$(BASEDIR)/uCOS/uCOS-III/Ports/ARM-Cortex-M3/Generic/GNU/

# Default to STM32F40_41xxx if no device is passed
ifeq ($(DEVICE_DEF), )
DEVICE_DEF = STM32F2xx
endif

CFLAGS += -D$(DEVICE_DEF)

# Sources
SRCS = cli.c \
	cli_hush.c \
	cli_readline.c \
	cli_simple.c \
	command.c \
	console.c

OBJS = $(SRCS:.c=.o)
LIBNAME = $(OUTPATH)/libshell.a

all: $(LIBNAME)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $(addprefix $(OUTPATH)/, $(notdir $@)) $^

$(LIBNAME): $(OBJS) $(S_OBJS)
	$(AR) -r $@ $(addprefix $(OUTPATH)/, $(notdir $(OBJS)))

clean:
	rm -f $(LIBNAME)
	rm -f $(addprefix $(OUTPATH)/,$(notdir $(OBJS)))
